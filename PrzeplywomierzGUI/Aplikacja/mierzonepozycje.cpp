#include "mierzonepozycje.h"
#include "ui_mierzonepozycje.h"
#include <QTableWidgetItem>
#include <QTimer>
#include <QMutexLocker>
#include <QPlainTextEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>


#include "mechanika.h"
#include "wybranyplik.h"
#include "wyborkwadratow.h"
#include "recznedodpozycji.h"

MierzonePozycje::MierzonePozycje(QWidget *parent) :
    TabWidget(parent),
    ui(new Ui::MierzonePozycje)
{
    ui->setupUi(this);
    QStringList headers;
    headers << QString::fromUtf8("Pozycja X\n[mm]") << QString::fromUtf8("Pozycja Y\n[mm]") << QString::fromUtf8("Czas\n[s]");
    headers << QString::fromUtf8("Średnia wartość\nczujnik");
    headers << QString::fromUtf8("Status");
    ui->table->setColumnCount(headers.size());
    ui->table->setHorizontalHeaderLabels(headers);
    ui->table->setAutoScroll(true);
    ui->table->resizeRowsToContents();
    ui->table->resizeColumnsToContents();
    ui->table->horizontalHeader()->setStretchLastSection(true);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    actStatus = WAIT;
    actPos = 0;
    connected = false;
    timer->setInterval(1000);

    ui->pbNoweDane->setVisible(true);
    //ui->pbNoweDane->setEnabled(false);
    ui->pbZapisz->setEnabled(false);
    started = false;
}

MierzonePozycje::~MierzonePozycje()
{
    delete ui;
    timer->stop();
}

void MierzonePozycje::setList(const Pozycje &pos)
{
    m_lista = pos;
    ui->table->setRowCount(pos.size());
    for(int r = 0; r < pos.size(); ++r) {
        QTableWidgetItem *item;
        item = new QTableWidgetItem(QString::number(pos.at(r).x));
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        item->setFlags(item->flags() ^ Qt::ItemIsSelectable);
        ui->table->setItem(r, col_X, item);

        item = new QTableWidgetItem(QString::number(pos.at(r).y));
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        item->setFlags(item->flags() ^ Qt::ItemIsSelectable);
        ui->table->setItem(r, col_Y, item);

        item = new QTableWidgetItem(QString::number(pos.at(r).time));
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        item->setFlags(item->flags() ^ Qt::ItemIsSelectable);
        ui->table->setItem(r, col_time, item);

        item = new QTableWidgetItem(QString::fromUtf8("--"));
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        item->setFlags(item->flags() ^ Qt::ItemIsSelectable);
        ui->table->setItem(r, col_meas, item);

        item = new QTableWidgetItem(QString::fromUtf8("W oczekiwaniu"));
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        item->setFlags(item->flags() ^ Qt::ItemIsSelectable);
        ui->table->setItem(r, col_status, item);
    }
    m_listawynikowa.clear();
    actStatus = WAIT;
    actPos = 0;
    //ui->pbNoweDane->setEnabled(false);
    ui->pbZapisz->setEnabled(false);
    ui->pbStart->setEnabled(true);
}

void MierzonePozycje::setPos()
{
    unsigned int xmm = m_lista.at(actPos).x;
    unsigned int ymm = m_lista.at(actPos).y;

    actCzas = m_lista.at(actPos).time;
    ui->table->item(actPos, col_status)->setText(QString::fromUtf8("Ustawiam pozycję"));
    uint32_t impx, impy;
    impx = mech.getImpulsyX(xmm);
    impy = mech.getImpulsyY(ymm);

    debug(QString("impulsy (%1,%2) => (%3,%4)").arg(xmm).arg(ymm).arg(impx).arg(impy));
    actStatus = WAIT_POS;
    ui->status->setText(QString("Rozpoczynam ustawianie pozycji %1 mm %2 mm").arg(xmm).arg(ymm));
    setPosition(impx, impy);
    avg1 = 0.0;
    cnt1 = 0;
    qDebug("xmm=%d",impx);
    qDebug("ymm=%d",impy);
}

void MierzonePozycje::update()
{
    //debug(QString("Update %1").arg(actStatus));
    switch(actStatus) {
    case WAIT:
    case WAIT_POS:
    case WAIT_HPOS:
    default:
        return;

    case FIRST_RUN:
    {
        debug("First run");
        ui->pbNoweDane->setEnabled(false);
        ui->pbZapisz->setEnabled(false);
        actPos = 0;
        if (m_lista.size() == 0) {
            actStatus = WAIT;
            timer->stop();
            ui->pbNoweDane->setEnabled(true);
            return;
        }
        actStatus = WAIT_POS;
        avg1 = 0;
        cnt1 = 0;
        setPos();
        return;
    }
    case NEXT_POS:
    {
        debug(QString("Ukonczono pomiary"));
        ui->status->setText("Zakonczono pomiary");
        ui->table->item(actPos, col_status)->setText(QString::fromUtf8("Ukończono"));
        ++actPos;
        avg1 = 0;
        cnt1 = 0;

        if (actPos >= m_lista.size()) {
            actStatus = WAIT;
            timer->stop();
            ui->pbNoweDane->setEnabled(true);
            ui->pbZapisz->setEnabled(true);
            ui->status->setText("Zakonczono pomiar dla wszystkich pozycji z listy");
            setPositionHome();
            return;
        }
        ui->table->scrollToItem(ui->table->item(actPos, 0));

        if (actPos % 10 == 0) {
            actStatus = WAIT_HPOS;
            debug("Set Home pos");
            setPositionHome();
            ui->status->setText("Trwa kalibracja urządzenia");
            return;
        }

        actStatus = WAIT_POS;
        setPos();
        return;
    }
    case NEXT_POS_AFTER_HPOS:
    {
        qDebug("Nex pos after hpos");
        actStatus = WAIT_POS;
        setPos();

        return;
    }
    case MEASURING:
    {
        ui->status->setText(QString("Pozycja %1 mm %2 mm ustawiona. Średni pomiar %3").arg(m_lista.at(actPos).x).arg(m_lista.at(actPos).y).arg(avg1));
        ui->table->item(actPos, col_status)->setText(QString::fromUtf8("Trwa pomiar. Zostało %1 s").arg(actCzas));
        debug(QString("Pomiar [%1 s] %2").arg(actCzas));
        readRadio();

        if (actCzas == 0) {
            actStatus = NEXT_POS;
        }
        return;
    }
    }
}

void MierzonePozycje::positionDone(bool home)
{
    debug(QString("position Done home=%1").arg(home));
    if (home) {
        if (started && actStatus == WAIT_HPOS) {
            actStatus = NEXT_POS_AFTER_HPOS;
        } else if (!started && actStatus == WAIT){
            started = true;
            actStatus = FIRST_RUN;
            timer->start();
        }
    } else {
        if (started && actStatus == WAIT_POS) {
            actStatus = MEASURING;
        }
    }
}

void MierzonePozycje::readedFromRadio(const double &val)
{
    debug(QString("Read from radio %1").arg(val));
    setValue1(val, "m/s");
    --actCzas;
}

void MierzonePozycje::errorReadFromRadio()
{
    --actCzas;
}

/*
void MierzonePozycje::readedFromRadio(int val)
{
    setValue1(0.01*val, "m/s");
}
*/

const WyborMetodyData &MierzonePozycje::getAllValues() const
{
    return allValues;
}

void MierzonePozycje::errorSerial(const QString &)
{
    ui->pbNoweDane->setEnabled(true);
    timer->stop();
    ui->pbZapisz->setEnabled(true);
    ui->table->item(actPos, col_status)->setText("Błąd sterownika");
    ui->status->setText("Wystąpił błąd sterownika. Przerywam pracę.");
}





void MierzonePozycje::setValues(const float &val1)
{
    DaneWynikowe data;
    data.x = m_lista[actPos].x;
    data.y = m_lista[actPos].y;
    data.time = /*m_lista[actPos].time - actCzas;*/cnt1+1;
    data.val1 = val1;
    avg1 = (cnt1*avg1 + val1)/(cnt1 + 1);
    cnt1++;
    m_listawynikowa.append(data);
}

void MierzonePozycje::setValue1(const float &val, const QString &unit)
{
    if (actStatus != MEASURING)
        return;
    setValues(val);
    avg1 = (avg1*cnt1 + val)/(cnt1 + 1);
    ++cnt1;
    ui->table->item(actPos, col_meas)->setText(QString::fromUtf8("%1 %2").arg(avg1, 0, 'g', 3).arg(unit));
}

void MierzonePozycje::setConnected(bool newConnected)
{
    TabWidget::setConnect(newConnected);
    connected = newConnected;
}

void MierzonePozycje::status(const QString &st)
{
    ui->status->setText(st);
}

void MierzonePozycje::restart()
{
    actStatus = WAIT;
    actPos = 0;
    connected = false;
    ui->pbNoweDane->setEnabled(false);
    ui->pbZapisz->setEnabled(false);
}

void MierzonePozycje::on_pbStart_clicked()
{
    debug(QString("Start work. Isconnected %1").arg(getConnect()));
    //if (getConnect()) {
    //    debug("set home position");
    //    setPositionHome();
    //    ui->pbStart->setEnabled(false);
    //} else {
    //    debug("connect to device");
    connectToDevice();
    //}
}




void MierzonePozycje::on_pbNoweDane_clicked()
{
    QCoreApplication::exit(1);
}


void MierzonePozycje::on_pbZapisz_clicked()
{

    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Zapisanie wyników"), "",
            tr("Pliki csv (*.csv);;Wszystkie pliki (*)"));
    if (fileName.isEmpty())
            return;
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("Nie można otworzyć pliku"),
            file.errorString());
        return;
    }

    QTextStream out(&file);

    for (int id = 0; id < m_listawynikowa.size(); ++id)
    {
        DaneWynikowe d = m_listawynikowa.at(id);
        out << d.x << ";" << d.y << ";" << d.time << ";" << d.val1 << "\n";
    }
    file.close();
}

