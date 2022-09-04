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
    actWork = WAIT_FOR_READY;
    actPos = 0;

    //ui->pbRestart->setEnabled(false);
    ui->pbRestart->setVisible(false);
    ui->pbZapisz->setEnabled(false);
    started = false;

    ui->table->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
}

MierzonePozycje::~MierzonePozycje()
{
    delete ui;
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
    updateWork();
    ui->pbNoweDane->setEnabled(false);
    ui->pbZapisz->setEnabled(false);
    ui->pbStart->setEnabled(true);
    adjustSize();
}

void MierzonePozycje::setPos()
{
    unsigned int xmm = m_lista.at(actPos).x;
    unsigned int ymm = m_lista.at(actPos).y;

    ui->table->item(actPos, col_status)->setText(QString("Rozpoczynam ustawianie pozycji %1 mm %2 mm").arg(xmm).arg(ymm));
    uint32_t impx, impy;
    impx = mech.getImpulsyX(xmm);
    impy = mech.getImpulsyY(ymm);

    DEBUGMPZ(QString("impulsy (%1,%2) => (%3,%4)").arg(xmm).arg(ymm).arg(impx).arg(impy));
    actWork = WAIT_POS;
    ui->status->setText(QString("Rozpoczynam ustawianie pozycji %1 mm %2 mm").arg(xmm).arg(ymm));
    setPosition(impx, impy);
    avg1 = 0.0;
    cnt1 = 0;
    updateWork();
}

void MierzonePozycje::updateWork()
{
    //DEBUGMPZ(QString("Update %1").arg(actWork));
    if (!getIsReady())
        return;

    switch(actWork) {
    case WAIT:
        DEBUGMPZ("actWork WAIT...");
        return;
    case WAIT_POS:
        DEBUGMPZ("actWork WAIT_POS...");
        return;
    case WAIT_HPOS:
        DEBUGMPZ("actWork WAIT_HPOS...");
        return;
    case WAIT_FOR_READY:
        DEBUGMPZ("actWork WAIT_FOR_READY...");
        return;
    default:
        DEBUGMPZ("actWork WAIT...");
        return;

    case READY_FOR_WORK:
    {
        DEBUGMPZ("actWork READY_FOR_WORK");
        ui->status->setText("Rozpoczynam prace");
        actWork = FIRST_RUN;
        updateWork();
        return;
    }

    case DONE:
    {
        DEBUGMPZ("actWork DONE");
        ui->pbNoweDane->setEnabled(true);
        ui->pbZapisz->setEnabled(true);
        ui->pbRestart->setEnabled(true);
        ui->pbNoweDane->setEnabled(true);
        ui->pbStart->setEnabled(true);
        if (m_listawynikowa.size() > 0)
            ui->pbZapisz->setEnabled(true);
        return;
    }

    case END_WORK:
    {
        DEBUGMPZ("actWork END_WORK");
        ui->status->setText("Zakonczono pomiar dla wszystkich pozycji z listy");
        actWork = DONE;
        setClose(true);
        updateWork();
        return;
    }

    case FIRST_RUN:
    {
        DEBUGMPZ("actWork FIRST_RUN");
        actPos = 0;
        if (m_lista.size() == 0) {
            actWork = DONE;
            updateWork();
            return;
        }
        actWork = WAIT_POS;
        avg1 = 0;
        cnt1 = 0;
        actCzas = m_lista.at(actPos).time;
        setPos();
        updateWork();
        return;
    }
    case NEXT_POS:
    {
        DEBUGMPZ("actWork NEXT_POS");
        ui->status->setText("Przechodzę do kolejnej pozycji z listy");
        ui->table->item(actPos, col_status)->setText(QString::fromUtf8("Ukończono"));
        ++actPos;


        if (actPos >= m_lista.size()) {
            actWork = END_WORK;
            ui->status->setText("Zakonczono pomiar dla wszystkich pozycji z listy");
            setPositionHome();
            updateWork();
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
        updateWork();
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
        ui->status->setText(QString("Pozycja %1 mm %2 mm ustawiona. Rozpoczynam pomiary (%3)").arg(m_lista.at(actPos).x).arg(m_lista.at(actPos).y).arg(actCzas));
        actWork = NOW_MEASURING;
        updateWork();
        return;
    }
    case NOW_MEASURING:
    {
        DEBUGMPZ("actWork NOW_MEASURING");
        if (actCzas <= cnt1) {
            actWork = END_MEASURING;
            ui->table->item(actPos, col_status)->setText(QString::fromUtf8("Ukończono pomiary."));
            updateWork();
            return;
        }
        ui->status->setText(QString("Pozycja %1 mm %2 mm ustawiona. Średni pomiar %3").arg(m_lista.at(actPos).x).arg(m_lista.at(actPos).y).arg(avg1));
        ui->table->item(actPos, col_status)->setText(QString::fromUtf8("Trwa pomiar. Zostało %1 pomiarów").arg(actCzas-cnt1));
        DEBUGMPZ(QString("Pomiar [%1]").arg(actCzas-cnt1));
        radioTimer.start();
        readRadio();
        return;
    }
    case END_MEASURING:
    {
        DEBUGMPZ("actWork END_MEASURING");
        setRadioVal("--");
        actWork = NEXT_POS;
        updateWork();
        return;
    }
    }
}

void MierzonePozycje::positionDone(bool home)
{
    DEBUGMPZ(QString("position Done home=%1").arg(home));
    if (home) {
        if (actWork == WAIT_HPOS) {
            actWork = NEXT_POS_AFTER_HPOS;
            updateWork();
        }
    } else {
        if (actWork == WAIT_POS) {
            actWork = START_MEASURING;
            updateWork();
        }
        
    }
}

void MierzonePozycje::readedFromRadio(const double &val)
{
    DEBUGMPZ(QString("Read from radio %1").arg(val));
    cntErr = 0;
    setValue1(val, "m/s");
    unsigned int msR = radioTimer.elapsed();
    DEBUGMPZ(QString("Kolejny odczyt za %1").arg(msR));
    if (msR > 1000) {
        updateWork();
    }
    else {
        timer->singleShot(1000 - msR, this, &MierzonePozycje::updateWork);
    }
}

void MierzonePozycje::errorReadFromRadio()
{
    DEBUGMPZ(QString("Read Error %1").arg(cntErr + 1));
    if (++cntErr > 10) {
        actWork = DONE;
        ui->status->setText(QString("Przerywam pomiary ze względu na błąd odczytu z czujnika radiowego"));
        updateWork();
    } else {
        updateWork();
    }
}

void MierzonePozycje::setStop()
{
    actWork = DONE;
    updateWork();
}

void MierzonePozycje::setStart()
{
    DEBUGMPZ(QString("setStart"));
    actPos = 0;
    actWork = READY_FOR_WORK;
    m_listawynikowa.clear();
    updateWork();
}

void MierzonePozycje::setError()
{
    setStop();
}

void MierzonePozycje::setStatus(const QString &st)
{
    ui->status->setText(st);
}

void MierzonePozycje::noweDane()
{
    m_listawynikowa.clear();
    actWork = WAIT_FOR_READY;
    actPos = 0;
    updateWork();
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
    setRadioVal(QString::fromUtf8("%1").arg(val1, 0, 'g', 3));
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
    updateWork();
    ui->pbStart->setEnabled(false);
    ui->pbRestart->setEnabled(false);
    ui->pbNoweDane->setEnabled(false);
    ui->pbZapisz->setEnabled(false);
    connectToDevice();
}

void MierzonePozycje::on_pbNoweDane_clicked()
{
    m_listawynikowa.clear();
    actWork = WAIT_FOR_READY;
    actPos = 0;
    updateWork();
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
    updateWork();
    ui->pbStart->setEnabled(false);
    ui->pbNoweDane->setEnabled(false);
    ui->pbZapisz->setEnabled(false);
    ui->pbRestart->setEnabled(false);
    connectToDevice();
}

