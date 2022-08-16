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

#define DEBUGMP(X) debug(QString("%1:%2 %3").arg(__FILE__).arg(__LINE__).arg(X))

MiernikPrzeplywu::MiernikPrzeplywu()
    : QMainWindow(NULL)
    , ui(new Ui::MiernikPrzeplywu)
    , modeWork(WyborMetody::MODE_NONE)
    , methodIns(WyborMetody::METHOD_NONE)
    , deviceConn(false)
    , deviceReady(false)
    , firstRun(false)
    , firstRun2(false)
    , checkRadio(false)
{
    ui->setupUi(this);
    widget = nullptr;

    sMsg.setThread(&thSterownik);

    setUstawienia();

    connect(&sMsg, &SerialDevice::error, this, &MiernikPrzeplywu::errorSerial, Qt::QueuedConnection);
    connect(&sMsg, &SerialDevice::debug, this, &MiernikPrzeplywu::debug, Qt::QueuedConnection);

    connect(&sMsg, &SerialDevice::deviceName, this, &MiernikPrzeplywu::deviceName, Qt::QueuedConnection);
    connect(&sMsg, &SerialDevice::kontrolerConfigured, this, &MiernikPrzeplywu::successOpenDevice, Qt::QueuedConnection);

    connect(&sMsg, &SerialDevice::setParamsDone, this, &MiernikPrzeplywu::setParamsDone, Qt::QueuedConnection);
    connect(&sMsg, &SerialDevice::setPositionDone, this, &MiernikPrzeplywu::setPositionDone, Qt::QueuedConnection);

    connect(&sMsg, &SerialDevice::readFromRadio, this, &MiernikPrzeplywu::readedFromRadio, Qt::QueuedConnection);

    chooseWork();

}

MiernikPrzeplywu::~MiernikPrzeplywu()
{
    sMsg.setStop();
    //thSterownik.quit();
    delete ui;

    thSterownik.wait();
}

void MiernikPrzeplywu::chooseWork()
{
    //qDebug() << "chooseWork()" << modeWork << methodIns;

    do {
        //qDebug() << "New wyborMethody";
        hide();
        WyborMetody::ModeWork prevWork = modeWork;
        modeWork = WyborMetody::MODE_NONE;
        WyborMetody m(this, prevWork, methodIns, ust, sMsg);

        int r = m.exec();
        if (r == 0) {
            QApplication::quit();
            exit(0);
        }
        modeWork = m.getWbMode();
        methodIns = m.getWbMetoda();
        chooseTab();
        adjustSize();
        show();

        if (chooseMethod(modeWork, methodIns, m.getData())) {
            widget->resize(QSize(sizeHint().width()-25, sizeHint().height()-200));
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
    else if (modeWork == WyborMetody::MODE_ROLETAP) {
         widget = ui->widget_860_1500prawe;
         ui->stackedWidget->setCurrentIndex(5);
    }
    else if (modeWork == WyborMetody::MODE_ROLETAL) {
         widget = ui->widget_860_1500lewe;
         ui->stackedWidget->setCurrentIndex(4);
    }

    ui->eStatusRoleta->setEnabled(modeWork == WyborMetody::MODE_ROLETAP || modeWork == WyborMetody::MODE_ROLETAL);
    ui->lStatusRoleta->setEnabled(modeWork == WyborMetody::MODE_ROLETAP || modeWork == WyborMetody::MODE_ROLETAL);

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
    case WyborMetody::MODE_ROLETAP:
    case WyborMetody::MODE_ROLETAL:
        mech.setPrzestrzen(1200, 2000);
        mech.setReverseY(false);
        mech.setReverseX(modeWork == WyborMetody::MODE_ROLETAP);
        mechR.setReverse(modeWork == WyborMetody::MODE_ROLETAL);
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
    sMsg.setParams(mech.getReverseX(), mech.getReverseY(), mech.getReverseR(),
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

    if (modeWork == WyborMetody::MODE_ROLETAP || modeWork == WyborMetody::MODE_ROLETAL) {
        PozycjeRoleta * w = static_cast<PozycjeRoleta*>(widget);
        bool reverse = modeWork == WyborMetody::MODE_ROLETAL;

        if (methodIns == WyborMetody::METHOD_MANUAL) {
            //TODO

            PodzialEtapuRolety pdr(this, false, values.etapNrRoleta, values.timeStopRoleta,
                                  values.stableTimeRoleta, 1500, 870,
                                  reverse ? values.offsetXL : values.offsetXP,
                                  reverse ? values.offsetYL : values.offsetYP);
            int r = pdr.exec();
            if (r == 0) {
                return false;
            }
            w->setList(pdr.getLista());
            return true;
        } else if (methodIns == WyborMetody::METHOD_FILE) {
            //qDebug() << "alloha" << values.stableTimeRoleta;
            WybranyPlikNorma wpn(this, false, values.fileName2, values.etapNrRoleta, values.stableTimeRoleta,
                                 1500, 870,
                                 reverse ? values.offsetXL : values.offsetXP,
                                 reverse ? values.offsetYL : values.offsetYP);
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
    mech.setUstawienia(ust);
    mechR.setUstawienia(ust);

    mech.calculate();
    mechR.calculate();
}




void MiernikPrzeplywu::errorSerial(const QString & error)
{
    DEBUGMP(QString("Error from serial : ") + error);
    /*
    deviceConn = false;
    ui->lStatus->setText("-");
    ui->lStatusMinor->setText("-");
    widget->setIsReady(false);
    widget->setError();
    */
}

void MiernikPrzeplywu::setPositionDone(bool success, bool home, int w)
{
    DEBUGMP(QString("positionDone ok=%1 home=%2 w=%3").arg(success).arg(home).arg(w));
    SerialMessage::StatusWork work = static_cast<SerialMessage::StatusWork>(w);
    if (modeWork == WyborMetody::MODE_NONE)
        return;

    if (success) {
        if (work == SerialMessage::ERROR_XY) {
             home ? errorHome() : errorPosition();
        } else if (work == SerialMessage::ERROR_R) {
            home ? errorHomeRoleta() : errorRoleta();
        } else {
            home ? homeStatus(work) : positionStatus(work);
        }
    } else {
        if (work == SerialMessage::START_R || work == SerialMessage::END_R || work == SerialMessage::ERROR_R) {
            home ? errorHomeRoleta() : errorRoleta();
        } else {
            home ? errorHome() : errorPosition();
        }
    }
}

void MiernikPrzeplywu::successOpenDevice(bool open, bool openErr, bool conf, bool confErr, bool sett, bool settErr)
{
    if (modeWork == WyborMetody::MODE_NONE)
        return;

    DEBUGMP(QString("Open device open = %1, configure = %2 sett = %3 [openErr = %4, configureErr = %5, settErr = %6]").arg(open).arg(conf).arg(sett).
                                arg(openErr).arg(confErr).arg(settErr));
    if (!open && !openErr && !conf && !confErr && !sett && !settErr) {
        ui->lConneected->setText("");
        ui->lcz1mv->setDisabled(true);
        ui->lcz1unit->setDisabled(true);
        ui->ecz1->setDisabled(true);
        ui->eStatus->setDisabled(true);
        ui->lStatus->setDisabled(true);
        ui->lStatusMinor->setDisabled(true);
        ui->eStatusRoleta->setDisabled(true);
        ui->lStatusRoleta->setDisabled(true);
        ui->lStatus->setText("--");
        ui->lStatusMinor->setText("--");
        ui->lStatusRoleta->setText("--");
        ui->lcz1mv->setText("--");
        ui->lcz1unit->setText("--");
        ui->eRadio->setDisabled(true);
        ui->lradio->setDisabled(true);
        ui->lradio->setText("--");
    } else if (!open && openErr && !conf && !confErr && !sett && !settErr) {
        ui->lConneected->setText("Nie udało się znaleźć sterownika");
        if (widget != nullptr) {
            widget->setIsReady(false);
            widget->setError();
        }
    } else if (open && !openErr && !conf && !confErr && !sett && !settErr) {
        ui->lConneected->setText(QString("%1 - Znaleziono port. Trwa konfiguracja sterownika").arg(m_portName));
    } else if (open && openErr && !conf && !confErr && !sett && !settErr) {
        ui->lConneected->setText(QString("%1 - Znaleziono sterownik,  ale problem z otwarciem").arg(m_portName));
        if (widget != nullptr) {
            widget->setIsReady(false);
            widget->setError();
        }
    } else if (open && !openErr && conf && confErr && !sett && !settErr) {
        ui->lConneected->setText(QString("%1 - Znaleziono sterownik, problem z konfiguracją").arg(m_portName));
        if (widget != nullptr) {
            widget->setIsReady(false);
            widget->setError();
        }
    } else if (open && !openErr && conf && !confErr && sett && settErr) {
        ui->lConneected->setText(QString("%1 - Znaleziono sterownik, problem z ustawieniami").arg(m_portName));
        if (widget != nullptr) {
            widget->setIsReady(false);
            widget->setError();
        }
    } else if (open && !openErr && conf && !confErr && sett && !settErr) {
        //wszystko OK
        ui->lcz1mv->setDisabled(modeWork == WyborMetody::MODE_FUNSET);
        ui->lcz1unit->setDisabled(modeWork == WyborMetody::MODE_FUNSET);
        ui->ecz1->setDisabled(modeWork == WyborMetody::MODE_FUNSET);
        ui->eStatus->setDisabled(false);
        ui->lStatus->setDisabled(false);
        ui->lStatusMinor->setDisabled(false);
        ui->eRadio->setDisabled(modeWork == WyborMetody::MODE_FUNSET);
        ui->lradio->setDisabled(modeWork == WyborMetody::MODE_FUNSET);
        ui->eStatusRoleta->setEnabled(modeWork == WyborMetody::MODE_ROLETAP);
        ui->lStatusRoleta->setEnabled(modeWork == WyborMetody::MODE_ROLETAP);
        ui->lConneected->setText(QString("%1 - Sterownik OK").arg(m_portName));
    }
}

void MiernikPrzeplywu::setParamsDone(bool success)
{
    DEBUGMP(QString("Ustawiono parametry %1.").arg(success));

    if (!success)
        return;

    if (modeWork == WyborMetody::MODE_NONE)
        return;

    widget->setIsReady(false);
    widget->setStart();
    firstRun = true;
    firstRun2 = true;
    checkRadio = true;
    if (modeWork == WyborMetody::MODE_FUNSET) {

        widget->setStatus(QString("Zakończono konfigurację urządzenia. Ustawiam czujnik w pozycji bazowej"));
        ui->lStatus->setText(QString("Zakończono konfigurację kontrolera. Trwa zerowanie urządzenia...."));
        ui->statusbar->showMessage("Zakończono konfigurację urządzenia. Ustawiam czujnik w pozycji bazowej", 5000);
        setPositionHome();
    } else {
        widget->setStatus(QString("Zakończono konfigurację urządzenia. Sprawdzam połączenie z modułem radiowym"));
        ui->lStatus->setText(QString("Zakończono konfigurację kontrolera. Sprawdzam połączenie z modułem radiowym...."));
        ui->statusbar->showMessage("Zakończono konfigurację urządzenia. Sprawdzam połączenie z modułem radiowym", 5000);
        checkRadio = true;
        readRadio();
    }
}

void MiernikPrzeplywu::readedFromRadio(bool sucess, int val1, int, int, int)
{
    if (modeWork == WyborMetody::MODE_NONE)
        return;

    DEBUGMP(QString("Read radio %1 [%2]").arg(sucess).arg(val1));
    if (!sucess) {
        errorReadFromRadio();
        return;
    }
    if (checkRadio) {
        checkRadio = false;
        widget->setStatus(QString("Zakończono konfigurację urządzenia. Ustawiam czujnik w pozycji bazowej"));
        ui->lStatus->setText(QString("Zakończono konfigurację kontrolera. Trwa zerowanie urządzenia...."));
        ui->lradio->setText(QString("Widoczny"));
        ui->statusbar->showMessage("Udało się połączyć przez radio. Ustawiam czujnik w pozycji bazowej", 5000);
        setPositionHome();
        return;
    }
    ui->lradio->setText(QString("Komunikacja OK"));
    ui->lcz1mv->setText(QString::number(ust.getRatioCzujnik1().toDouble()*sucess, 'g', 4));
    ui->lcz1unit->setText(ust.getUnitCzujnik1());
    ui->lStatusMinor->setText("Trwa pomiar....");
    ui->statusbar->showMessage("Trwa pomiar", 1000);
    widget->readedFromRadio(ust.getRatioCzujnik1().toDouble()*val1);
}

void MiernikPrzeplywu::errorReadFromRadio()
{
    if (modeWork == WyborMetody::MODE_NONE)
        return;

    DEBUGMP(QString("errorReadFromRadio"));
    ui->lradio->setText(QString("Błąd odczytu"));
    ui->lcz1mv->setText("---");
    ui->lcz1unit->setText("---");

    if (checkRadio) {
        checkRadio = false;
        widget->setStatus(QString("Nie udało się połączyć z radio. Ustawiam czujnik w pozycji bazowej"));
        ui->lStatus->setText(QString("Nie udało się połączyć z radio. Trwa zerowanie urządzenia...."));
        ui->statusbar->showMessage("Brak łączności z radio. Ustawiam czujnik w pozycji bazowej", 5000);
        ui->lradio->setText("Brak komunikacji");
        setPositionHome();
    } else {
        ui->statusbar->showMessage("Błąd odczytu", 10000);
        widget->errorReadFromRadio();
        ui->lradio->setText("Brak komunikacji");
    }
}
void MiernikPrzeplywu::positionHome()
{
    DEBUGMP(QString("POSITION HOME %1").arg(firstRun));
    if (firstRun && modeWork != WyborMetody::MODE_FUNSET) {
        if (modeWork == WyborMetody::MODE_ROLETAP) {
            ui->lStatusRoleta->setText(QString("Ustawiam roletę w pozycji bazowej"));
            widget->setStatus(QString("Trwa zerowanie rolety"));
            ui->statusbar->showMessage("Trwa zerowanie rolety", 5000);
            firstRun2 = true;
            setRoletaHome();
        } else {
            widget->setStatus(QString("Rozpoczynam prace"));
            ui->statusbar->showMessage("Rozpoczynam prace", 5000);
            widget->setIsReady(true);
        }
        firstRun = false;
    } else {
        widget->positionDone(true);
    }
}

void MiernikPrzeplywu::roletaHome()
{
    DEBUGMP(QString("firstRun2 = %1").arg(firstRun2));
    if (firstRun2) {
        firstRun2 = false;
        widget->roletaDone(true);
        widget->setIsReady(true);
    }

}

void MiernikPrzeplywu::deviceName(const QString & portname)
{
    if (modeWork == WyborMetody::MODE_NONE)
        return;

    DEBUGMP(QString("DeviceName = %1").arg(portname));
    m_portName = portname;
    ui->lConneected->setText(QString("%1 - Znaleziono port. Trwa konfiguracja").arg(m_portName));
}

void MiernikPrzeplywu::positionStatus(SerialMessage::StatusWork work)
{
    qDebug() << __FILE__ << __LINE__ ;
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
    DEBUGMP(QString("home position %1").arg(work));
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
        positionHome();
        break;
    case SerialMessage::START_R:
        ui->lStatusRoleta->setText("Ustawianie rolety na pozycji bazowej.");
        ui->statusbar->showMessage("Ustawianie rolety na pozycji bazowej.",5000);
        break;
    case SerialMessage::END_R:
        ui->lStatusRoleta->setText("Roleta ustawiona na pozycji bazowej.");
        ui->statusbar->showMessage("Zakończono ustawianie rolety na pozycji bazowej.",5000);
        roletaHome();
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

void MiernikPrzeplywu::errorHome()
{
    DEBUGMP("ERROR HOME");
    widget->setError();
    QMessageBox::critical(this, "Pozycjonowanie bazowe", "Nie udało się ustawić pozycji bazowej dla czujnika");
}

void MiernikPrzeplywu::errorPosition()
{
    DEBUGMP("ERROR POS");
    widget->setError();
    QMessageBox::critical(this, "Pozycjonowanie", "Nie udało się ustawić pozycji dla czujnika");
}

void MiernikPrzeplywu::errorHomeRoleta()
{
    DEBUGMP("ERROR HOME ROLETA");
    widget->setError();
    QMessageBox::critical(this, "Pozycjonowanie bazowe rolety", "Nie udało się ustawić pozycji bazowej dla rolety");
}

void MiernikPrzeplywu::errorRoleta()
{
    DEBUGMP("ERROR ROLETA");
    widget->setError();
    QMessageBox::critical(this, "Pozycjonowanie rolety", "Nie udało się ustawić pozycji dla rolety");
}


void MiernikPrzeplywu::debug(const QString & dbg)
{
    //ui->debug->append(dbg);
    qDebug() << addTime(dbg);
}

const WyborMetodyData &MiernikPrzeplywu::getData() const
{
    return data;
}

void MiernikPrzeplywu::connectToDevice()
{
    DEBUGMP("connectToDevice");
    sMsg.connectToDevice();
    //emit connectAndConfigureSig();
}

void MiernikPrzeplywu::setPositionHome()
{
    sMsg.setPositionHome();
}

void MiernikPrzeplywu::setPosition(uint32_t x, uint32_t y)
{
    sMsg.setPosition(x,y);
}

void MiernikPrzeplywu::setRoletaHome()
{
    sMsg.setRoletaHome();
}

void MiernikPrzeplywu::setRoleta(uint32_t r)
{
    sMsg.setRoleta(r);
}

void MiernikPrzeplywu::setParams(bool reverseX, bool reverseY, bool reverseR, uint32_t maxImpX, uint32_t maxImpY,
                                 uint32_t maxStepX, uint32_t maxStepY, uint32_t maxStepR)
{
    sMsg.setParams(reverseX, reverseY, reverseR, maxImpX, maxImpY, maxStepX, maxStepY, maxStepR);
}

void MiernikPrzeplywu::readRadio()
{
    sMsg.readRadio();
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
    chooseWork();
}

void MiernikPrzeplywu::setClose(bool afterBase)
{
    sMsg.closeDevice(afterBase);
}


