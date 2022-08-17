#include "pozycjonowanieoffsetunormy.h"
#include "ui_pozycjonowanieoffsetunormy.h"

#include <QDebug>

PozycjonowanieOffsetuNormy::PozycjonowanieOffsetuNormy(Ustawienia & ust, SerialDevice * sd, bool left,
                                                       unsigned int offsetX, unsigned int offsetY,
                                                       QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PozycjonowanieOffsetuNormy),
    oX(offsetX),
    oY(offsetY),
    sDev(sd),
    stepMM(1),
    reverse(left)
{
    ui->setupUi(this);
    mech.setUstawienia(ust);
    mech.setPrzestrzen(1200, 2000);
    mech.setReverseY(false);
    mech.setReverseX(!left);
    mech.setReverseR(true);

    sDev->setParams(mech.getReverseX(), mech.getReverseY(), mech.getReverseR(),
                      mech.getMaxImpusyX(), mech.getMaxImpusyY(),
                      mech.getMaxKrokiX(), mech.getMaxKrokiY(),
                      mech.getMaxKrokiR());

    ui->rb1->setChecked(true);

    ui->left->setEnabled(false);
    ui->right->setEnabled(false);
    ui->up->setEnabled(false);
    ui->down->setEnabled(false);

    ui->pos->setText(QString("%1 x %2").arg(oX).arg(oY));

    connect(ui->left, &QToolButton::clicked, this, &PozycjonowanieOffsetuNormy::left);
    connect(ui->right, &QToolButton::clicked, this, &PozycjonowanieOffsetuNormy::right);
    connect(ui->up, &QToolButton::clicked, this, &PozycjonowanieOffsetuNormy::up);
    connect(ui->down, &QToolButton::clicked, this, &PozycjonowanieOffsetuNormy::down);

    connect(ui->rb1, &QRadioButton::clicked, this, &PozycjonowanieOffsetuNormy::rb1_clicked);
    connect(ui->rb10, &QRadioButton::clicked, this, &PozycjonowanieOffsetuNormy::rb10_clicked);
    connect(ui->rb100, &QRadioButton::clicked, this, &PozycjonowanieOffsetuNormy::rb100_clicked);

    connect(sDev, &SerialDevice::kontrolerConfigured, this, &PozycjonowanieOffsetuNormy::successOpenDevice);

    connect(sDev, &SerialDevice::setParamsDone, this, &PozycjonowanieOffsetuNormy::setParamsDone);
    connect(sDev, &SerialDevice::setPositionDone, this, &PozycjonowanieOffsetuNormy::setPositionDone);

    connect(sDev, &SerialDevice::error, this, &PozycjonowanieOffsetuNormy::errorSerial, Qt::QueuedConnection);
    connect(sDev, &SerialDevice::debug, this, &PozycjonowanieOffsetuNormy::debug, Qt::QueuedConnection);


    sDev->connectToDevice();
}

PozycjonowanieOffsetuNormy::~PozycjonowanieOffsetuNormy()
{
    //disconnect(sDev, nullptr, this, nullptr);
    sDev->closeDevice(false);

    delete ui;
    //disconnect()

}

void PozycjonowanieOffsetuNormy::rb1_clicked()
{
    stepMM = 1;
}

void PozycjonowanieOffsetuNormy::rb10_clicked()
{
   stepMM = 10;
}

void PozycjonowanieOffsetuNormy::rb100_clicked()
{
    stepMM = 100;
}

void PozycjonowanieOffsetuNormy::up()
{
    oY -= stepMM;
    if (oY < 0)
        oY = 0;
    setPos();
}

void PozycjonowanieOffsetuNormy::down()
{
    oY += stepMM;
    if (oY > (int)mech.getMaxYmm())
        oY = mech.getMaxYmm();
    setPos();
}

void PozycjonowanieOffsetuNormy::left()
{
    if (reverse) {
        oX -= stepMM;
        if (oX < 0)
            oX = 0;
    } else {
        oX += stepMM;
        if (oX > (int)mech.getMaxXmm())
            oX = mech.getMaxXmm();
    }
    setPos();
}

void PozycjonowanieOffsetuNormy::right()
{
    if (reverse) {
        oX += stepMM;
        if (oX > (int)mech.getMaxXmm())
            oX = mech.getMaxXmm();
    } else {
        oX -= stepMM;
        if (oX < 0)
            oX = 0;
    }
    setPos();
}

void PozycjonowanieOffsetuNormy::setPositionDone(bool success, bool home, int work)
{
    qDebug() << __LINE__ << success << home << work;
    if (success && !home && work == SerialMessage::END_XY) {
        ui->status->setText("Pozycja ustawiona");
        ui->left->setEnabled(true);
        ui->right->setEnabled(true);
        ui->up->setEnabled(true);
        ui->down->setEnabled(true);
    } else if (success && home && work == SerialMessage::END_XY) {
        ui->status->setText("Pozycjonowanie");
        setPos();
    } else if (success && home && work != SerialMessage::ERROR_XY) {
        ui->status->setText("Zerowanie");
    } else if (success && !home && work != SerialMessage::ERROR_XY) {
        ui->status->setText("Pozycjonowanie");
    } else {
        ui->status->setText("Błąd");
    }
}

void PozycjonowanieOffsetuNormy::successOpenDevice(bool succ, int state)
{
    qDebug() << __FILE__ << __LINE__ << succ << state;
    

    switch(state) {

    case SerialDevice::NO_FOUND:
        ui->status->setText("Nie znaleziono kontrolera");
        break;

    case SerialDevice::FOUND:
        break;

    case SerialDevice::NO_OPEN:
        ui->status->setText(QString("Nie udało się otworzyć portu"));
        break;

    case SerialDevice::OPEN:
        break;

    case SerialDevice::NO_READ:
        ui->status->setText(QString("Problem z odczytem z portu"));
        break;

    case SerialDevice::IDENT_FAILD:
        ui->status->setText(QString("Nieprawidłowy kontroler."));
        break;

    case SerialDevice::IDENT_OK:
        break;

    case SerialDevice::PARAMS_FAILD:
        ui->status->setText(QString("Problem z konfiguracja"));
        break;

    case SerialDevice::PARAMS_OK:
        ui->status->setText(QString("Kontroler OK"));
        break;

    case SerialDevice::ALL_OK:
        setParamsDone(true);
        break;

    case SerialDevice::CLOSE:
        ui->status->setText(QString(""));
        break;

    default:
        break;
    }
}

void PozycjonowanieOffsetuNormy::setParamsDone(bool success)
{
    qDebug() << __LINE__ << success;
    if (success) {
        ui->status->setText("Zerowanie");
        sDev->setPositionHome();
    } else {
        ui->status->setText("Błąd Sterownika");
        sDev->closeDevice(false);
    }
}

void PozycjonowanieOffsetuNormy::debug(const QString &s)
{
    qDebug() << s;
}

void PozycjonowanieOffsetuNormy::errorSerial(const QString &e)
{
    qDebug() << "ERR: " << e;
}

void PozycjonowanieOffsetuNormy::setPos()
{
    ui->pos->setText(QString("%1 x %2").arg(oX).arg(oY));
    ui->left->setEnabled(false);
    ui->right->setEnabled(false);
    ui->up->setEnabled(false);
    ui->down->setEnabled(false);
    sDev->setPosition(mech.getImpulsyX(oX), mech.getImpulsyY(oY));
}
