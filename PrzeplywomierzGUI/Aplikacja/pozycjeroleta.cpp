#include "pozycjeroleta.h"
#include "miernikprzeplywu.h"
#include "ui_pozycjeroleta.h"
#include "tabwidget.h"

#include <QTimer>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

#define DEBUGPR(X) debug(QString("%1:%2 %3").arg(__FILE__).arg(__LINE__).arg(X))

PozycjeRoleta::PozycjeRoleta(QWidget *parent) :
    TabWidget(parent),
    ui(new Ui::PozycjeRoleta)
{
    ui->setupUi(this);
    QStringList headers;
    headers << QString::fromUtf8("Pozycja X\n[mm]") << QString::fromUtf8("Pozycja Y\n[mm]")
            << QString::fromUtf8("Pozycja X\nwg normy") << QString::fromUtf8("Pozycja Y\nwg normy")
            << QString::fromUtf8("Numer\netapu")
            << QString::fromUtf8("Wysokość\nrolety [mm]")
            << QString::fromUtf8("Czas [s]\nstabilizacji")
            << QString::fromUtf8("Czas [s]\npomiaru");

    headers << QString::fromUtf8("Średnia wartość\nczujnika");
    headers << QString::fromUtf8("Akcja");
    headers << QString::fromUtf8("Status");

    ui->table->setColumnCount(headers.size());
    ui->table->setHorizontalHeaderLabels(headers);
    //ui->table->setAutoScroll(true);
    ui->table->resizeRowsToContents();
    ui->table->resizeColumnsToContents();
    ui->table->horizontalHeader()->setStretchLastSection(true);
    timer = new QTimer(this);
    //timer->setInterval(1000);
    //connect(timer, SIGNAL(timeout()), this, SLOT(updateWork()));
    actWork = WAIT_FOR_READY;
    actPos = 0;

    ui->pbZapisz->setEnabled(false);

    avg1 = 0.0;
    cnt1 = 0;
    cntErr = 0;
    ui->pbRestart->setVisible(false);
    adjustSize();
    ui->table->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    roletaClose = false;
    firstRoleta = false;
}

PozycjeRoleta::~PozycjeRoleta()
{
    delete ui;
    timer->stop();
}

void PozycjeRoleta::setList(const PozycjeRol &l)
{
    ui->table->setRowCount(0);
    for (int r = 0; r < l.size(); r++) {
        PosRoleta p = l.at(r);
        if (p.measPoint) {
            createPoint(r, p.mmx, p.mmy, p.nx, p.ny, p.measTime, p.etap, p.mmr);
        } else {
            createRoletaRow(r, p.etap, p.mmr, p.stableTime);
        }
    }
    adjustSize();
}

unsigned int PozycjeRoleta::createRoletaRow(unsigned int row, unsigned int nrR, unsigned int sizeR,
                                            unsigned int timeStab)
{
    PunktyPomiariowe p;
    p.roleta = true;
    p.stableRolTime = timeStab;
    p.rmm = sizeR;
    p.nrEtap = nrR;
    p.measurTime = 0;
    p.xmm = 0;
    p.x = 0;
    p.ymm = 0;
    p.y = 0;
    punkty.append(p);
    
    createRow(row, QString("-"), QString("-"), QString("-"), QString("-"),
              QString::number(nrR), QString::number(sizeR), QString::number(timeStab),
              QString("-"), QString("-"),
              QString("R"));
    return row + 1;
}

unsigned int PozycjeRoleta::createPoint(unsigned int row, unsigned int mmX, unsigned int mmY, float nX, float nY,
                                        unsigned int pomiar, unsigned int nrR, unsigned int sizeR)
{
    PunktyPomiariowe p;
    p.roleta = false;
    p.stableRolTime = 0;
    p.rmm = 0;
    p.nrEtap = nrR;
    p.measurTime = pomiar;
    p.xmm = mmX;
    p.x = nX;
    p.ymm = mmY;
    p.y = nY;
    punkty.append(p);

    createRow(row, QString::number(mmX), QString::number(mmY), QString::number(nX), QString::number(nY),
              QString::number(nrR), QString::number(sizeR), QString("-"),
              QString::number(pomiar), QString("?"),
              QString("P"));
    return row + 1;
}

void PozycjeRoleta::createRow(int row, const QString &c1, const QString &c2, const QString &c3,
                              const QString &c4, const QString &c5, const QString &c6, const QString &c7,
                              const QString &c8, const QString &c9, const QString &c10)
{
    ui->table->setRowCount(row+1);
    QTableWidgetItem *item;
    item = new QTableWidgetItem(c1);
    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
    item->setFlags(item->flags() ^ Qt::ItemIsSelectable);
    ui->table->setItem(row, 0, item);

    item = new QTableWidgetItem(c2);
    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
    item->setFlags(item->flags() ^ Qt::ItemIsSelectable);
    ui->table->setItem(row, 1, item);

    item = new QTableWidgetItem(c3);
    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
    item->setFlags(item->flags() ^ Qt::ItemIsSelectable);
    ui->table->setItem(row, 2, item);

    item = new QTableWidgetItem(c4);
    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
    item->setFlags(item->flags() ^ Qt::ItemIsSelectable);
    ui->table->setItem(row, 3, item);

    item = new QTableWidgetItem(c5);
    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
    item->setFlags(item->flags() ^ Qt::ItemIsSelectable);
    ui->table->setItem(row, 4, item);

   item = new QTableWidgetItem(c6);
   item->setFlags(item->flags() ^ Qt::ItemIsEditable);
   item->setFlags(item->flags() ^ Qt::ItemIsSelectable);
   ui->table->setItem(row, 5, item);

   item = new QTableWidgetItem(c7);
   item->setFlags(item->flags() ^ Qt::ItemIsEditable);
   item->setFlags(item->flags() ^ Qt::ItemIsSelectable);
   ui->table->setItem(row, 6, item);

   item = new QTableWidgetItem(c8);
   item->setFlags(item->flags() ^ Qt::ItemIsEditable);
   item->setFlags(item->flags() ^ Qt::ItemIsSelectable);
   ui->table->setItem(row, 7, item);

   item = new QTableWidgetItem(c9);
   item->setFlags(item->flags() ^ Qt::ItemIsEditable);
   item->setFlags(item->flags() ^ Qt::ItemIsSelectable);
   ui->table->setItem(row, 8, item);

   item = new QTableWidgetItem(c10);
   item->setFlags(item->flags() ^ Qt::ItemIsEditable);
   item->setFlags(item->flags() ^ Qt::ItemIsSelectable);
   ui->table->setItem(row, 9, item);

   item = new QTableWidgetItem(QString::fromUtf8("Oczekiwanie"));
   item->setFlags(item->flags() ^ Qt::ItemIsEditable);
   item->setFlags(item->flags() ^ Qt::ItemIsSelectable);
   ui->table->setItem(row, 10, item);
}

void PozycjeRoleta::setPos()
{
    uint32_t impx, impy;
    impx = mech.getImpulsyX(xmm);
    impy = mech.getImpulsyY(ymm);
    setWorkStatus(actPos, QString::fromUtf8("Rozpoczynam ustawianie pozycji [ %1 ; %2 ] mm").arg(xmm).arg(ymm));

    DEBUGPR(QString::fromUtf8("Ustawianie pozycji (%1,%2) => imp(%3,%4)").arg(xmm).arg(ymm).arg(impx).arg(impy));
    actWork = WAIT_POS;
    setPosition(impx, impy);
    avg1 = 0.0;
    cnt1 = 0;
    updateWork();
}

void PozycjeRoleta::updateWork()
{
    //DEBUGPR(QString("updateWork %1").arg(actWork));
    if (!getIsReady())
        return;

    switch (actWork) {
    case WAIT:
        DEBUGPR("actWork WAIT...");
        return;
    case WAIT_POS:
        DEBUGPR("actWork WAIT_POS...");
        return;
    case WAIT_HPOS:
        DEBUGPR("actWork WAIT_HPOS...");
        return;
    case WAIT_ROLETA_HOME:
        DEBUGPR("actWork WAIT_ROLETA_HOME...");
        return;
    case WAIT_ROLETA_POS:
        DEBUGPR("actWork WAIT_ROLETA_POS...");
        return;
    case WAIT_FOR_READY:
        DEBUGPR("actWork WAIT_FOR_READY...");
        return;
    default:
        DEBUGPR("actWork WAIT...");
        return;

    case READY_FOR_WORK:
    {
        DEBUGPR("actWork READY_FOR_WORK");
        setStatus("Rozpoczynam prace");
        actWork = FIRST_RUN;
        firstRoleta = true;
        updateWork();
        return;
    }

    case WAIT_ROLETA_STABLE:
    {
        DEBUGPR(QString("actWork WAIT_ROLETA_STABLE %1").arg(firstRoleta));

        if (firstRoleta) {
            firstRoleta = false;
            setStatus("Czekam na uruchomienie wentylatorów.");
            QMessageBox::information(this, "Stanowisko z roletą", "Roleta została otwarta - po uruchomieniu wentylatorów, można kliknąć dalej aby kontynuować pomiary");
        }
        if (stableTime == 0 || stableTime > 3600) {
            actWork = NEXT_POS;
            setWorkStatus(actPos, "Stabilazacja zakończona");
            updateWork();
        }
        else {
            setWorkStatus(actPos, QString::fromUtf8("Czekam na stabilizację, zostało %1 s").arg(stableTime));
            --stableTime;
            timer->singleShot(1000, this, &PozycjeRoleta::updateWork);
        }
        return;
    }

    case FIRST_RUN:
    {
        DEBUGPR("actWork FIRST_RUN");
        actPos = 0;
        DEBUGPR(QString("Row count %1").arg(ui->table->rowCount()));
        if (ui->table->rowCount() == 0 || punkty.size() == 0) {
            actWork = DONE;
            setStatus("Brak danych");
            updateWork();
            return;
        }

        avg1 = 0;
        cnt1 = 0;
        DEBUGPR(QString("is row R = %1").arg(ui->table->item(actPos, col_action)->text()));
        if (ui->table->item(actPos, col_action)->text() != QString("R") ||
            !punkty[actPos].roleta)
        {
            actWork = DONE;
            setStatus("Nieprawidłowe dane");
            updateWork();
            return;
        }

        if (actPos >= punkty.size()) {
            actWork = DONE;
            setStatus("Nieprawidłowe dane. Brak danych ?");
            updateWork();
            return;
        }


        stableTime = punkty[actPos].stableRolTime;
        if (stableTime > 3600)
            stableTime = 3600;

        unsigned int rmm = punkty[actPos].rmm;
        actWork = WAIT_ROLETA_POS;
        setWorkStatus(actPos, QString::fromUtf8("Ustawiam wysokość rolety na %1 mm").arg(rmm));
        DEBUGPR(QString::fromUtf8("Ustawiam wysokośc rolety na %1 mm").arg(rmm));
        setRoleta(mechR.podniescMM(rmm));
        updateWork();
        return;
    }
    case NEXT_POS:
    {
        DEBUGPR("actWork NEXT_POS");
        ++actPos;

        if (actPos >= ui->table->rowCount() || actPos >= punkty.size()) {
            actWork = END_WORK;
            setStatus("Zakonczono pomiar dla wszystkich pozycji z listy");
            setPositionHome();
            updateWork();
            return;
        }
        ui->table->scrollToItem(ui->table->item(actPos, 0), QAbstractItemView::PositionAtCenter);
        /*
                if (actPos % 100 == 0) {
                    actWork = WAIT_HPOS;
                    DEBUGPR("Set Home pos");
                    setPositionHome();
                    ui->status->setText("Trwa kalibracja urządzenia");
                    --actPos;
                    return;
                }
        */
        if (ui->table->item(actPos, col_action)->text() == QString("R") || punkty[actPos].roleta) {
            stableTime = punkty[actPos].stableRolTime;
            if (stableTime > 3600)
                stableTime = 3600;
            unsigned int rmm = punkty[actPos].rmm;
            setWorkStatus(actPos, QString::fromUtf8("Ustawiam wysokość rolety na %1 mm").arg(rmm));
            actWork = WAIT_ROLETA_POS;
            DEBUGPR(QString::fromUtf8("Ustawiam wysokośc rolety na %1 mm").arg(rmm));
            setRoleta(mechR.podniescMM(rmm));
            updateWork();
            return;
        }
        else if (ui->table->item(actPos, col_action)->text() == QString("P") || !punkty[actPos].roleta) {
            xmm = punkty[actPos].xmm;
            ymm = punkty[actPos].ymm;
            measTime = punkty[actPos].measurTime;
            DEBUGPR(QString::fromUtf8("Ustawiam pozycje [ %1 , %2 ] mm. Ilość próbek %3").arg(xmm).arg(ymm).arg(measTime));
            actWork = WAIT_POS;
            setPos();
            updateWork();
        }
        return;
    }
    /*
        case NEXT_POS_AFTER_HPOS:
        {
            DEBUGPR("actWork NEXT_POS_AFTER_HPOS");
            actWork = WAIT_POS;
            //measTime = m_lista.at(actPos).time;
            setPos();
            return;
        }
    */
    case START_MEASURING:
    {
        DEBUGPR("actWork START_MEASURING");
        avg1 = 0.0;
        cnt1 = 0;
        setWorkStatus(actPos, QString::fromUtf8("Rozpoczynam pomiary. Zostało %1").arg(measTime));
        actWork = NOW_MEASURING;
        updateWork();
        return;
    }
    case NOW_MEASURING:
    {
        DEBUGPR("actWork NOW_MEASURING");
        if (measTime <= cnt1) {
            actWork = END_MEASURING;
            setWorkStatus(actPos, QString::fromUtf8("Ukończono pomiary."));
            updateWork();
            return;
        }
        setStatus(QString::fromUtf8("[%1] Pozycja %2 mm %3 mm ustawiona. Średni pomiar %4 m/s. Zostało %5 pomiarów").arg(actPos+1).arg(xmm).arg(ymm).arg(avg1).arg(measTime - cnt1));
        ui->table->item(actPos, col_status)->setText(QString::fromUtf8("Trwa pomiar. Zostało %1 pomiarów").arg(measTime - cnt1));
        DEBUGPR(QString::fromUtf8("Pomiar [%1]").arg(measTime - cnt1));
        radioTimer.start();
        readRadio();
        return;
    }
    
    case END_MEASURING:
    {
        DEBUGPR("actWork END_MEASURING");
        setRadioVal("--");
        actWork = NEXT_POS;
        updateWork();
        return;
    }

    case END_WORK:
    {
        DEBUGPR("actWork END_WORK");
        setStatus(QString::fromUtf8("Zakończono pomiar dla wszystkich pozycji z listy"));
        actWork = DONE;
        setClose(true);
        updateWork();
        return;
    }

    case DONE:
    {
        DEBUGPR("actWork DONE");
        ui->pbRestart->setEnabled(true);
        ui->pbNoweDane->setEnabled(true);
        ui->pbStart->setEnabled(true);
        ui->pbZamknijRoleta->setEnabled(true);
        if (m_listawynikowa.size() > 0)
            ui->pbZapisz->setEnabled(true);
        return;
    }



    } //switch
}

void PozycjeRoleta::setValue1(const float &val1, const QString &unit1)
{
    if (actWork != NOW_MEASURING)
        return;
    DaneWynikoweRoleta data;
    data.x = ui->table->item(actPos, col_xmm)->text();
    data.y = ui->table->item(actPos, col_ymm)->text();
    data.nx = ui->table->item(actPos, col_xnorma)->text();
    data.ny = ui->table->item(actPos, col_ynorma)->text();
    data.nrEtap = ui->table->item(actPos, col_etap)->text();
    data.time = cnt1+1;
    data.val1 = val1;
    avg1 = (cnt1*avg1 + val1)/(cnt1 + 1);
    cnt1++;
    m_listawynikowa.append(data);
    ui->table->item(actPos, col_measValue)->setText(QString::fromUtf8("%1 %2").arg(avg1, 0, 'g', 3).arg(unit1));
    setRadioVal(QString::fromUtf8("%1").arg(val1, 0, 'g', 3));
}

void PozycjeRoleta::positionDone(bool home)
{
    DEBUGPR(QString("position Done home=%1 roletaClose=%2").arg(home).arg(roletaClose));

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

void PozycjeRoleta::roletaDone(bool home)
{
    DEBUGPR(QString("rolet Done home=%1 roletaClose=%2").arg(home).arg(roletaClose));
    if (roletaClose && home) {
        roletaClose = false;
        return;
    }
    if (!home) {
        actWork = WAIT_ROLETA_STABLE;
        updateWork();
    }
}

void PozycjeRoleta::readedFromRadio(const double &val)
{
    DEBUGPR(QString("Read from radio %1").arg(val));
    cntErr = 0;
    setValue1(val, "m/s");
    unsigned int msR = radioTimer.elapsed();
    DEBUGPR(QString::fromUtf8("Odczyt trwał %1 ms").arg(msR));
    if (msR > 1000) {
        updateWork();
    } else {
        timer->singleShot(1000-msR, this, &PozycjeRoleta::updateWork);
    }
}

void PozycjeRoleta::errorReadFromRadio()
{
    DEBUGPR(QString("Read Error %1").arg(cntErr + 1));
    if (++cntErr > 1000) {
        actWork = DONE;
        setStatus(QString::fromUtf8("Przerywam pomiary ze względu na błąd odczytu z czujnika radiowego"));
        updateWork();
    } else {
        if (cntErr % 10 == 0)
            timer->singleShot(5000, this, &PozycjeRoleta::updateWork);
        else
        {
            timer->singleShot(500, this, &PozycjeRoleta::updateWork);
        }
    }
}

void PozycjeRoleta::setStop()
{
    actWork = DONE;
    updateWork();

}

void PozycjeRoleta::setStart()
{
    DEBUGPR(QString("setStart"));
    actPos = 0;
    actWork = READY_FOR_WORK;
    m_listawynikowa.clear();
    firstRoleta = true;
    updateWork();
}

void PozycjeRoleta::setError()
{
    setStop();
}

void PozycjeRoleta::setStatus(const QString &st)
{
    ui->status->setText(st);
}

void PozycjeRoleta::setWorkStatus(int actPos, const QString& st)
{
    ui->status->setText(QString("[%1] %2").arg(actPos + 1).arg(st));
    ui->table->item(actPos, col_status)->setText(st);
}

void PozycjeRoleta::on_pbStart_clicked()
{
    DEBUGPR(QString("Start work. "));
    m_listawynikowa.clear();
    actWork = WAIT_FOR_READY;
    actPos = 0;
    ui->pbStart->setEnabled(false);
    ui->pbRestart->setEnabled(false);
    ui->pbNoweDane->setEnabled(false);
    ui->pbZapisz->setEnabled(false);
    ui->pbZamknijRoleta->setEnabled(false);
    connectToDevice();
}

void PozycjeRoleta::on_pbNoweDane_clicked()
{
    DEBUGPR(QString("Nowe dane."));
    m_listawynikowa.clear();
    actWork = WAIT_FOR_READY;
    actPos = 0;
    ui->pbNoweDane->setEnabled(false);
    ui->pbRestart->setEnabled(false);
    ui->pbZapisz->setEnabled(false);
    ui->pbStart->setEnabled(false);
    if (miernikPrzeplywu)
        TabWidget::miernikPrzeplywu->noweDane();
}


void PozycjeRoleta::on_pbZapisz_clicked()
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
        DaneWynikoweRoleta d = m_listawynikowa.at(id);
        out << d.nrEtap << ";" << d.x << ";" << d.nx << ";" << d.y << ";" << d.ny << ";" << d.time << ";" << d.val1 << "\n";
    }
    file.close();
}

void PozycjeRoleta::on_pbRestart_clicked()
{
    DEBUGPR(QString("ReStart work. "));
    for (int i = 0; i < ui->table->rowCount(); i++) {
         ui->table->item(i, col_measValue)->setText("-");
         ui->table->item(i, col_status)->setText("Oczekiwanie");
    }

    m_listawynikowa.clear();

    actWork = WAIT_FOR_READY;
    actPos = 0;

    ui->pbStart->setEnabled(false);
    ui->pbNoweDane->setEnabled(false);
    ui->pbZapisz->setEnabled(false);
    ui->pbRestart->setEnabled(false);
    ui->pbZamknijRoleta->setEnabled(false);
    connectToDevice();
}

void PozycjeRoleta::on_pbZamknijRoleta_clicked()
{
    ui->pbZamknijRoleta->setEnabled(false);
    ui->pbNoweDane->setEnabled(false);
    ui->pbRestart->setEnabled(false);
    ui->pbZapisz->setEnabled(false);
    ui->pbStart->setEnabled(false);
    roletaClose = true;
    closeRoleta();
    ui->pbZamknijRoleta->setEnabled(true);
    ui->pbNoweDane->setEnabled(true);
    ui->pbRestart->setEnabled(true);
    ui->pbZapisz->setEnabled(true);
    ui->pbStart->setEnabled(true);
}
