#include "mierzonepozycje.h"
#include "ui_mierzonepozycje.h"
#include <QTableWidgetItem>
#include <QTimer>
#include <QMutexLocker>
#include <QPlainTextEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

#include "mechanika.h"

MierzonePozycje::MierzonePozycje(QWidget *parent) :
    QWidget(parent),
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
    timer->start(1000);

    ui->pbNoweDane->setVisible(true);
    //ui->pbNoweDane->setEnabled(false);
    ui->pbZapisz->setEnabled(false);
    ui->debug->setVisible(false);
    ui->localdebug->setVisible(false);

}

MierzonePozycje::~MierzonePozycje()
{
    delete ui;
    timer->stop();
}

void MierzonePozycje::setList(const Pozycje &pos)
{
    debug("Set List");
    setIsStart(false);
    m_lista = pos;
    ui->table->setRowCount(pos.size());
    for(int r = 0; r < pos.size(); ++r) {
        QTableWidgetItem *item;
        item = new QTableWidgetItem(QString::number(pos.at(r).x));
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        item->setFlags(item->flags() ^ Qt::ItemIsSelectable);
        ui->table->setItem(r, 0, item);

        item = new QTableWidgetItem(QString::number(pos.at(r).y));
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        item->setFlags(item->flags() ^ Qt::ItemIsSelectable);
        ui->table->setItem(r, 1, item);

        item = new QTableWidgetItem(QString::number(pos.at(r).time));
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        item->setFlags(item->flags() ^ Qt::ItemIsSelectable);
        ui->table->setItem(r, 2, item);

        item = new QTableWidgetItem(QString::fromUtf8("--"));
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        item->setFlags(item->flags() ^ Qt::ItemIsSelectable);
        ui->table->setItem(r, 3, item);

        item = new QTableWidgetItem(QString::fromUtf8("W oczekiwaniu"));
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        item->setFlags(item->flags() ^ Qt::ItemIsSelectable);
        ui->table->setItem(r, 4, item);
    }
    m_listawynikowa1.clear();
    m_listawynikowa2.clear();
    actStatus = WAIT;
    actPos = 0;
    //ui->pbNoweDane->setEnabled(false);
    ui->pbZapisz->setEnabled(false);
    ui->pbStart->setEnabled(true);
}


void MierzonePozycje::setMechanika(const Ruch & m)
{
    mech = m;
}


void MierzonePozycje::update()
{
    if (getIsStart()) {
        if (getIsWait())
            return;

        if (actStatus == WAIT)
            return;


        if (actStatus == POSITIONING) {
            if (actPos >= m_lista.size()) {
                setIsStart(false);
                actStatus = WAIT;
                emit statusMiernik("Zakonczono ustawianie wszystkich pozycji");
                ui->status->setText("Zakonczono ustawianie wszystkich pozycji");
                emit end();
                ui->pbStart->setEnabled(false);
                ui->pbNoweDane->setEnabled(true);
                ui->pbZapisz->setEnabled(true);
                return;
            }

            //ustaw na pozycji
            unsigned int xmm = m_lista.at(actPos).x;
            unsigned int ymm = m_lista.at(actPos).y;
            qDebug("xmm=%d",xmm);
            qDebug("ymm=%d",ymm);
            actMeasure = 2*m_lista.at(actPos).time;
            ui->table->item(actPos, 4)->setText(QString::fromUtf8("Ustawiam pozycję"));
            uint32_t impx, impy;
            impx = mech.getImpulsyX(xmm);
            impy = mech.getImpulsyY(ymm);
            qDebug("xmm=%d",impx);
            qDebug("ymm=%d",impy);
            debug(QString("impulsy (%1,%2) => (%3,%4)").arg(xmm).arg(ymm).arg(impx).arg(impy));
            actStatus = MEASURING;
            emit setPosition(impx, impy);
            ui->status->setText(QString("Rozpoczynam ustawianie pozycji %1 mm %2 mm").arg(xmm).arg(ymm));
            setIsWait( true );
            avg1 = 0.0;
            cnt1 = 0;
        } else if (actStatus == MEASURING) {
            ui->status->setText(QString("Pozycja %1 mm %2 mm ustawiona. Średni pomiar %3").arg(m_lista.at(actPos).x).arg(m_lista.at(actPos).y).arg(avg1));
            ui->table->item(actPos, 4)->setText(QString::fromUtf8("Trwa pomiar. Zostało %1 s").arg(actCzas));
            debug(QString("Pomiar [%1 s]").arg(actCzas));
            emit readRadio();

            if (actCzas == 0) {
                actStatus = NEXTPOSITION;
            } else {
                --actCzas;
            }
        } else if (actStatus == NEXTPOSITION) {
            debug(QString("Ukonczono"));
            ui->status->setText("Zakonczono pomiary");
            actStatus = POSITIONING;
            ui->table->item(actPos, 4)->setText(QString::fromUtf8("Ukończono"));
            DaneWynikowe2 data;
            data.x = m_lista.at(actPos).x;
            data.y = m_lista.at(actPos).y;
            data.val1 = avg1;
            m_listawynikowa2.append(data);
            ++actPos;

            ui->table->scrollToItem(ui->table->item(actPos, 0));
        }
    }
}

void MierzonePozycje::readedFromRadio(int val)
{
    setValue1(0.01*val, "m/s");
}

bool MierzonePozycje::getIsStart()
{
    const QMutexLocker locker(&m_mutex);
    return isStart;
}

void MierzonePozycje::setIsStart(bool value)
{
    debug(QString("Set start %1").arg(value));
    const QMutexLocker locker(&m_mutex);
    isStart = value;
    actStatus = POSITIONING;
    actPos = 0;
}

bool MierzonePozycje::getIsWait()
{
    const QMutexLocker locker(&m_mutex2);
    return isWait;
}

void MierzonePozycje::setIsWait(bool value)
{
    debug(QString("Set wait %1").arg(value));
    const QMutexLocker locker(&m_mutex2);
    isWait = value;
}

void MierzonePozycje::setDebug(const QString & val)
{
    //ui->debug->append(val);
}

void MierzonePozycje::debug(const QString & val)
{
    //qDebug("%s",val.toStdString().c_str());
    //ui->localdebug->append(val);
}

void MierzonePozycje::setValues(const float &val1)
{
    DaneWynikowe1 data;
    data.x = m_lista[actPos].x;
    data.y = m_lista[actPos].y;
    data.time = m_lista[actPos].time - actCzas;
    data.val1 = val1;
    m_listawynikowa1.append(data);
    if ((int)actSize == m_listawynikowa1.size()) {
        actStatus = NEXTPOSITION;
        actMeasure = 0;
    }

}

void MierzonePozycje::setValue1(const float &val, const QString &unit)
{
    if (actStatus != MEASURING)
        return;
    setValues(val);
    avg1 = (avg1*cnt1 + val)/(cnt1 + 1);
    ++cnt1;
    ui->table->item(actPos, 3)->setText(QString::fromUtf8("%1 %2").arg(avg1, 0, 'g', 3).arg(unit));
}

void MierzonePozycje::setConnected(bool newConnected)
{
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
    //debug("Start");
    //emit start();
    //if (!connected)
    emit doConnect();
    //else
    //    emit checkDevice();
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

    for (int id = 0; id < m_listawynikowa1.size(); ++id)
    {
        DaneWynikowe1 d = m_listawynikowa1.at(id);
        out << d.x << ";" << d.y << ";" << d.time << ";" << d.val1 << "\n";
    }
    file.close();
}

