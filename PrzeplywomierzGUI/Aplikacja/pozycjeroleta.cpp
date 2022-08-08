#include "pozycjeroleta.h"
#include "miernikprzeplywu.h"
#include "ui_pozycjeroleta.h"
#include "tabwidget.h"

#include <QTimer>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

PozycjeRoleta::PozycjeRoleta(QWidget *parent) :
    TabWidget(parent),
    ui(new Ui::PozycjeRoleta)
{
    ui->setupUi(this);
    QStringList headers;
    headers << QString::fromUtf8("Pozycja X\n[mm]") << QString::fromUtf8("Pozycja Y\n[mm]")
            << QString::fromUtf8("Pozycja X\nwg normy") << QString::fromUtf8("Pozycja Y\nwg normy")
            << QString::fromUtf8("Numer etapu")
            << QString::fromUtf8("Wysokość\nrolety [mm]")
            << QString::fromUtf8("Czas\nstabilizacji")
            << QString::fromUtf8("Czas pomiaru\n[s]");

    headers << QString::fromUtf8("Średnia wartość\nczujnik");
    headers << QString::fromUtf8("Akcja");
    headers << QString::fromUtf8("Status");

    ui->table->setColumnCount(headers.size());
    ui->table->setHorizontalHeaderLabels(headers);
    ui->table->setAutoScroll(true);
    ui->table->resizeRowsToContents();
    ui->table->resizeColumnsToContents();
    ui->table->horizontalHeader()->setStretchLastSection(true);
    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    actWork = WAIT_FOR_CONN;
    actPos = 0;

    debug("timer Start");
    ui->pbNoweDane->setEnabled(false);
    ui->pbZapisz->setEnabled(false);

    m_width = 860;
    m_height = 1600;

    m_offsetX = 0;
    m_offsetY = 0;

    avg1 = 0.0;
    cnt1 = 0;
    cntErr = 0;
    ui->pbRestart->setVisible(false);
    adjustSize();
}

PozycjeRoleta::~PozycjeRoleta()
{
    delete ui;
    timer->stop();
}

void PozycjeRoleta::setList(const PozycjeRol &l)
{
    ui->table->setRowCount(0);
    //ui->table->setRowCount(l.size());
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
    createRow(row, QString("-"), QString("-"), QString("-"), QString("-"),
              QString::number(nrR), QString::number(sizeR), QString::number(timeStab),
              QString("-"), QString("-"),
              QString("R"));
    return row + 1;
}

unsigned int PozycjeRoleta::createPoint(unsigned int row, unsigned int mmX, unsigned int mmY, float nX, float nY,
                                        unsigned int pomiar, unsigned int nrR, unsigned int sizeR)
{
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

   item = new QTableWidgetItem(QString("Oczekiwanie"));
   item->setFlags(item->flags() ^ Qt::ItemIsEditable);
   item->setFlags(item->flags() ^ Qt::ItemIsSelectable);
   ui->table->setItem(row, 10, item);
}

void PozycjeRoleta::setPos()
{
    ui->table->item(actPos, col_status)->setText(QString::fromUtf8("Ustawiam pozycję"));
    uint32_t impx, impy;
    impx = mech.getImpulsyX(xmm);
    impy = mech.getImpulsyY(ymm);

    debug(QString("impulsy (%1,%2) => (%3,%4)").arg(xmm).arg(ymm).arg(impx).arg(impy));
    actWork = WAIT_POS;
    ui->status->setText(QString("Rozpoczynam ustawianie pozycji %1 mm %2 mm").arg(xmm).arg(ymm));
    setPosition(impx, impy);
    avg1 = 0.0;
    cnt1 = 0;
}

void PozycjeRoleta::update()
{
    //debug(QString("Update %1").arg(actWork));
    switch(actWork) {
    case WAIT:
    case WAIT_POS:
    case WAIT_HPOS:
    case ROLETA_HOMEWAIT:
    case ROLETAWAIT:
    default:
        debug("actWork WAIT...");
        return;

    case WAIT_FOR_CONN:
    {
        debug("actWork WAIT_FOR_CONN");
        if (getConnect() && getIsReady()) {
            actWork = ROLETA_HOME;
            ui->status->setText("Oczekiwanie na zamknięcie rolety");
        }
        return;
    }
    case DONE:
    {
        debug("actWork DONE");
        timer->stop();
        ui->pbStart->setEnabled(true);
        ui->pbRestart->setEnabled(true);
        ui->pbZapisz->setEnabled(true);
        return;
    }
    case ROLETA_HOME:
    {
        actWork = ROLETA_HOMEWAIT;
        setRoletaHome();
        return;
    }
    case FIRST_RUN:
    {
        debug("actWork FIRST_RUN");
        ui->pbNoweDane->setEnabled(false);
        ui->pbZapisz->setEnabled(false);
        actPos = 0;
        debug(QString("Row count %1").arg(ui->table->rowCount()));
        if (ui->table->rowCount() == 0) {
            actWork = DONE;
            timer->stop();
            ui->pbNoweDane->setEnabled(true);
            return;
        }

        avg1 = 0;
        cnt1 = 0;
        debug(QString("is row R = %1").arg(ui->table->item(actPos, col_action)->text()));
        if (ui->table->item(actPos, col_action)->text() != QString("R")) {
            actWork = DONE;
            timer->stop();
            ui->pbNoweDane->setEnabled(true);
            return;
        }
        stableTime = ui->table->item(actPos, col_stableTime)->text().toUInt();
        unsigned int rmm = ui->table->item(actPos, col_rmm)->text().toUInt();
        actWork = ROLETAWAIT;
        ui->status->setText("Ustawiam wysokość rolety");
        setRoleta(rmm);

        return;
    }
    case NEXT_POS:
    {
        debug("actWork NEXT_POS");
        debug(QString("Szukam kolejnej pozycji"));
        ui->status->setText("Przechodzę do kolejnej pozycji z listy");
        ++actPos;

        if (actPos >= ui->table->rowCount()) {
            actWork = DONE;
            timer->stop();
            ui->pbNoweDane->setEnabled(true);
            ui->pbZapisz->setEnabled(true);
            ui->pbRestart->setEnabled(true);
            ui->status->setText("Zakonczono pomiar dla wszystkich pozycji z listy");
            setPositionHome();
            return;
        }
        ui->table->scrollToItem(ui->table->item(actPos, 0));
/*
        if (actPos % 100 == 0) {
            actWork = WAIT_HPOS;
            debug("Set Home pos");
            setPositionHome();
            ui->status->setText("Trwa kalibracja urządzenia");
            --actPos;
            return;
        }
*/
        if (ui->table->item(actPos, col_action)->text() == QString("R")) {
            stableTime = ui->table->item(actPos, col_stableTime)->text().toUInt();
            unsigned int rmm = ui->table->item(actPos, col_rmm)->text().toUInt();
            actWork = ROLETAWAIT;
            ui->status->setText("Ustawiam wysokość rolety");
            setRoleta(rmm);
        } else if (ui->table->item(actPos, col_action)->text() == QString("P")) {
            ui->status->setText("Ustawiam pozycje czujnika");
            xmm = ui->table->item(actPos, col_xmm)->text().toUInt();
            ymm = ui->table->item(actPos, col_ymm)->text().toUInt();
            actCzas = ui->table->item(actPos, col_measTime)->text().toUInt();
            actWork = WAIT_POS;
            setPos();
        }
        return;
    }
    case NEXT_POS_AFTER_HPOS:
    {
        debug("actWork NEXT_POS_AFTER_HPOS");
        actWork = WAIT_POS;
        //actCzas = m_lista.at(actPos).time;
        setPos();
        return;
    }
    case START_MEASURING:
    {
        debug("actWork START_MEASURING");
        avg1 = 0.0;
        cnt1 = 0;
        ui->status->setText(QString("Pozycja %1 mm %2 mm ustawiona. Rozpoczynam pomiary").arg(xmm).arg(ymm));
        actWork = NOW_MEASURING;
        return;
    }
    case NOW_MEASURING:
    {
        debug("actWork NOW_MEASURING");
        if (actCzas == cnt1) {
            actWork = END_MEASURING;
            ui->table->item(actPos, col_status)->setText(QString::fromUtf8("Ukończono pomiary."));
            return;
        }
        ui->status->setText(QString("Pozycja %1 mm %2 mm ustawiona. Średni pomiar %3").arg(xmm).arg(ymm).arg(avg1));
        ui->table->item(actPos, col_status)->setText(QString::fromUtf8("Trwa pomiar. Zostało %1 s").arg(actCzas-cnt1));
        debug(QString("Pomiar [%1 s]").arg(actCzas));
        readRadio();
        return;
    }
    case END_MEASURING:
    {
        debug("actWork END_MEASURING");
        actWork = NEXT_POS;
        return;
    }
    case WAIT2STABLE:
        debug("actWork WAIT2STABLE");
        ui->status->setText("Czekam na stabilizacje.");
        if (stableTime == 0) {
            actWork = NEXT_POS;
            ui->table->item(actPos, col_status)->setText("Oczekiwanie na stabilazacje zakończone");
        } else {
            ui->table->item(actPos, col_status)->setText(QString("Czekam na stabilizację, zostało %1 s").arg(stableTime));
            --stableTime;
        }
        return;
    }
}

void PozycjeRoleta::debug(const QString &val)
{
    qDebug("%s",val.toStdString().c_str());
}


unsigned int PozycjeRoleta::offsetY() const
{
    return m_offsetY;
}

void PozycjeRoleta::setOffsetY(unsigned int newOffsetY)
{
    m_offsetY = newOffsetY;
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
}

unsigned int PozycjeRoleta::offsetX() const
{
    return m_offsetX;
}

void PozycjeRoleta::setOffsetX(unsigned int newOffsetX)
{
    m_offsetX = newOffsetX;
}

void PozycjeRoleta::status(const QString &st)
{
    ui->status->setText(st);
}

void PozycjeRoleta::positionDone(bool home)
{
    debug(QString("position Done home=%1").arg(home));
    if (home) {
        if (actWork == WAIT_HPOS) {
            actWork = NEXT_POS_AFTER_HPOS;
        } else if (actWork == WAIT){
            actWork = FIRST_RUN;
        }
    } else {
        if (actWork == WAIT_POS) {
            actWork = START_MEASURING;
        }
    }
}

void PozycjeRoleta::roletaDone(bool home)
{
    if (home) {
        actWork = FIRST_RUN;
    } else {
        actWork = WAIT2STABLE;
    }
}

void PozycjeRoleta::readedFromRadio(const double &val)
{
    debug(QString("Read from radio %1").arg(val));
    cntErr = 0;
    setValue1(val, "m/s");
}

void PozycjeRoleta::errorReadFromRadio()
{
    debug(QString("Read Error %1").arg(cntErr + 1));
    if (++cntErr > 3) {
        timer->stop();
        actWork = DONE;
        ui->status->setText(QString("Przerywam pomiary ze względu na błąd odczytu z czujnika radiowego"));
    } else {
        readRadio();
    }
}

void PozycjeRoleta::setStop()
{
    timer->stop();
    actWork = DONE;
    ui->pbRestart->setEnabled(true);
    ui->pbNoweDane->setEnabled(true);
    if (m_listawynikowa.size() > 0)
        ui->pbZapisz->setEnabled(true);
}

void PozycjeRoleta::on_pbStart_clicked()
{
    m_listawynikowa.clear();
    debug(QString("Start work. Isconnected %1").arg(getConnect()));
    setIsReady(false);
    actWork = WAIT_FOR_CONN;
    actPos = 0;
    timer->start();
    ui->pbStart->setEnabled(false);
    ui->pbRestart->setEnabled(false);
    ui->pbNoweDane->setEnabled(false);
    ui->pbZapisz->setEnabled(false);
    connectToDevice();
}

void PozycjeRoleta::on_pbNoweDane_clicked()
{
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
    for (int i = 0; i < ui->table->rowCount(); i++) {
         ui->table->item(i, col_measValue)->setText("-");
         ui->table->item(i, col_status)->setText("Oczekiwanie");
    }
    ui->pbRestart->setEnabled(false);
    ui->pbNoweDane->setEnabled(false);
    ui->pbZapisz->setEnabled(false);
}

