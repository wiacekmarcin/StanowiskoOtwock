#include "roletaclose.h"
#include "ui_roletaclose.h"

#include <QDebug>

RoletaClose::RoletaClose(Ustawienia & ust, SerialDevice * sd, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RoletaClose),
    sDev(sd)
{
    ui->setupUi(this);
    mech.setUstawienia(ust);
    mechRol.setUstawienia(ust);
    mech.setPrzestrzen(ust.getRolOsXReal().toUInt(), ust.getRolOsYReal().toUInt());
    mech.setReverseY(false);
    mech.setReverseX(false);
    mech.setReverseR(true);
    
    sDev->setParams(mech.getReverseX(), mech.getReverseY(), mech.getReverseR(),
                      mech.getMaxImpusyX(), mech.getMaxImpusyY(),
                      mech.getMaxKrokiX(), mech.getMaxKrokiY(),
                      mech.getMaxKrokiR(), mechRol.getMinKroki(),
                      mechRol.getSpeedHome(), mechRol.getSpeedPos());


    connect(sDev, &SerialDevice::kontrolerConfigured, this, &RoletaClose::successOpenDevice);

    connect(sDev, &SerialDevice::setParamsDone, this, &RoletaClose::setParamsDone);
    connect(sDev, &SerialDevice::setPositionDone, this, &RoletaClose::setPositionDone);

    connect(sDev, &SerialDevice::error, this, &RoletaClose::errorSerial, Qt::QueuedConnection);
    connect(sDev, &SerialDevice::debug, this, &RoletaClose::debug, Qt::QueuedConnection);

    connect(ui->pbCloseRoleta, &QPushButton::clicked, this, &RoletaClose::setCloseRoleta);


    visibleFound(false);
    visibleIdent(false);
    visibleConf(false);
    visibleOpen(false);
    ui->statusK->setText("Szukam kontrolera");
    ui->status->setText("");
    sDev->connectToDevice();
    ui->pbCloseRoleta->setEnabled(false);
}

RoletaClose::~RoletaClose()
{
    //disconnect(sDev, nullptr, this, nullptr);
    sDev->closeDevice(false);

    delete ui;
    //disconnect()

}

void RoletaClose::setCloseRoleta()
{
    sDev->setRoletaHome();
    sDev->closeDevice(true);
}

void RoletaClose::setPositionDone(bool success, bool home, int work)
{
    //qDebig() << __LINE__ << success << home << work;
    if (success) {
        switch (work) {
        case SerialMessage::START_R:
            ui->status->setText("Zamykanie rolety....");
            break;
        
        case SerialMessage::END_R:
            ui->status->setText("Roleta zamknieta");
            break;

        default:
            break;
        };
    }
}

void RoletaClose::successOpenDevice(bool succ, int state)
{
    //qDebig() << __FILE__ << __LINE__ << succ << state;
    //if (!succ) {
    //    ui->statusK->setText("Nie otwarty");
    //    return;
    //}

    switch(state) {

    case SerialDevice::NO_FOUND:
        visibleFound(true);
        ui->rbKontrolerFoundNo->setChecked(true);
        break;

    case SerialDevice::FOUND:
        visibleFound(true);
        ui->rbKontrolerFoundYes->setChecked(true);
        break;

    case SerialDevice::NO_OPEN:
        visibleOpen(true);
        ui->rbKontrolerOpenNo->setChecked(true);
        ui->statusK->setText(QString("Nie udało się otworzyć portu"));
        break;

    case SerialDevice::OPEN:
        visibleOpen(true);
        ui->rbKontrolerOpenYes->setChecked(true);
        break;

    case SerialDevice::NO_READ:
        visibleIdent(true);
        ui->rbKontrolerIdentNo->setChecked(true);
        ui->statusK->setText(QString("Problem z odczytem z portu"));
        break;

    case SerialDevice::IDENT_FAILD:
        visibleIdent(true);
        ui->rbKontrolerIdentNo->setChecked(true);
        ui->statusK->setText(QString("Nieprawidłowy kontroler."));
        break;

    case SerialDevice::IDENT_OK:
        visibleIdent(true);
        ui->rbKontrolerIdentYes->setChecked(true);
        break;

    case SerialDevice::PARAMS_FAILD:
        visibleConf(true);
        ui->rbKontrolerKonfNo->setChecked(true);
        ui->statusK->setText(QString("Problem z konfiguracja"));
        break;

    case SerialDevice::PARAMS_OK:
        visibleConf(true);
        ui->rbKontrolerKonfYes->setChecked(true);
        ui->statusK->setText(QString("Kontroler OK"));
        break;

    case SerialDevice::ALL_OK:
        setParamsDone(true);
        break;

    case SerialDevice::CLOSE:
        ui->statusK->setText(QString(""));
        break;

    default:
        break;
    }
}

void RoletaClose::setParamsDone(bool success)
{
    //qDebig() << __LINE__ << success;
    if (success) {
        //ui->status->setText("Zerowanie");
        ui->statusK->setText(QString("Kontroler OK"));
        ui->pbCloseRoleta->setEnabled(true);
    } else {
        ui->statusK->setText(QString("Błąd konfiguracji"));
        sDev->closeDevice(false);
    }
}

void RoletaClose::debug(const QString &s)
{
    (void)s;
    //qDebig() << s;
}

void RoletaClose::errorSerial(const QString &e)
{
    (void)e;
    //qDebig() << "ERR: " << e;
}

void RoletaClose::visibleFound(bool vis)
{
    ui->lFound->setEnabled(vis);
    ui->rbKontrolerFoundYes->setEnabled(vis);
    ui->rbKontrolerFoundNo->setEnabled(vis);
}

void RoletaClose::visibleIdent(bool vis)
{
    ui->lIdent->setEnabled(vis);
    ui->rbKontrolerIdentYes->setEnabled(vis);
    ui->rbKontrolerIdentNo->setEnabled(vis);
}

void RoletaClose::visibleConf(bool vis)
{
    ui->lKonfiguracja->setEnabled(vis);
    ui->rbKontrolerKonfYes->setEnabled(vis);
    ui->rbKontrolerKonfNo->setEnabled(vis);
}

void RoletaClose::visibleOpen(bool vis)
{
    ui->lOpen->setEnabled(vis);
    ui->rbKontrolerOpenYes->setEnabled(vis);
    ui->rbKontrolerOpenNo->setEnabled(vis);
}

