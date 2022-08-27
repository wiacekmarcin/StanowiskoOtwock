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
    mechRol.setUstawienia(ust);
    mech.setPrzestrzen(ust.getRolOsXReal().toUInt(), ust.getRolOsYReal().toUInt());
    mech.setReverseY(false);
    mech.setReverseX(!left);
    mech.setReverseR(true);
    mech.setMaxKrokiR(mechRol.getMaxKroki());
    sDev->setParams(mech.getReverseX(), mech.getReverseY(), mech.getReverseR(),
                      mech.getMaxImpusyX(), mech.getMaxImpusyY(),
                      mech.getMaxKrokiX(), mech.getMaxKrokiY(),
                      mech.getMaxKrokiR(), mechRol.getMinKroki(),
                      mechRol.getSpeedHome(), mechRol.getSpeedPos());

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


    visibleFound(false);
    visibleIdent(false);
    visibleConf(false);
    visibleHomePos(false);
    visibleOpen(false);
    visibleProgres(false);
    ui->statusK->setText("Szukam kontrolera");
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
    //qDebig() << __LINE__ << success << home << work;
    if (success) {
        switch (work) {
        case SerialMessage::START_XY:
            ui->progressBar->setValue(1);
            break;
        
        case SerialMessage::START_X:
            ui->progressBar->setValue(2);
            break;

        case SerialMessage::END_X:
            ui->progressBar->setValue(3);
            break;

        case SerialMessage::START_Y:
            ui->progressBar->setValue(4);
            break;

        case SerialMessage::END_Y:
            ui->progressBar->setValue(5);
            break;

        case SerialMessage::END_XY:
            ui->progressBar->setValue(6);
            break;

        default:
            break;
        };
    }
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
        ui->status->setText(QString("Błąd pozycjonowania %1 %2 %3").arg(success).arg(home).arg(work));
    }
}

void PozycjonowanieOffsetuNormy::successOpenDevice(bool succ, int state)
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

void PozycjonowanieOffsetuNormy::setParamsDone(bool success)
{
    //qDebig() << __LINE__ << success;
    if (success) {
        //ui->status->setText("Zerowanie");
        visibleHomePos(true);
        ui->rbKontrolerPosHomeYes->setChecked(true);
        ui->statusK->setText(QString("Kontroler OK"));
        ui->progressBar->setValue(0);
        visibleProgres(true);
        sDev->setPositionHome();
    } else {
        visibleHomePos(true);
        visibleProgres(false);
        ui->rbKontrolerPosHomeNo->setChecked(true);
        ui->statusK->setText(QString("Błąd konfiguracji"));
        sDev->closeDevice(false);
    }
}

void PozycjonowanieOffsetuNormy::debug(const QString &s)
{
    (void)s;
    //qDebig() << s;
}

void PozycjonowanieOffsetuNormy::errorSerial(const QString &e)
{
    (void)e;
    //qDebig() << "ERR: " << e;
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

void PozycjonowanieOffsetuNormy::visibleFound(bool vis)
{
    ui->lFound->setEnabled(vis);
    ui->rbKontrolerFoundYes->setEnabled(vis);
    ui->rbKontrolerFoundNo->setEnabled(vis);
}

void PozycjonowanieOffsetuNormy::visibleIdent(bool vis)
{
    ui->lIdent->setEnabled(vis);
    ui->rbKontrolerIdentYes->setEnabled(vis);
    ui->rbKontrolerIdentNo->setEnabled(vis);
}

void PozycjonowanieOffsetuNormy::visibleConf(bool vis)
{
    ui->lKonfiguracja->setEnabled(vis);
    ui->rbKontrolerKonfYes->setEnabled(vis);
    ui->rbKontrolerKonfNo->setEnabled(vis);
}

void PozycjonowanieOffsetuNormy::visibleHomePos(bool vis)
{
    ui->lPosHome->setEnabled(vis);
    ui->rbKontrolerPosHomeNo->setEnabled(vis);
    ui->rbKontrolerPosHomeYes->setEnabled(vis);
}

void PozycjonowanieOffsetuNormy::visibleOpen(bool vis)
{
    ui->lOpen->setEnabled(vis);
    ui->rbKontrolerOpenYes->setEnabled(vis);
    ui->rbKontrolerOpenNo->setEnabled(vis);
}

void PozycjonowanieOffsetuNormy::visibleProgres(bool vis)
{
    ui->progressBar->setEnabled(vis);
    ui->lKoniec->setEnabled(vis);
    ui->Lstart->setEnabled(vis);
    ui->lX->setEnabled(vis);
    ui->lY->setEnabled(vis);
}