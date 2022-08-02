#include "pozycjeroleta.h"
#include "ui_pozycjeroleta.h"

#include <QTimer>
#include <QDebug>
PozycjeRoleta::PozycjeRoleta(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PozycjeRoleta)
{
    ui->setupUi(this);
    QStringList headers;
    headers << QString::fromUtf8("Pozycja X\n[mm]") << QString::fromUtf8("Pozycja Y\n[mm]")
            << QString::fromUtf8("Pozycja X\nwg normy") << QString::fromUtf8("Pozycja Y\nwg normy")
            << QString::fromUtf8("Numer normy")
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
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    actStatus = WAIT;
    actPos = 0;
    connected = false;
    timer->start(1000);
    debug("timer Start");
    ui->pbNoweDane->setEnabled(false);
    ui->pbZapisz->setEnabled(false);

    m_width = 860;
    m_height = 1600;

    m_offsetX = 0;
    m_offsetY = 0;

    avg1 = 0.0;
    cnt1 = 0;
}

PozycjeRoleta::~PozycjeRoleta()
{
    delete ui;
    timer->stop();
}

void PozycjeRoleta::setData(unsigned short etapNr, unsigned int stableTime, unsigned int cnt)
{
    unsigned int row = 0;
    for (unsigned int e = 1; e <= etapNr; ++e) {
        unsigned int rolS = (unsigned int)m_height*e/etapNr;
        row = createRoletaRow(row, e, rolS, stableTime);
        unsigned int wysokosc = m_height*e/etapNr;
        row = setNorma5(row, wysokosc, cnt, e, rolS);
        row = setNorma6(row, wysokosc, cnt, e, rolS);
        row = setNorma7(row, wysokosc, cnt, e, rolS);
        row = setNorma8(row, wysokosc, cnt, e, rolS);
        row = setNorma9(row, wysokosc, cnt, e, rolS);
        row = setNorma10(row, wysokosc, cnt, e, rolS);
    }
}

void PozycjeRoleta::update()
{
    if (getIsStart()) {
        if (getIsWait())
            return;

        if (actStatus == WAIT)
            return;


        if (actStatus == POSITIONING) {
            if (actPos >= ui->table->rowCount()) {
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
            unsigned int xmm = ui->table->item(actPos,0)->text().toUInt();
            unsigned int ymm = ui->table->item(actPos,1)->text().toUInt();

            actCzas = ui->table->item(actPos,8)->text().toUInt();
            ui->table->item(actPos, 11)->setText(QString::fromUtf8("Ustawiam pozycję"));
            uint32_t impx, impy;
            impx = mech.getImpulsyX(xmm);
            impy = mech.getImpulsyY(ymm);

            debug(QString("impulsy (%1,%2) => (%3,%4)").arg(xmm).arg(ymm).arg(impx).arg(impy));
            actStatus = MEASURING;
            emit setPosition(impx, impy);
            ui->status->setText(QString("Rozpoczynam ustawianie pozycji %1 mm %2 mm").arg(xmm).arg(ymm));
            setIsWait( true );
            avg1 = 0.0;
            cnt1 = 0;
            qDebug("xmm=%d",impx);
            qDebug("ymm=%d",impy);
        } else if (actStatus == MEASURING) {
            ui->status->setText(QString("Pozycja %1 mm %2 mm ustawiona. Średni pomiar %3").arg(ui->table->item(actPos,0)->text()).arg(ui->table->item(actPos,1)->text()).arg(avg1));
            ui->table->item(actPos, 11)->setText(QString::fromUtf8("Trwa pomiar. Zostało %1 s").arg(actCzas));
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

            ui->table->item(actPos, 11)->setText(QString::fromUtf8("Ukończono"));
            ++actPos;
            ui->table->scrollToItem(ui->table->item(actPos, 0));
            if (actPos < ui->table->rowCount()) {
                if (ui->table->item(actPos, 10)->text() ==QString("P")) {
                    actStatus = POSITIONING;
                } else {
                    actStatus = MOVEROLETA;
                }
            } else {
                actStatus = WAIT;
            }
        } else if (actStatus == MOVEROLETA) {
            emit
        }
    }
}

void PozycjeRoleta::readedFromRadio(int val)
{
    setValue1(0.01*val, "m/s");
}

void PozycjeRoleta::debug(const QString &val)
{
    qDebug("%s",val.toStdString().c_str());
    ui->localdebug->append(val);
}

unsigned int PozycjeRoleta::setNorma5(unsigned int row, unsigned int wysokosc, unsigned int pomiary, unsigned int nrR, unsigned int rSize)
{
    float A = 0;
    float B = 0.212;
    float C = 0.426;

    float norma[5] = {-C, -B, A, B, C};

    unsigned int r = row;

    for (short i = 0; i < 5; ++i ) {
        for (short j = 0; j < 5; ++j ) {
            float px = 0.5 - norma[i];
            float py = 0.5 - norma[j];
            unsigned mmX = m_width * px + m_offsetX;
            unsigned mmY = wysokosc * py + m_offsetY;
            r = createPoint(r, mmX, mmY, norma[i], norma[j], 5, pomiary, nrR, rSize);
        }
    }
    return r;

    //[(-0.426, 0.426), (-0.426, 0.212), (-0.426, 0), (-0.426, -0.212), (-0.426, -0.426)
    //(-0.212, 0.426), (-0.212, 0.212), (-0.212, 0), (-0.212, -0.212), (-0.212 -0.426)
    //(0, 0.426), (0, 0.212), (0, 0), (0, -0.212), (0 -0.426)
    //(0.212, 0.426), (0.212, 0.212), (0.212, 0), (0.212, -0.212), (0.212 -0.426)
    //(0.426, 0.426), (0.426, 0.212), (0.426, 0), (0.426, -0.212), (0.426, -0.426)

}

unsigned int PozycjeRoleta::setNorma6(unsigned row, unsigned int wysokosc, unsigned int pomiary, unsigned int nrR, unsigned int rSize)
{
    float A = 0.063;
    float B = 0.265;
    float C = 0.439;

    float norma[6] = {-C, -B, -A, A, B, C};

    unsigned int r = row;

    for (short i = 0; i < 6; ++i ) {
        for (short j = 0; j < 6; ++j ) {
            float px = 0.5 - norma[i];
            float py = 0.5 - norma[j];
            unsigned mmX = m_width * px + m_offsetX;
            unsigned mmY = wysokosc * py + m_offsetY;
            r = createPoint(r, mmX, mmY, norma[i], norma[j], 6, pomiary, nrR, rSize);
        }
    }
    return r;
}

unsigned int PozycjeRoleta::setNorma7(unsigned row, unsigned int wysokosc, unsigned int pomiary, unsigned int nrR, unsigned int rSize)
{
    float A = 0.0;
    float B = 0.134;
    float C = 0.297;
    float D = 0.447;

    float norma[7] = {-D, -C, -B, A, B, C, D};

    unsigned int r = row;

    for (short i = 0; i < 7; ++i ) {
        for (short j = 0; j < 7; ++j ) {
            float px = 0.5 - norma[i];
            float py = 0.5 - norma[j];
            unsigned mmX = m_width * px + m_offsetX;
            unsigned mmY = wysokosc * py + m_offsetY;
            r = createPoint(r, mmX, mmY, norma[i], norma[j], 7, pomiary, nrR, rSize);
        }
    }
    return r;
}

unsigned int PozycjeRoleta::setNorma8(unsigned row, unsigned int wysokosc, unsigned int pomiary, unsigned int nrR, unsigned int rSize)
{
    float A = 0.098;
    float B = 0.278;
    float C = 0.416;
    float D = 0.490;

    float norma[8] = {-D, -C, -B, -A, A, B, C, D};

    unsigned int r = row;

    for (short i = 0; i < 8; ++i ) {
        for (short j = 0; j < 8; ++j ) {
            float px = 0.5 - norma[i];
            float py = 0.5 - norma[j];
            unsigned mmX = m_width * px + m_offsetX;
            unsigned mmY = wysokosc * py + m_offsetY;
            r = createPoint(r, mmX, mmY, norma[i], norma[j], 8, pomiary, nrR, rSize);
        }
    }
    return r;
}

unsigned int PozycjeRoleta::setNorma9(unsigned row, unsigned int wysokosc, unsigned int pomiary, unsigned int nrR, unsigned int rSize)
{
    float A = 0.0;
    float B = 0.171;
    float C = 0.321;
    float D = 0.433;
    float E = 0.492;

    float norma[9] = {-E, -D, -C, -B, A, B, C, D, E};

    unsigned int r = row;

    for (short i = 0; i < 9; ++i ) {
        for (short j = 0; j < 9; ++j ) {
            float px = 0.5 - norma[i];
            float py = 0.5 - norma[j];
            unsigned mmX = m_width * px + m_offsetX;
            unsigned mmY = wysokosc * py + m_offsetY;
            r = createPoint(r, mmX, mmY, norma[i], norma[j], 9, pomiary, nrR, rSize);
        }
    }
    return r;
}

unsigned int PozycjeRoleta::setNorma10(unsigned row, unsigned int wysokosc, unsigned int pomiary, unsigned int nrR, unsigned int rSize)
{
    float A = 0.078;
    float B = 0.227;
    float C = 0.354;
    float D = 0.446;
    float E = 0.494;

    float norma[10] = {-E, -D, -C, -B, -A, A, B, C, D, E};

    unsigned int r = row;

    for (short i = 0; i < 10; ++i ) {
        for (short j = 0; j < 10; ++j ) {
            float px = 0.5 - norma[i];
            float py = 0.5 - norma[j];
            unsigned mmX = m_width * px + m_offsetX;
            unsigned mmY = wysokosc * py + m_offsetY;
            r = createPoint(r, mmX, mmY, norma[i], norma[j], 10, pomiary, nrR, rSize);
        }
    }
    return r;
}

void PozycjeRoleta::createRow(int row, const QString &c1, const QString &c2, const QString &c3,
                              const QString &c4, const QString &c5, const QString &c6, const QString &c7,
                              const QString &c8, const QString &c9, const QString &c10, const QString &c11)
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

   item = new QTableWidgetItem(c11);
   item->setFlags(item->flags() ^ Qt::ItemIsEditable);
   item->setFlags(item->flags() ^ Qt::ItemIsSelectable);
   ui->table->setItem(row, 10, item);

   item = new QTableWidgetItem("");
   item->setFlags(item->flags() ^ Qt::ItemIsEditable);
   item->setFlags(item->flags() ^ Qt::ItemIsSelectable);
   ui->table->setItem(row, 11, item);
}

unsigned int PozycjeRoleta::createRoletaRow(unsigned int row, unsigned int nrR, unsigned int sizeR, unsigned int timeStab)
{
    createRow(row, QString("-"), QString("-"), QString("-"), QString("-"), QString("-"),
              QString::number(nrR), QString::number(sizeR), QString::number(timeStab),
              QString("-"), QString("-"),
              QString("R"));
    return row + 1;
}

unsigned int PozycjeRoleta::createPoint(unsigned int row, unsigned int mmX, unsigned int mmY, float nX, float nY, unsigned int numer, unsigned int pomiar, unsigned int nrR, unsigned int sizeR)
{
    createRow(row, QString::number(mmX), QString::number(mmY), QString::number(nX), QString::number(nY), QString::number(numer),
              QString::number(nrR), QString::number(sizeR), QString("-"),
              QString::number(pomiar), QString("?"),
              QString("P"));
    return row + 1;
}

unsigned int PozycjeRoleta::offsetY() const
{
    return m_offsetY;
}

void PozycjeRoleta::setOffsetY(unsigned int newOffsetY)
{
    m_offsetY = newOffsetY;
}

void PozycjeRoleta::setValue1(const float &val, const QString &unit)
{
    if (actStatus != MEASURING)
        return;

    avg1 = (avg1*cnt1 + val)/(cnt1 + 1);
    ++cnt1;
    ui->table->item(actPos, 10)->setText(QString::fromUtf8("%1 %2").arg(avg1, 0, 'g', 3).arg(unit));
}

bool PozycjeRoleta::getConnected() const
{
    return connected;
}

unsigned int PozycjeRoleta::offsetX() const
{
    return m_offsetX;
}

void PozycjeRoleta::setOffsetX(unsigned int newOffsetX)
{
    m_offsetX = newOffsetX;
}

void PozycjeRoleta::setConnected(bool newConnected)
{
    connected = newConnected;
}

void PozycjeRoleta::status(const QString &st)
{
    ui->status->setText(st);
}

void PozycjeRoleta::on_pbStart_clicked()
{
    debug("Start");
    emit start();
    if (!connected)
        emit doConnect();
    else
        emit checkDevice();
}

void PozycjeRoleta::on_pbNoweDane_clicked()
{
    emit noweDane();
    //ui->pbStart->setEnabled(false);
    ui->pbZapisz->setEnabled(false);
}


void PozycjeRoleta::on_pbZapisz_clicked()
{

}
