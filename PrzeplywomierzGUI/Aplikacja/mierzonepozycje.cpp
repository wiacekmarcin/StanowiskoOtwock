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
#include "miernikprzeplywu.h"

#define DEBUGMPZ(X) debug(QString("%1:%2 %3").arg(__FILE__).arg(__LINE__).arg(X))

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
    actWork = WAIT_FOR_READY;
    actPos = 0;
    timer->setInterval(1000);

    //ui->pbRestart->setEnabled(false);
    ui->pbRestart->setVisible(false);
    ui->pbZapisz->setEnabled(false);
    started = false;

    ui->table->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
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
    if (pos.size() == 0) {
        ui->pbStart->setEnabled(false);
        return;
    }
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
    actWork = WAIT_FOR_READY;
    actPos = 0;
    //ui->pbNoweDane->setEnabled(false);
    ui->pbZapisz->setEnabled(false);
    ui->pbStart->setEnabled(true);
    adjustSize();
}

void MierzonePozycje::setPos()
{
    unsigned int xmm = m_lista.at(actPos).x;
    unsigned int ymm = m_lista.at(actPos).y;

    ui->table->item(actPos, col_status)->setText(QString::fromUtf8("Ustawiam pozycję"));
    uint32_t impx, impy;
    impx = mech.getImpulsyX(xmm);
    impy = mech.getImpulsyY(ymm);

    DEBUGMPZ(QString("impulsy (%1,%2) => (%3,%4)").arg(xmm).arg(ymm).arg(impx).arg(impy));
    actWork = WAIT_POS;
    ui->status->setText(QString("Rozpoczynam ustawianie pozycji %1 mm %2 mm").arg(xmm).arg(ymm));
    setPosition(impx, impy);
    avg1 = 0.0;
    cnt1 = 0;
}

void MierzonePozycje::update()
{
    //DEBUGMPZ(QString("Update %1").arg(actWork));
    if (!getIsReady())
        return;

    switch(actWork) {
    case WAIT:
    case WAIT_POS:
    case WAIT_HPOS:
    default:
        DEBUGMPZ("actWork WAIT...");
        return;

    case WAIT_FOR_READY:
    {
        DEBUGMPZ("actWork WAIT_FOR_READY");
        ui->pbNoweDane->setEnabled(false);
        ui->pbZapisz->setEnabled(false);
        ui->pbStart->setEnabled(false);
        ui->status->setText("Rozpoczynam prace");
        actWork = FIRST_RUN;
        return;
    }

    case DONE:
    {
        DEBUGMPZ("actWork DONE");
        timer->stop();
        return;
    }

    case END:
    {
        DEBUGMPZ("actWork END");
        timer->stop();
        ui->pbNoweDane->setEnabled(true);
        ui->pbZapisz->setEnabled(true);
        ui->pbRestart->setEnabled(true);
        ui->status->setText("Zakonczono pomiar dla wszystkich pozycji z listy");
        actWork = DONE;
        setClose(true);
        return;
    }

    case FIRST_RUN:
    {
        DEBUGMPZ("actWork FIRST_RUN");
        actPos = 0;
        if (m_lista.size() == 0) {
            actWork = DONE;
            timer->stop();
            ui->pbNoweDane->setEnabled(true);
            return;
        }
        actWork = WAIT_POS;
        avg1 = 0;
        cnt1 = 0;
        actCzas = m_lista.at(actPos).time;
        setPos();
        return;
    }
    case NEXT_POS:
    {
        DEBUGMPZ("actWork NEXT_POS");
        DEBUGMPZ(QString("Szukam kolejnej pozycji"));
        ui->status->setText("Przechodzę do kolejnej pozycji z listy");
        ui->table->item(actPos, col_status)->setText(QString::fromUtf8("Ukończono"));
        ++actPos;


        if (actPos >= m_lista.size()) {
            actWork = END;
            setPositionHome();
            return;
        }
        ui->table->scrollToItem(ui->table->item(actPos, 0));
/*
        if (actPos % 100 == 0) {
            actWork = WAIT_HPOS;
            DEBUGMPZ("Set Home pos");
            setPositionHome();
            ui->status->setText("Trwa kalibracja urządzenia");
            return;
        }
*/
        actWork = WAIT_POS;
        actCzas = m_lista.at(actPos).time;
        setPos();
        return;
    }
    /*
    case NEXT_POS_AFTER_HPOS:
    {
        DEBUGMPZ("actWork NEXT_POS_AFTER_HPOS");
        actWork = WAIT_POS;
        actCzas = m_lista.at(actPos).time;
        setPos();
        return;
    }
    */
    case START_MEASURING:
    {
        DEBUGMPZ("actWork START_MEASURING");
        avg1 = 0.0;
        cnt1 = 0;
        ui->status->setText(QString("Pozycja %1 mm %2 mm ustawiona. Rozpoczynam pomiary").arg(m_lista.at(actPos).x).arg(m_lista.at(actPos).y));
        actWork = NOW_MEASURING;
        return;
    }
    case NOW_MEASURING:
    {
        DEBUGMPZ("actWork NOW_MEASURING");
        if (actCzas == cnt1) {
            actWork = END_MEASURING;
            ui->table->item(actPos, col_status)->setText(QString::fromUtf8("Ukończono pomiary."));
            return;
        }
        ui->status->setText(QString("Pozycja %1 mm %2 mm ustawiona. Średni pomiar %3").arg(m_lista.at(actPos).x).arg(m_lista.at(actPos).y).arg(avg1));
        ui->table->item(actPos, col_status)->setText(QString::fromUtf8("Trwa pomiar. Zostało %1 s").arg(actCzas-cnt1));
        DEBUGMPZ(QString("Pomiar [%1 s]").arg(actCzas));
        readRadio();
        return;
    }
    case END_MEASURING:
    {
        DEBUGMPZ("actWork END_MEASURING");
        actWork = NEXT_POS;
        return;
    }
    }
}

void MierzonePozycje::positionDone(bool home)
{
    DEBUGMPZ(QString("position Done home=%1").arg(home));
    if (home) {
        /*
        if (actWork == WAIT_HPOS) {
            actWork = NEXT_POS_AFTER_HPOS;
        } else if (actWork == WAIT){
            actWork = FIRST_RUN;
        }
        */
    } else {
        if (actWork == WAIT_POS) {
            actWork = START_MEASURING;
        }
    }
}

void MierzonePozycje::readedFromRadio(const double &val)
{
    DEBUGMPZ(QString("Read from radio %1").arg(val));
    cntErr = 0;
    setValue1(val, "m/s");

}

void MierzonePozycje::errorReadFromRadio()
{
    DEBUGMPZ(QString("Read Error %1").arg(cntErr + 1));
    if (++cntErr > 10) {
        timer->stop();
        actWork = DONE;
        ui->status->setText(QString("Przerywam pomiary ze względu na błąd odczytu z czujnika radiowego"));
    } else {
        readRadio();
    }
}

void MierzonePozycje::setStop()
{
    timer->stop();
    actWork = DONE;
    ui->pbRestart->setEnabled(true);
    ui->pbNoweDane->setEnabled(true);
    ui->pbStart->setEnabled(true);
    if (m_listawynikowa.size() > 0)
        ui->pbZapisz->setEnabled(true);
}

void MierzonePozycje::setStart()
{
    timer->start();
}

void MierzonePozycje::setError()
{
    setStop();
}

void MierzonePozycje::setStatus(const QString &st)
{
    ui->status->setText(st);
}

const WyborMetodyData &MierzonePozycje::getAllValues() const
{
    return allValues;
}

void MierzonePozycje::setValue1(const float &val1, const QString &unit1)
{
    if (actWork != NOW_MEASURING)
        return;
    DaneWynikowe data;
    data.x = m_lista[actPos].x;
    data.y = m_lista[actPos].y;
    data.time = cnt1+1;
    data.val1 = val1;
    avg1 = (cnt1*avg1 + val1)/(cnt1 + 1);
    cnt1++;
    m_listawynikowa.append(data);
    ui->table->item(actPos, col_meas)->setText(QString::fromUtf8("%1 %2").arg(avg1, 0, 'g', 3).arg(unit1));
}

void MierzonePozycje::restart()
{
    on_pbRestart_clicked();
}

void MierzonePozycje::on_pbStart_clicked()
{
    m_listawynikowa.clear();
    DEBUGMPZ(QString("Start work. "));
    actWork = WAIT_FOR_READY;
    actPos = 0;
    ui->pbStart->setEnabled(false);
    ui->pbRestart->setEnabled(false);
    ui->pbNoweDane->setEnabled(false);
    ui->pbZapisz->setEnabled(false);
    connectToDevice();
}

void MierzonePozycje::on_pbNoweDane_clicked()
{
    if (miernikPrzeplywu)
        TabWidget::miernikPrzeplywu->noweDane();
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

void MierzonePozycje::on_pbRestart_clicked()
{
    DEBUGMPZ(QString("ReStart work. "));
    for (int i = 0; i < ui->table->rowCount(); i++) {
         ui->table->item(i, col_meas)->setText("-");
         ui->table->item(i, col_status)->setText("Oczekiwanie");
    }

    m_listawynikowa.clear();

    actWork = WAIT_FOR_READY;
    actPos = 0;

    ui->pbStart->setEnabled(false);
    ui->pbNoweDane->setEnabled(false);
    ui->pbZapisz->setEnabled(false);
    ui->pbRestart->setEnabled(false);
    connectToDevice();
}

