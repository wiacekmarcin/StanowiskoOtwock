#include "miernikprzeplywu.h"
#include "ui_miernikprzeplywu.h"

#include "wyborkwadratow.h"
#include "recznedodpozycji.h"
#include "wybranyplik.h"
#include "wybranypliknorma.h"
#include "ustawienia.h"
#include "mierzonepozycje.h"
#include "pozycjeroleta.h"
#include "wentylator.h"
#include "podzialetapurolety.h"

#include <QMessageBox>
#include <QCloseEvent>
#include <QWidget>
#include <QSerialPortInfo>

#include <QDateTime>
#include <QTime>

#include <QDebug>

#define TABVISIBLE 0

MiernikPrzeplywu::MiernikPrzeplywu()
    : QMainWindow(NULL)
    , ui(new Ui::MiernikPrzeplywu)
    , modeWork(WyborMetody::MODE_NONE)
    , methodIns(WyborMetody::METHOD_NONE)
    , deviceConn(false)
    , deviceReady(false)
    , sendParams(false)
    , checkRadio(false)
{
    ui->setupUi(this);
    widget = nullptr;

    //setUstawienia();

    connect(&sMsg, &SerialMessage::errorSerial, this, &MiernikPrzeplywu::errorSerial, Qt::QueuedConnection);
    connect(&sMsg, &SerialMessage::debug, this, &MiernikPrzeplywu::debug, Qt::QueuedConnection);

    connect(&sMsg, &SerialMessage::deviceName, this, &MiernikPrzeplywu::deviceName, Qt::QueuedConnection);
    connect(&sMsg, &SerialMessage::controllerOK, this, &MiernikPrzeplywu::controllerOK, Qt::QueuedConnection);
    connect(&sMsg, &SerialMessage::successOpenDevice, this, &MiernikPrzeplywu::successOpenDevice);

    connect(&sMsg, &SerialMessage::setParamsDone, this, &MiernikPrzeplywu::setParamsDone);

    connect(&sMsg, &SerialMessage::readFromRadio, this, &MiernikPrzeplywu::readedFromRadio);
    connect(&sMsg, &SerialMessage::errorReadFromRadio, this, &MiernikPrzeplywu::errorReadFromRadio);

    connect(&sMsg, &SerialMessage::positionStatus, this, &MiernikPrzeplywu::positionStatus);
    connect(&sMsg, &SerialMessage::homeStatus, this, &MiernikPrzeplywu::homeStatus);
    connect(&sMsg, &SerialMessage::errorHome, this, &MiernikPrzeplywu::errorHome);
    connect(&sMsg, &SerialMessage::errorHomeRoleta, this, &MiernikPrzeplywu::errorHomeRoleta);



    connect(this, &MiernikPrzeplywu::connectToDeviceSig, &sMsg, &SerialMessage::connectToSerial, Qt::QueuedConnection);
    connect(this, &MiernikPrzeplywu::setPositionHomeSig, &sMsg, &SerialMessage::setPositionHome, Qt::QueuedConnection);
    connect(this, &MiernikPrzeplywu::setPositionSig, &sMsg, &SerialMessage::setPosition, Qt::QueuedConnection);
    connect(this, &MiernikPrzeplywu::setRoletaHomeSig, &sMsg, &SerialMessage::setRoletaHome, Qt::QueuedConnection);
    connect(this, &MiernikPrzeplywu::setRoletaSig, &sMsg, &SerialMessage::setRoleta, Qt::QueuedConnection);
    connect(this, &MiernikPrzeplywu::setParamsSig, &sMsg, &SerialMessage::setParams, Qt::QueuedConnection);
    connect(this, &MiernikPrzeplywu::readRadioSig, &sMsg, &SerialMessage::readRadio, Qt::QueuedConnection);

    showMaximized();
    chooseWork();

}

MiernikPrzeplywu::~MiernikPrzeplywu()
{
    delete ui;
}

void MiernikPrzeplywu::chooseWork()
{
    qDebug() << "chooseWork()" << modeWork << methodIns;
    hide();
    do {
        qDebug() << "New wyborMethody";
        WyborMetody m(this, modeWork, methodIns, ust);

        int r = m.exec();
        if (r == 0) {
            QApplication::quit();
            exit(0);
        }
        modeWork = m.getWbMode();
        methodIns = m.getWbMetoda();
        chooseTab();
        if (chooseMethod(modeWork, methodIns, m.getData())) {
            show();
            return;
        }
    } while (true);
}

void MiernikPrzeplywu::chooseTab()
{
    if (modeWork == WyborMetody::MODE_FUNSET) {
        ui->stackedWidget->setCurrentIndex(3);
        widget = ui->widget_wentylator;

    }
    else if (modeWork == WyborMetody::MODE_SERVICE) {
        widget = new TabWidget();
    }
    else if (modeWork == WyborMetody::MODE_2700) {
        ui->stackedWidget->setCurrentIndex(0);
        widget = ui->widget_2700_3000;
    }
    else if (modeWork == WyborMetody::MODE_1000L) {
        ui->stackedWidget->setCurrentIndex(2);
        widget = ui->widget_1000_2000lewe;
    }
    else if (modeWork == WyborMetody::MODE_1000P) {
        ui->stackedWidget->setCurrentIndex(1);
        widget = ui->widget_1000_2000prawe;
    }
    else if (modeWork == WyborMetody::MODE_ROLETA) {
         widget = ui->widget_860_1500;
         ui->stackedWidget->setCurrentIndex(4);
    }

    ui->eStatusRoleta->setEnabled(modeWork == WyborMetody::MODE_ROLETA);
    ui->lStatusRoleta->setEnabled(modeWork == WyborMetody::MODE_ROLETA);

    ui->lradio->setEnabled(modeWork != WyborMetody::MODE_FUNSET);
    ui->eRadio->setEnabled(modeWork != WyborMetody::MODE_FUNSET);

    ui->lcz1mv->setEnabled(modeWork != WyborMetody::MODE_FUNSET);
    ui->lcz1unit->setEnabled(modeWork != WyborMetody::MODE_FUNSET);
    ui->ecz1->setEnabled(modeWork != WyborMetody::MODE_FUNSET);

    if (widget == nullptr) {
        QCoreApplication::exit(1);
        return;
    }

    widget->setMiernikPrzeplywu(this);

    calculateMechanika();
}
void MiernikPrzeplywu::calculateMechanika()

{
    switch(modeWork) {
    case WyborMetody::MODE_ROLETA:
        mech.setPrzestrzen(1200, 2000);
        mech.setReverseY(false);
        mech.setReverseX(true);
        mech.setReverseR(true);
        break;
    case WyborMetody::MODE_2700:
    case WyborMetody::MODE_FUNSET:
        mech.setPrzestrzen(2700, 3000);
        mech.setReverseY(false);
        mech.setReverseX(false);
        mech.setReverseR(true);
        break;
    case WyborMetody::MODE_1000L:
    case WyborMetody::MODE_1000P:
        mech.setPrzestrzen(1000, 2000);
        mech.setReverseY(false);
        mech.setReverseX(modeWork == WyborMetody::MODE_1000P);
        mech.setReverseR(true);
        break;
    case WyborMetody::MODE_NONE:
    default:
        mech.setPrzestrzen(0, 0);
        mech.setReverseY(false);
        mech.setReverseX(false);
        mech.setReverseR(false);
        break;
    }
    mech.setMaxKrokiR(160000);
    widget->setMechanika(mech);
    sMsg.setMechParams(mech.getReverseX(), mech.getReverseY(), mech.getReverseR(),
                      mech.getMaxImpusyX(), mech.getMaxImpusyY(),
                      mech.getMaxKrokiX(), mech.getMaxKrokiY(),
                      mech.getMaxKrokiR());
}

bool MiernikPrzeplywu::chooseMethod(const WyborMetody::ModeWork & modeWork,
                                   const WyborMetody::MethodInsData & methodIns,
                                   const WyborMetodyData & values)
{
    this->modeWork = modeWork;
    this->methodIns = methodIns;
    this->data = values;

    if (modeWork == WyborMetody::MODE_FUNSET) {
        return true;
    }

    if (modeWork == WyborMetody::MODE_1000L || modeWork == WyborMetody::MODE_1000P || modeWork == WyborMetody::MODE_2700)
    {
        MierzonePozycje * w = static_cast<MierzonePozycje*>(widget);
        if (methodIns == WyborMetody::METHOD_FILE) {
            WybranyPlik wp(this, values.fileName, modeWork == WyborMetody::MODE_2700 ? 2700 : 1000,
                                           modeWork == WyborMetody::MODE_2700 ? 3000 : 2000);
            int r = wp.exec();
            if (r == 0) {
                return  false;
            }
            w->setList(wp.getList());
            return true;
        } else if (methodIns == WyborMetody::METHOD_SQUERE) {
            WyborKwadratow wk(this, values.numberWidth, values.numberHeight, values.timeStopAuto, modeWork == WyborMetody::MODE_2700 ? 2700 : 1000,
                                                               modeWork == WyborMetody::MODE_2700 ? 3000 : 2000);
            int r = wk.exec();
            if (r == 0) {
                return false;
            }
            w->setList(wk.getList());
            return true;
        } else if (methodIns == WyborMetody::METHOD_MANUAL) {
            ReczneDodPozycji rdp(this, values.numberPozMan, values.timeStopManual, modeWork == WyborMetody::MODE_2700 ? 2700 : 1000,
                                                            modeWork == WyborMetody::MODE_2700 ? 3000 : 2000);
            int r = rdp.exec();
            if (r == 0) {
                return false;
            }
            w->setList(rdp.getList());
            return true;
        }
    } //mode MODE_1000L || MODE_1000L || MODE_1000P) {

    if (modeWork == WyborMetody::MODE_ROLETA) {
        PozycjeRoleta * w = static_cast<PozycjeRoleta*>(widget);
        if (methodIns == WyborMetody::METHOD_MANUAL) {
            //TODO

            PodzialEtapuRolety pdr(this, values.etapNrRoleta, values.timeStopRoleta,
                                  values.stableTimeRoleta, 1500, 870,
                                  values.offsetX, values.offsetY);
            int r = pdr.exec();
            if (r == 0) {
                return false;
            }
            w->setList(pdr.getLista());
            return true;
        } else if (methodIns == WyborMetody::METHOD_FILE) {
            qDebug() << "alloha" << values.stableTimeRoleta;
            WybranyPlikNorma wpn(this, values.fileName2, values.etapNrRoleta, values.stableTimeRoleta,
                                 1500, 870, values.offsetX, values.offsetY);
            int r = wpn.exec();
            if (r == 0) {
                return  false;
            }
            w->setList(wpn.getList());
            return true;
        }
    }
    return false;
}


QString MiernikPrzeplywu::addTime(QString status)
{
    return QString("[%1] %2").arg(QTime::currentTime().toString("HH:mm:ss.zzz")).arg(status);
}

void MiernikPrzeplywu::debugClear()
{
    //ui->debug->clear();
}

void MiernikPrzeplywu::closeEvent (QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Preplywomierz",
                                                                tr("Czy jesteś pewien?\n"),

                                                                QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        event->accept();
    }
}

void MiernikPrzeplywu::setUstawienia()
{
    ust.read();
    //mech.setUstawienia(ust);

    auto val1 = ust.getImpulsyXperMM();
    if (!val1.isEmpty())
        mech.setImpusyXPerMM(val1.toUInt());

    auto val2 = ust.getImpulsyYperMM();
    if (!val2.isEmpty())
        mech.setImpusyYPerMM(val2.toUInt());

    auto val3 = ust.getKrokiXperMM();
    if (!val3.isEmpty())
        mech.setKrokiXPerMM(val3.toUInt());

    auto val4 = ust.getKrokiYperMM();
    if (!val4.isEmpty())
        mech.setKrokiYPerMM(val4.toUInt());

    auto val5 = ust.getWentOffsetX();
    if (!val5.isEmpty())
        mech.setWentOffsetX(val5.toDouble());

    auto val6 = ust.getWentOffsetY();
    if (!val6.isEmpty())
        mech.setWentOffsetY(val6.toDouble());

    auto val7 = ust.getKatnachylenia();
    if (!val7.isEmpty())
        mech.setWentKatNach(val7.toDouble());


    mech.calculate();
}




void MiernikPrzeplywu::errorSerial(const QString & error)
{
    sMsg.closeDevice();
    deviceConn = false;
    ui->statusserial->setText(QString("Błąd : %1").arg(error));
    ui->lStatus->setText("-");
    ui->lStatusMinor->setText("-");
    widget->setConnect(false, error);
    widget->errorSerial(error);
}

void MiernikPrzeplywu::successOpenDevice(bool open)
{
    debug(QString("Open device open = %1").arg(open));
    if (open) {
        ui->statusserial->setText("Sprawdzam sterownik...");
        if (modeWork == WyborMetody::MODE_FUNSET) {
            widget->setStatus(addTime("Sprawdzam sterownik."));
        } else {
            widget->setStatus("Sprawdzam sterownik.");
        }
    } else {
        ui->lConneected->setText("Nie udalo sie otworzyc portu");
        widget->setStatus("Nie udalo sie otworzyc portu");
        widget->setConnect(false);
        sMsg.closeDevice();
        deviceConn = false;
        widget->setConnect(false);
        widget->setIsReady(false);
    }
}

void MiernikPrzeplywu::controllerOK()
{
    debug("controllerOk");
    ui->statusserial->setText("Sterownik OK");
    deviceConn = true;
    debug ("setParams");
    ui->lStatus->setText("Ustawiam parametry...");
    emit setParams(mech.getReverseX(),mech.getReverseY(),mech.getReverseR(),
                   mech.getMaxImpusyX(), mech.getMaxImpusyY(),
                   mech.getMaxKrokiX(), mech.getMaxKrokiY(), mech.getMaxKrokiR());

}



void MiernikPrzeplywu::readedFromRadio(int value)
{
    debug(QString("Read radio %1").arg(value));
    if (checkRadio) {
        checkRadio = false;
        widget->setStatus(QString("Zakończono konfigurację urządzenia. Ustawiam czujnik w pozycji bazowej"));
        ui->lStatus->setText(QString("Zakończono konfigurację kontrolera. Trwa zerowanie urządzenia...."));
    ui->lradio->setText(QString("Widoczny"));
        setPositionHome();
        return;
    }
    ui->lcz1mv->setText(QString::number(ust.getRatioCzujnik1().toDouble()*value, 'g', 4));
    ui->lcz1unit->setText(ust.getUnitCzujnik1());
    ui->lStatusMinor->setText("Trwa pomiar....");
    widget->readedFromRadio(ust.getRatioCzujnik1().toDouble()*value);
}

void MiernikPrzeplywu::errorReadFromRadio()
{
    debug(QString("errorReadFromRadio"));
    ui->lradio->setText(QString("Błąd odczytu"));
    ui->lcz1mv->setText("---");
    ui->lcz1unit->setText("---");
    if (checkRadio) {
        checkRadio = false;
        setPositionHome();
    } else {
        widget->errorReadFromRadio();
    }
}

void MiernikPrzeplywu::deviceName(QString portname)
{
    debug(QString("DeviceName = %1").arg(portname));
    ui->lConneected->setText(portname);
}

void MiernikPrzeplywu::positionStatus(SerialMessage::StatusWork work)
{
    switch(work) {
    case SerialMessage::START_XY:
        ui->lStatus->setText("Ustawianie pozycji czujnika...");
        ui->lStatusMinor->setText("--");
        ui->lcz1mv->setText("--");
        ui->lcz1unit->setText("--");
        ui->statusbar->showMessage("Ustawianie czujnika.",5000);
        break;
    case SerialMessage::START_X:
        ui->lStatusMinor->setText("Oś pozioma.");
        ui->statusbar->showMessage("Rozpoczynam ustawianie czujnika na osi X.", 5000);
        break;
    case SerialMessage::END_X:
        ui->lStatusMinor->setText("--");
        ui->statusbar->showMessage("Ustawianie pozycji czujnika. Zakończone ustawianie czujnika na osi X.", 5000);
        break;
    case SerialMessage::START_Y:
        ui->lStatusMinor->setText("Oś pionowa");
        ui->statusbar->showMessage("Ustawianie pozycji czujnika. Rozpoczynam ustawianie czujnika na osi Y.", 5000);
        break;
    case SerialMessage::END_Y:
        ui->lStatusMinor->setText("--");
        ui->statusbar->showMessage("Ustawianie pozycji czujnika. Zakończone ustawianie czujnika na osi Y.", 5000);
        break;
    case SerialMessage::END_XY:
        ui->lStatus->setText("Czujnik ustawiony na pozycji.");
        ui->lStatusMinor->setText("--");
        ui->statusbar->showMessage("Czujnik ustawiony na pozycji.",5000);
        widget->positionDone(false);
        break;
    case SerialMessage::START_R:
        ui->lStatusRoleta->setText("Podnoszenie rolety...");
        ui->statusbar->showMessage("Podnoszenie rolety do pozycji.",5000);
        break;
    case SerialMessage::END_R:
        ui->lStatusRoleta->setText("Roleta ustawiona.");
        ui->statusbar->showMessage("Zakończono ustawianie rolety na pozycji.",5000);
        widget->roletaDone(false);
        break;
    case SerialMessage::ERROR_XY:
        ui->lStatusMinor->setText("Błąd pozycjonowania czujnika");
        QMessageBox::critical(this, "Pozycjonowanie czujnika", "Nie udało się ustawić czujnika w pozycji zadanej.");
        return;
    case SerialMessage::ERROR_R:
        ui->lStatusRoleta->setText("Błąd podnoszenia rolety");
        QMessageBox::critical(this, "Pozycjonowanie rolety", "Nie udało się ustawić rolety w pozycji zadanej.");
        return;
    }
    widget->positionStatus(false, work);
}

void MiernikPrzeplywu::homeStatus(SerialMessage::StatusWork work)
{
    switch(work) {
    case SerialMessage::START_XY:
        ui->lStatus->setText("Zerowanie pozycji czujnika...");
        ui->lStatusMinor->setText("--");
        ui->statusbar->showMessage("Ustawianie pozycji bazowej czujnika.");
        ui->lcz1mv->setText("--");
        ui->lcz1unit->setText("--");
        break;
    case SerialMessage::START_X:
        ui->lStatusMinor->setText("Oś pozioma.");
        ui->statusbar->showMessage("Ustawianie pozycji bazowej czujnika. Rozpoczynam ustawianie czujnika na osi X.", 5000);
        break;
    case SerialMessage::END_X:
        ui->lStatusMinor->setText("--");
        ui->statusbar->showMessage("Ustawianie pozycji bazowej czujnika. Zakończone ustawianie czujnika na pozycji bazowej dla osi X.", 5000);
        break;
    case SerialMessage::START_Y:
        ui->lStatusMinor->setText("Oś pionowa");
        ui->statusbar->showMessage("Ustawianie pozycji bazowej czujnika. Rozpoczynam ustawianie czujnika na pozycji bazowe dla osi Y.", 5000);
        break;
    case SerialMessage::END_Y:
        ui->lStatusMinor->setText("--");
        ui->statusbar->showMessage("Ustawianie pozycji bazowej czujnika. Zakończone ustawianie czujnika na pozycji bazowej dla osi Y.", 5000);
        break;
    case SerialMessage::END_XY:
        ui->lStatus->setText("Czujnik ustawiony na pozycji bazowej.");
        ui->lStatusMinor->setText("--");
        ui->statusbar->showMessage("Czujnik ustawiony na pozycji bazowej.",5000);
        if (sendParams) {
            sendParams = false;
            deviceReady = true;
            widget->setIsReady(true);
        }
        widget->positionDone(true);
        break;
    case SerialMessage::START_R:
        ui->lStatusRoleta->setText("Ustawianie rolety na pozycji bazowej.");
        ui->statusbar->showMessage("Ustawianie rolety na pozycji bazowej.",5000);
        break;
    case SerialMessage::END_R:
        ui->lStatusRoleta->setText("Roleta ustawiona na pozycji bazowej.");
        ui->statusbar->showMessage("Zakończono ustawianie rolety na pozycji bazowej.",5000);
        widget->roletaDone(true);
        break;
    case SerialMessage::ERROR_XY:
        QMessageBox::critical(this, QString("Kalibracja urządzenia"),
                              QString("Nie powiodła się kalibracja urządzenia. Wyłącz silniki i podjedź tacką do rogu gdzie są umieszczone silniki i spróbuje raz jeszcze"));
        return;
    case SerialMessage::ERROR_R:
        QMessageBox::critical(this, "Pozycjonowanie rolety", "Nie udało się ustawić rolety w pozycji bazowej");
        return;
    }
    widget->positionStatus(true, work);
}

void MiernikPrzeplywu::setParamsDone()
{
    debug("Ustawiono parametry.");
    sendParams = true;
    widget->setConnect(true);

    if (modeWork == WyborMetody::MODE_FUNSET) {
    widget->setStatus(QString("Zakończono konfigurację urządzenia. Ustawiam czujnik w pozycji bazowej"));
    ui->lStatus->setText(QString("Zakończono konfigurację kontrolera. Trwa zerowanie urządzenia...."));
    setPositionHome();
    } else {
        widget->setStatus(QString("Zakończono konfigurację urządzenia. Sprawdzam połączenie z modułem radiowym"));
        ui->lStatus->setText(QString("Zakończono konfigurację kontrolera. Sprawdzam połączenie z modułem radiowym...."));
        checkRadio = true;
        readRadio();
    }

}

void MiernikPrzeplywu::errorHome()
{
    debug("ERROR HOME");
}

void MiernikPrzeplywu::errorHomeRoleta()
{
    debug("ERROR HOME ROLETA");
}

void MiernikPrzeplywu::debug(const QString & dbg)
{
    //ui->debug->append(dbg);
    qDebug() << dbg;
}

const WyborMetodyData &MiernikPrzeplywu::getData() const
{
    return data;
}

void MiernikPrzeplywu::connectToDevice()
{
    emit connectToDeviceSig();
    //emit connectAndConfigureSig();
}

void MiernikPrzeplywu::setPositionHome()
{
    emit setPositionHomeSig();
}

void MiernikPrzeplywu::setPosition(uint32_t x, uint32_t y)
{
    emit setPositionSig(x,y);
}

void MiernikPrzeplywu::setRoletaHome()
{
    emit setRoletaHomeSig();
}

void MiernikPrzeplywu::setRoleta(uint32_t r)
{
    emit setRoletaSig(r);
}

void MiernikPrzeplywu::setParams(bool reverseX, bool reverseY, bool reverseR, uint32_t maxImpX, uint32_t maxImpY,
                                 uint32_t maxStepX, uint32_t maxStepY, uint32_t maxStepR)
{
    emit setParamsSig(reverseX, reverseY, reverseR, maxImpX, maxImpY, maxStepX, maxStepY, maxStepR);
}

void MiernikPrzeplywu::readRadio()
{
    emit readRadioSig();
}

WyborMetody::MethodInsData MiernikPrzeplywu::getMethodIns() const
{
    return methodIns;
}

WyborMetody::ModeWork MiernikPrzeplywu::getModeWork() const
{
    return modeWork;
}

void MiernikPrzeplywu::noweDane()
{
    //WyborMetody::ModeWork actWork = modeWork;
    //modeWork = WyborMetody::MODE_SERVICE;
    qDebug() << __FILE__ << __LINE__;
    sMsg.closeDevice();
    //QCoreApplication::exit(1);
    //exit(1);
    //if (widget)
    //    widget->restart();
    //on_tabWidget_currentChanged(actWork);
    //sMsg.closeDevice();
    chooseWork();
}


