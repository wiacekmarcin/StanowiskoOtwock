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
#include "roletaclose.h"

#include <QMessageBox>
#include <QCloseEvent>
#include <QWidget>
#include <QSerialPortInfo>

#include <QDateTime>
#include <QTime>

#include <QDebug>

//#define DEBUGMP(X) debug(QString("%1:%2 %3").arg(__FILE__).arg(__LINE__).arg(X))
#define DEBUGMP(X) 

MiernikPrzeplywu::MiernikPrzeplywu()
    : QMainWindow(NULL)
    , ui(new Ui::MiernikPrzeplywu)
    , modeWork(WyborMetody::MODE_NONE)
    , methodIns(WyborMetody::METHOD_NONE)
    , homePosStart(false)
    , homeRolStart(false)
    , checkRadio(false)
    , debugFile("przeplywomierzLog.txt")
{
    ui->setupUi(this);
    widget = nullptr;

    sMsg.setThread(&thSterownik);
    debugFile.open(QFile::WriteOnly | QFile::Truncate | QFile::Text | QFile::Unbuffered);

    setUstawienia();

    connect(&sMsg, &SerialDevice::error, this, &MiernikPrzeplywu::errorSerial, Qt::QueuedConnection);
    connect(&sMsg, &SerialDevice::debug, this, &MiernikPrzeplywu::debug, Qt::QueuedConnection);

    connect(&sMsg, &SerialDevice::deviceName, this, &MiernikPrzeplywu::deviceName, Qt::QueuedConnection);
    connect(&sMsg, &SerialDevice::kontrolerConfigured, this, &MiernikPrzeplywu::kontrolerConfigured, Qt::QueuedConnection);

    connect(&sMsg, &SerialDevice::setParamsDone, this, &MiernikPrzeplywu::setParamsDone, Qt::QueuedConnection);
    connect(&sMsg, &SerialDevice::setPositionDone, this, &MiernikPrzeplywu::setPositionDone, Qt::QueuedConnection);

    connect(&sMsg, &SerialDevice::readFromRadio, this, &MiernikPrzeplywu::readedFromRadio, Qt::QueuedConnection);

    chooseWork();
    ui->lstanowisko2700_3000->setText(QString("Stanowisko %1x%2 [mm]").arg(ust.getStacOsXNazwa(),ust.getStacOsYNazwa()));
    ui->lstanowisko1000_2000prawe->setText(QString("Stanowisko %1x%2 [mm] prawe").arg(ust.getOknoOsXNazwa(), ust.getOknoOsYNazwa()));
    ui->lStanowisko1000_2000lewe->setText(QString("Stanowisko %1x%2 [mm] lewe").arg(ust.getOknoOsXNazwa(), ust.getOknoOsYNazwa()));
    ui->lstanowisko860x1500P->setText(QString("Stanowisko z roletą %1x%2 [mm] prawe").arg(ust.getRolOsXNazwa(), ust.getRolOsYNazwa()));
    ui->lstanowisko860x1500L->setText(QString("Stanowisko z roletą %1x%2 [mm] lewe").arg(ust.getRolOsXNazwa(), ust.getRolOsYNazwa()));

    ui->debugLine->setVisible(false);
    closeRoleta = false;
    
}

MiernikPrzeplywu::~MiernikPrzeplywu()
{
    sMsg.closeDevice(false);
    thSterownik.quit();
    delete ui;
    debugFile.close();
}

void MiernikPrzeplywu::chooseWork()
{
    ////qDebig() << "chooseWork()" << modeWork << methodIns;

    do {
        ////qDebig() << "New wyborMethody";
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
        mech.setPrzestrzen(ust.getRolOsXReal().toUInt(), ust.getRolOsYReal().toUInt());
        mech.setReverseY(false);
        mech.setReverseX(modeWork == WyborMetody::MODE_ROLETAP);
        mechR.setReverse(modeWork == WyborMetody::MODE_ROLETAL);
        mech.setReverseR(true);
        break;
    case WyborMetody::MODE_2700:
    case WyborMetody::MODE_FUNSET:
        mech.setPrzestrzen(ust.getStacOsXReal().toUInt(), ust.getStacOsYReal().toUInt());
        mech.setReverseY(false);
        mech.setReverseX(false);
        mech.setReverseR(true);
        break;
    case WyborMetody::MODE_1000L:
    case WyborMetody::MODE_1000P:
        mech.setPrzestrzen(ust.getOknoOsXReal().toUInt(), ust.getOknoOsYReal().toUInt());
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
    mech.setMaxKrokiR(ust.getMaxRolKroki().toUInt());
    widget->setMechanika(mech);
    widget->setMechanikaRolety(mechR);
    sMsg.setParams(mech.getReverseX(), mech.getReverseY(), mech.getReverseR(),
                      mech.getMaxImpusyX(), mech.getMaxImpusyY(),
                      mech.getMaxKrokiX(), mech.getMaxKrokiY(),
                      mech.getMaxKrokiR(), mechR.getMinKroki(),
                      mechR.getSpeedHome(), mechR.getSpeedPos());
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
        unsigned int wymiarX;
        unsigned int wymiarY;
        if (modeWork == WyborMetody::MODE_2700) {
            wymiarX = ust.getStacOsXReal().toUInt();
            wymiarY = ust.getStacOsYReal().toUInt();
        } else {
            wymiarX = ust.getOknoOsXReal().toUInt();
            wymiarY = ust.getOknoOsYReal().toUInt();
        }
        if (methodIns == WyborMetody::METHOD_FILE) {
            WybranyPlik wp(this, values.fileName, wymiarX, wymiarY);
            int r = wp.exec();
            if (r == 0) {
                return  false;
            }
            w->setList(wp.getList());
            return true;
        } else if (methodIns == WyborMetody::METHOD_SQUERE) {
            WyborKwadratow wk(this, values.numberWidth, values.numberHeight, values.timeStopAuto, 
                                        wymiarX, wymiarY);
            int r = wk.exec();
            if (r == 0) {
                return false;
            }
            w->setList(wk.getList());
            return true;
        } else if (methodIns == WyborMetody::METHOD_MANUAL) {
            ReczneDodPozycji rdp(this, values.numberPozMan, values.timeStopManual, wymiarX, wymiarY);
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
        unsigned int wymiarX;
        unsigned int wymiarY;
        wymiarX = ust.getRolOsXReal().toUInt();
        wymiarY = ust.getRolOsYReal().toUInt();
        unsigned int maxH = ust.getNormaRoletaHeight().toUInt();
        unsigned int maxW = ust.getNormaRoletaWidth().toUInt();

        if (methodIns == WyborMetody::METHOD_MANUAL) {
            //TODO

            PodzialEtapuRolety pdr(this, reverse, values.etapNrRoleta,
                                   values.timeStopRoleta,
                                  values.stableTimeRoleta, maxH, maxW,
                                  reverse ? values.offsetXL : values.offsetXP,
                                  reverse ? values.offsetYL : values.offsetYP);
            int r = pdr.exec();
            if (r == 0) {
                return false;
            }
            w->setList(pdr.getLista());
            return true;
        } else if (methodIns == WyborMetody::METHOD_FILE) {
            ////qDebig() << "alloha" << values.stableTimeRoleta;
            WybranyPlikNorma wpn(this, false, values.fileName2, values.etapNrRoleta, values.stableTimeRoleta,
                                 wymiarY, wymiarX,
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
    return QString("[%1] %2\n").arg(QTime::currentTime().toString("HH:mm:ss.zzz")).arg(status.remove("C:\\Devel\\StanowiskoOtwock\\PrzeplywomierzGUI\\Aplikacja\\"));
}

void MiernikPrzeplywu::debugClear()
{
    
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
}

void MiernikPrzeplywu::errorSerial(QString error)
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


void MiernikPrzeplywu::enableStatusWidget(bool enable)
{
    if (!enable) {
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
    }
}

void MiernikPrzeplywu::kontrolerConfigured(bool success, int state)
{
    if (modeWork == WyborMetody::MODE_NONE)
        return;

    if (closeRoleta)
        return;

    DEBUGMP(QString("Open device open = %1, state = %2 ").arg(success).arg(state));

    //if (widget != nullptr) {
    //    widget->setIsReady(success);
    //}

    switch(state) {

    case SerialDevice::NO_FOUND:
        ui->lConneected->setText("Nie znaleziono kontrolera");
        enableStatusWidget(false);
        if (widget != nullptr) {
            widget->setError();
        }
        break;

    case SerialDevice::FOUND:
        ui->lConneected->setText(QString("Znaleziono kontroler. Otwieram port %1 ....").arg(m_portName));
        break;

    case SerialDevice::NO_OPEN:
        ui->lConneected->setText(QString("%1 - kontroler. Nie udało się otworzyć").arg(m_portName));
        enableStatusWidget(false);
        if (widget != nullptr) {
            widget->setError();
        }
        break;

    case SerialDevice::OPEN:
        ui->lConneected->setText(QString("%1 - kontroler. Sprawdzam kontroler...").arg(m_portName));
        break;

    case SerialDevice::NO_READ:
        ui->lConneected->setText(QString("%1 - kontroler. Problem z odczytem z portu").arg(m_portName));
        enableStatusWidget(false);
        if (widget != nullptr) {
            widget->setError();
        }
        break;

    case SerialDevice::IDENT_FAILD:
        ui->lConneected->setText(QString("%1 - kontroler. Nieprawidłowy kontroler.").arg(m_portName));
        enableStatusWidget(false);
        if (widget != nullptr) {
            widget->setError();
        }
        break;

    case SerialDevice::IDENT_OK:
        ui->lConneected->setText(QString("%1 - kontroler. Trwa konfiguracja...").arg(m_portName));
        break;

    case SerialDevice::PARAMS_FAILD:
        ui->lConneected->setText(QString("%1 - kontroler. Nie skonfigurowano.").arg(m_portName));
        enableStatusWidget(false);
        if (widget != nullptr) {
            widget->setError();
        }
        break;

    case SerialDevice::PARAMS_OK:
        ui->lConneected->setText(QString("%1 - kontroler. Skonfigurowano.").arg(m_portName));
        break;

    case SerialDevice::ALL_OK:
        ui->lcz1mv->setDisabled(modeWork == WyborMetody::MODE_FUNSET);
        ui->lcz1unit->setDisabled(modeWork == WyborMetody::MODE_FUNSET);
        ui->lcz1mv->setText("--");
        ui->lcz1unit->setText("--");
        ui->ecz1->setDisabled(modeWork == WyborMetody::MODE_FUNSET);
        ui->eStatus->setDisabled(false);
        ui->lStatus->setDisabled(false);
        ui->lStatusMinor->setDisabled(false);
        ui->eRadio->setDisabled(modeWork == WyborMetody::MODE_FUNSET);
        ui->lradio->setDisabled(modeWork == WyborMetody::MODE_FUNSET);
        ui->eStatusRoleta->setEnabled(modeWork == WyborMetody::MODE_ROLETAP);
        ui->lStatusRoleta->setEnabled(modeWork == WyborMetody::MODE_ROLETAP);
        ui->lConneected->setText(QString("%1 - kontroler OK.").arg(m_portName));
        if (modeWork == WyborMetody::MODE_FUNSET) {
            if (widget) {
                Wentylator * w = static_cast<Wentylator*>(widget);
                w->addStatus("Sterownik skonfigurowany");
            }
        }
        setParamsDone(true);
        break;

    case SerialDevice::CLOSE:
        ui->lConneected->setText("Kontroler rozłączony");
        enableStatusWidget(false);
        if (widget != nullptr) {
            widget->setIsReady(false);
        }
        break;

    default:
        break;
    }
}

void MiernikPrzeplywu::setParamsDone(bool success)
{
    DEBUGMP(QString("Ustawiono parametry %1.").arg(success));

    if (!success)
        return;

    if (modeWork == WyborMetody::MODE_NONE)
        return;

    if (closeRoleta)
        return;

    homePosStart = true;
    homeRolStart = true;
    checkRadio = true;

    if (modeWork == WyborMetody::MODE_FUNSET) {

        widget->setStatus(QString("Ustawiam czujnik w pozycji bazowej"));
        ui->lStatus->setText(QString("Trwa zerowanie urządzenia...."));
        ui->statusbar->showMessage("Ustawiam czujnik w pozycji bazowej", 5000);
        Wentylator * w = static_cast<Wentylator*>(widget);
        w->addStatus("Zeruje pozycje lasera");
        setPositionHome();
    } else {
        widget->setStatus(QString("Sprawdzam połączenie z modułem radiowym"));
        ui->lStatus->setText(QString("Sprawdzam połączenie z modułem radiowym...."));
        ui->statusbar->showMessage("Sprawdzam połączenie z modułem radiowym", 5000);
        readRadio();
    }
}

void MiernikPrzeplywu::readedFromRadio(bool sucess, int val1, int, int, int)
{
    DEBUGMP(QString("Read radio succ=%1 val=[%2]").arg(sucess).arg(val1));

    if (modeWork == WyborMetody::MODE_NONE || modeWork == WyborMetody::MODE_FUNSET)
        return;

    if (checkRadio) {
        checkRadio = false;
        if (widget) {
            widget->setStatus(QString("Odczytano radio"));
        }
        ui->lStatus->setText(QString("Odczytano radio"));
        ui->statusbar->showMessage(sucess ? QString("Udało się połączyć przez radio.") :
            QString("Brak łączności z radio."), 5000);
        ui->lradio->setText(sucess ? QString("Komunikacja OK") : QString("Brak komunikacji"));

        if (sucess) {
            ui->lcz1mv->setText(QString::number(ust.getRatioCzujnik1().toDouble() * sucess, 'g', 4));
            ui->lcz1unit->setText(ust.getUnitCzujnik1());
        }
    }

    if (homePosStart) {
        if (widget) {
            widget->setStatus(QString("Trwa zerowanie..."));
        }
        ui->lStatus->setText(QString("Trwa zerowanie..."));
        ui->statusbar->showMessage(QString("Ustawiam pozycję startową"), 5000);
        setPositionHome();
        return;
    }

    ui->lradio->setText(sucess ? QString("Komunikacja OK") : QString("Brak komunikacji"));
    if (!sucess) {
        errorReadFromRadio();
        return;
    }
    else {
        ui->lStatusMinor->setText("Trwa pomiar....");
        ui->statusbar->showMessage("Trwa pomiar", 1000);
        if (widget)
            widget->readedFromRadio(ust.getRatioCzujnik1().toDouble() * val1);
    }
}

void MiernikPrzeplywu::errorReadFromRadio()
{
    DEBUGMP(QString("errorReadFromRadio"));

    if (modeWork == WyborMetody::MODE_NONE || modeWork == WyborMetody::MODE_FUNSET)
        return;

    //ui->lradio->setText("Brak komunikacji");
    ui->lcz1mv->setText("---");
    ui->lcz1unit->setText("---");

    
    ui->statusbar->showMessage("Błąd odczytu", 10000);
    if (widget)
        widget->errorReadFromRadio();
    
    checkRadio = true;
    
}

void MiernikPrzeplywu::setRadioVal(const QString val)
{
    ui->lcz1mv->setText(val);
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

void MiernikPrzeplywu::positionHome()
{
    DEBUGMP(QString("POSITION HOME %1").arg(homePosStart));

    if (modeWork == WyborMetody::MODE_NONE)
        return;

    if (homePosStart) {
        homePosStart = false;
        if (modeWork == WyborMetody::MODE_FUNSET) {
            if (widget) {
                Wentylator* w = static_cast<Wentylator*>(widget);
                w->addStatus("Urządzenie wyzerowane.");
                w->setIsReady(true);
                w->positionDone(true);
            }
            ui->statusbar->showMessage("Urządzenie gotowe do pracy", 5000);
        }
        else if (modeWork == WyborMetody::MODE_ROLETAP || modeWork == WyborMetody::MODE_ROLETAL)
        {
            ui->lStatusRoleta->setText(QString("Ustawiam roletę w pozycji bazowej"));
            ui->lStatus->setText(QString("Trwa zamykanie rolety"));
            if (widget)
                widget->setStatus(QString("Trwa zerowanie rolety"));

            homeRolStart = true;
            setRoletaHome();
        }
        else if (modeWork == WyborMetody::MODE_1000L || modeWork == WyborMetody::MODE_1000P || modeWork == WyborMetody::MODE_2700)
        {
            ui->lStatus->setText(QString("Rozpoczynam pracę"));
            if (widget) {
                widget->setStatus(QString("Rozpoczynam pracę"));
                widget->setIsReady(true);
                widget->setStart();
            }
        }
    }
    else {
        if (widget)
            widget->positionDone(true);
    }
}

void MiernikPrzeplywu::roletaHome()
{
    DEBUGMP(QString("homeRolStart = %1").arg(homeRolStart));

    if (modeWork == WyborMetody::MODE_NONE || modeWork == WyborMetody::MODE_1000L || modeWork == WyborMetody::MODE_1000P || modeWork == WyborMetody::MODE_2700
        || modeWork == WyborMetody::MODE_FUNSET)
        return;

    if (homeRolStart) {
        homeRolStart = false;
        if (modeWork == WyborMetody::MODE_ROLETAL || modeWork == WyborMetody::MODE_ROLETAP) {
            ui->lStatus->setText(QString("Rozpoczynam pracę"));
            if (widget) {
                widget->setStatus(QString("Rozpoczynam pracę"));
                widget->setIsReady(true);
                widget->setStart();
            }
        }
    }
    else {
        if (widget)
            widget->roletaDone(true);
    }
    ui->lStatusRoleta->setText(QString("Roleta zamknięta"));
}

void MiernikPrzeplywu::positionStatus(SerialMessage::StatusWork work)
{
    switch(work) {
    case SerialMessage::START_XY:
        ui->lStatus->setText("Ustawianie pozycji czujnika...");
        ui->lStatusMinor->setText("--");
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
        if (widget)
            widget->positionDone(false);
        break;
    case SerialMessage::START_R:
        ui->lStatusRoleta->setText("Podnoszenie rolety...");
        ui->statusbar->showMessage("Podnoszenie rolety do pozycji.",5000);
        break;
    case SerialMessage::END_R:
        ui->lStatusRoleta->setText("Roleta ustawiona.");
        ui->statusbar->showMessage("Zakończono ustawianie rolety na pozycji.",5000);
        if (widget)
            widget->roletaDone(false);
        break;
    case SerialMessage::ERROR_XY:
        ui->lStatusMinor->setText("Błąd pozycjonowania czujnika");
        QMessageBox::critical(this, "Pozycjonowanie czujnika", "Nie udało się ustawić czujnika w pozycji zadanej.");
        if (widget)
            widget->setError();
        return;
    case SerialMessage::ERROR_R:
        ui->lStatusRoleta->setText("Błąd podnoszenia rolety");
        QMessageBox::critical(this, "Pozycjonowanie rolety", "Nie udało się ustawić rolety w pozycji zadanej.");
        if (widget)
            widget->setError();
        return;
    }
    if (widget)
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
        if (widget)
            widget->setError();
        return;
    case SerialMessage::ERROR_R:
        QMessageBox::critical(this, "Pozycjonowanie rolety", "Nie udało się ustawić rolety w pozycji bazowej");
        if (widget)
            widget->setError();
        return;
    }
    if (widget)
        widget->positionStatus(true, work);
}

void MiernikPrzeplywu::errorHome()
{
    DEBUGMP("ERROR HOME");
    if (widget)
        widget->setError();
    QMessageBox::critical(this, "Pozycjonowanie bazowe", "Nie udało się ustawić pozycji bazowej dla czujnika");
}

void MiernikPrzeplywu::errorPosition()
{
    DEBUGMP("ERROR POS");
    if (widget)
        widget->setError();
    QMessageBox::critical(this, "Pozycjonowanie", "Nie udało się ustawić pozycji dla czujnika");
}

void MiernikPrzeplywu::errorHomeRoleta()
{
    DEBUGMP("ERROR HOME ROLETA");
    if (widget)
        widget->setError();
    QMessageBox::critical(this, "Pozycjonowanie bazowe rolety", "Nie udało się ustawić pozycji bazowej dla rolety");
}

void MiernikPrzeplywu::errorRoleta()
{
    DEBUGMP("ERROR ROLETA");
    if (widget)
        widget->setError();
    QMessageBox::critical(this, "Pozycjonowanie rolety", "Nie udało się ustawić pozycji dla rolety");
}

void MiernikPrzeplywu::deviceName(const QString & portname)
{
    DEBUGMP(QString("DeviceName = %1").arg(portname));
    m_portName = portname;
}

void MiernikPrzeplywu::debug(const QString & dbg)
{
    (void)dbg;
    debugFile.write(addTime(dbg).toUtf8());
    //ui->debugLine->append(addTime(dbg));
    //qDebug() << addTime(dbg);
}

const WyborMetodyData &MiernikPrzeplywu::getData() const
{
    return data;
}

void MiernikPrzeplywu::connectToDevice()
{
    if (widget)
        widget->setIsReady(false);
    DEBUGMP("connectToDevice");
    sMsg.connectToDevice();
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
                                 uint32_t maxStepX, uint32_t maxStepY, uint32_t maxStepR, int16_t minStepR,
                                 uint16_t speedRolHome, uint16_t speedRolPos)
{
    sMsg.setParams(reverseX, reverseY, reverseR, maxImpX, maxImpY, maxStepX, maxStepY, maxStepR,
                    minStepR, speedRolHome, speedRolPos);
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

void MiernikPrzeplywu::setClose(bool waitForDone)
{
    sMsg.closeDevice(waitForDone);
}

void MiernikPrzeplywu::setRoletaClose()
{
    closeRoleta = true;
    RoletaClose* dlg = new RoletaClose(mech, mechR, &sMsg, RoletaClose::ROL_CLOSE, this);
    dlg->exec();
    delete dlg;
    closeRoleta = false;
}