#include "miernikprzeplywu.h"
#include "ui_miernikprzeplywu.h"

#include "wyborkwadratow.h"
#include "recznedodpozycji.h"
#include "wybranyplik.h"
#include "ustawienia.h"
#include "mierzonepozycje.h"
#include <QMessageBox>
#include <QCloseEvent>
#include <QWidget>
#include <QSerialPortInfo>

#include <QDateTime>
#include <QTime>

#include <QDebug>
MiernikPrzeplywu::MiernikPrzeplywu(QWidget *parent, WyborMetody::ModeWork mode,
                                   WyborMetody::MethodInsData method,
                                   const QString & _filename , unsigned int _manPos, unsigned _manTimeStop,
                                   unsigned int _widthPoz, unsigned int _heightPoz, unsigned int _autoTimeStop)
    : QMainWindow(parent)
    , ui(new Ui::MiernikPrzeplywu)
    , modeWork(mode)
    , methodIns(method)
    , manTimeStop(_manTimeStop)
    , manPos(_manPos)
    , autoTimeStop(_autoTimeStop)
    , widthPos(_widthPoz)
    , heightPos(_heightPoz)
    , fileName(_filename)
    , ust(nullptr)
    , connIsOk(false)
    , impx(0)
    , impy(0)
    , lockTab(false)
{
    ui->setupUi(this);
    widget = nullptr;

    widget2700 = new MierzonePozycje(ui->tab_2700);
    widget2700->setObjectName(QString::fromUtf8("mierzonepozycje2700"));
    ui->verticalLaoutTab2700->addWidget(widget2700);

    widget1000l = new MierzonePozycje(ui->tab_1000l);
    widget1000l->setObjectName(QString::fromUtf8("mierzonepozycje1000L"));
    ui->verticallayoutTab1000l->addWidget(widget1000l);

    widget1000p = new MierzonePozycje(ui->tab_1000l);
    widget1000p->setObjectName(QString::fromUtf8("mierzonepozycje1000P"));
    ui->verticallayoutTab1000p->addWidget(widget1000p);

    ui->lConneected->setText(QString::fromUtf8("Nie znaleziono"));

    proceedChooseMethod();

    ui->errorWentylatorX->setVisible(false);
    ui->errorWentylatorY->setVisible(false);
    ui->errorWentylatorL->setVisible(false);



    connect(&sMsg, SIGNAL(errorSerial(QString)), this, SLOT(errorSerial(QString)));
    connect(&sMsg, SIGNAL(debug(QString)), this, SLOT(debug(QString)));


    connect(&sMsg, SIGNAL(deviceName(QString)), this, SLOT(deviceName(QString)));
    connect(&sMsg, SIGNAL(controllerOK()), this, SLOT(controllerOK()));
    connect(&sMsg, SIGNAL(successOpenDevice(bool)), this, SLOT(successOpenDevice(bool)));

    connect(&sMsg, SIGNAL(setParamsDone()), this, SLOT(setParamsDone()));

    connect(&sMsg, SIGNAL(readedFromRadio(int)), this, SLOT(readedFromRadio(int)));
    connect(&sMsg, SIGNAL(errorReadFromRadio()), this, SLOT(errorReadFromRadio()));

    connect(this, SIGNAL(connectToDevice()), &sMsg, SLOT(connectToSerial()));
    connect(this, SIGNAL(checkDevice()), &sMsg, SLOT(checkController()));
    connect(this, SIGNAL(setPositionHome()), &sMsg, SLOT(setPositionHome()));
    connect(this, SIGNAL(setPosition(uint32_t,uint32_t)), &sMsg, SLOT(setPosition(uint32_t,uint32_t)));

    connect(this, SIGNAL(setParams(bool,bool,uint32_t,uint32_t,uint32_t,uint32_t)), &sMsg,
            SLOT(setParams(bool,bool,uint32_t,uint32_t,uint32_t,uint32_t)));

    //connect(&sMsg, SIGNAL(startingHome()), this, SLOT(startingHome()));
    //connect(&sMsg, SIGNAL(startingHomeX()), this, SLOT(startingHomeX()));
    //connect(&sMsg, SIGNAL(doneHomeX()), this, SLOT(doneHomeX()));
    //connect(&sMsg, SIGNAL(startingHomeY()), this, SLOT(startingHomeY()));
    //connect(&sMsg, SIGNAL(doneHomeY()), this, SLOT(doneHomeY()));
    connect(&sMsg, SIGNAL(doneHome()), this, SLOT(doneHome()));
    connect(&sMsg, SIGNAL(errorHome()), this, SLOT(errorHome()));

    //connect(&sMsg, SIGNAL(startingPosition()), this, SLOT(startingPosition()));
    //connect(&sMsg, SIGNAL(startingPositionX()), this, SLOT(startingPositionX()));
    //connect(&sMsg, SIGNAL(donePositionX()), this, SLOT(donePositionX()));
    //connect(&sMsg, SIGNAL(startingPositionY()), this, SLOT(startingPositionY()));
    //connect(&sMsg, SIGNAL(donePositionY()), this, SLOT(donePositionY()));
    connect(&sMsg, SIGNAL(donePosition()), this, SLOT(donePosition()));


}

MiernikPrzeplywu::~MiernikPrzeplywu()
{
    delete ui;
}

void MiernikPrzeplywu::chooseTab()
{
    if (modeWork == WyborMetody::MODE_FUNSET) {
        ui->tabWidget->setCurrentIndex(3);
    }
    else if (modeWork == WyborMetody::MODE_SERVICE) {
        ui->tabWidget->setCurrentIndex(4);
    }
    else if (modeWork == WyborMetody::MODE_2700) {
        ui->tabWidget->setCurrentIndex(0);
        widget = widget2700;
    }
    else if (modeWork == WyborMetody::MODE_1000L) {
        ui->tabWidget->setCurrentIndex(2);
        widget = widget1000l;
    }

    else if (modeWork == WyborMetody::MODE_1000P) {
        ui->tabWidget->setCurrentIndex(1);
        widget = widget1000p;
    }

    if (modeWork != WyborMetody::MODE_FUNSET) {
        connect(widget, SIGNAL(doConnect()), this, SIGNAL(connectToDevice()));
        connect(widget, SIGNAL(checkDevice()), this, SIGNAL(checkDevicec()));
        connect(widget, SIGNAL(setPosition(uint32_t,uint32_t)), this, SIGNAL(setPosition(uint32_t,uint32_t)));
        connect(widget, SIGNAL(readRadio()), &sMsg, SLOT(readValueFromRadio()));
        connect(&sMsg, SIGNAL(readedFromRadio(int)), widget, SLOT(readedFromRadio(int)));
        connect(widget, SIGNAL(statusMiernik(QString)), this, SLOT(statusMiernik(QString)));
        connect(widget, SIGNAL(end()), this, SLOT(end()));
        connect(widget, SIGNAL(start()), this, SLOT(start()));
        connect(widget, SIGNAL(noweDane()), this, SLOT(noweDane()));
    }
}

void MiernikPrzeplywu::chooseMethod()
{
    if (modeWork == WyborMetody::MODE_FUNSET || modeWork == WyborMetody::MODE_SERVICE) {
        return;
    }
    if (methodIns == WyborMetody::METHOD_FILE) {
        WybranyPlik wp(this, fileName, modeWork == WyborMetody::MODE_2700 ? 2700 : 1000,
                                       modeWork == WyborMetody::MODE_2700 ? 3000 : 2000);
        int r = wp.exec();
        if (r == 0) {
            WyborMetody wb(nullptr, modeWork);
            int w = wb.exec();
            if (w == 1){
                modeWork = wb.getWbMode();
                methodIns = wb.getWbMetoda();
                manTimeStop = wb.getTimeStopManual();;
                manPos = wb.getNumberPozMan();
                autoTimeStop = wb.getTimeStopAuto();
                widthPos = wb.getNumberWidth();
                heightPos = wb.getNumberHeight();
                fileName = wb.getFileName();
                proceedChooseMethod();
                return;
            }
        } else {
            m_lista = wp.getList();
            return;
        }
    } else if (methodIns == WyborMetody::METHOD_SQUERE) {
        WyborKwadratow w(this, widthPos, heightPos, autoTimeStop, modeWork == WyborMetody::MODE_2700 ? 2700 : 1000,
                                                               modeWork == WyborMetody::MODE_2700 ? 3000 : 2000);
        int r = w.exec();
        if (r == 0) {
            WyborMetody wb(nullptr, modeWork);
            int w = wb.exec();
            if (w == 1){
                modeWork = wb.getWbMode();
                methodIns = wb.getWbMetoda();
                manTimeStop = wb.getTimeStopManual();
                manPos = wb.getNumberPozMan();
                autoTimeStop = wb.getTimeStopAuto();
                widthPos = wb.getNumberWidth();
                heightPos = wb.getNumberHeight();
                fileName = wb.getFileName();
                proceedChooseMethod();
                return;
            }
        } else {
            m_lista = w.getList();
            return;
        }
    }
    else if (methodIns == WyborMetody::METHOD_MANUAL) {
        ReczneDodPozycji rdp(this, manPos, manTimeStop, modeWork == WyborMetody::MODE_2700 ? 2700 : 1000,
                                                        modeWork == WyborMetody::MODE_2700 ? 3000 : 2000);
        int r = rdp.exec();
        if (r == 0) {
            WyborMetody wb(nullptr, modeWork);
            int w = wb.exec();
            if (w == 1) {
                modeWork = wb.getWbMode();
                methodIns = wb.getWbMetoda();
                manTimeStop = wb.getTimeStopManual();
                manPos = wb.getNumberPozMan();
                autoTimeStop = wb.getTimeStopAuto();
                widthPos = wb.getNumberWidth();
                heightPos = wb.getNumberHeight();
                fileName = wb.getFileName();
                proceedChooseMethod();
                return;
            }
        } else {
            m_lista = rdp.getList();
            return;
        }
    }
}

void MiernikPrzeplywu::proceedChooseMethod()
{
    chooseTab();
    chooseMethod();
    calculateMechanika();
    if (widget) {
        widget->setList(m_lista);
        widget->setMechanika(mech);

    }
}

void MiernikPrzeplywu::calculateMechanika()
{
    mech.setPrzestrzen((modeWork == WyborMetody::MODE_2700 || modeWork == WyborMetody::MODE_FUNSET ) ? 2700 : 1000,
                       (modeWork == WyborMetody::MODE_2700 || modeWork == WyborMetody::MODE_FUNSET ) ? 3000 : 2000);

    mech.setReverseY(false);
    mech.setReverseX(modeWork == WyborMetody::MODE_1000P);
}

QString MiernikPrzeplywu::addTime(QString status)
{
    return QString("[%1] %2").arg(QTime::currentTime().toString("HH:mm:ss.zzz")).arg(status);
}

void MiernikPrzeplywu::debugclear()
{
    ui->debug->clear();
}


void MiernikPrzeplywu::on_tabWidget_currentChanged(int index)
{
    int mapMode[] = {WyborMetody::MODE_2700, WyborMetody::MODE_1000P, WyborMetody::MODE_1000L, WyborMetody::MODE_FUNSET,
                     WyborMetody::MODE_SERVICE};
    WyborMetody::ModeWork modeTab[] = {WyborMetody::MODE_2700, WyborMetody::MODE_1000P, WyborMetody::MODE_1000L,
                                       WyborMetody::MODE_FUNSET, WyborMetody::MODE_SERVICE};

    if (lockTab) {
        ui->tabWidget->setCurrentIndex(modeWork);
        return;
    }

    if (modeWork == mapMode[index])
        return;


    WyborMetody wb(this, modeTab[index]);
    int r = wb.exec();
    if (r == 1) {
        modeWork = wb.getWbMode();
        methodIns = wb.getWbMetoda();
        manTimeStop = wb.getTimeStopManual();
        manPos = wb.getNumberPozMan();
        autoTimeStop = wb.getTimeStopAuto();
        widthPos = wb.getNumberWidth();
        heightPos = wb.getNumberHeight();
        fileName = wb.getFileName();
        proceedChooseMethod();
    } else {
        // nie zmieniam zakladki
        chooseTab();
    }
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

void MiernikPrzeplywu::setUstawienia(Ustawienia *u)
{
    ust = u;
    auto val1 = ust->getImpulsyXperMM();
    if (!val1.isEmpty())
        mech.setImpusyXPerMM(val1.toUInt());

    auto val2 = ust->getImpulsyYperMM();
    if (!val2.isEmpty())
        mech.setImpusyYPerMM(val2.toUInt());

    auto val3 = ust->getKrokiXperMM();
    if (!val3.isEmpty())
        mech.setKrokiXPerMM(val3.toUInt());

    auto val4 = ust->getKrokiYperMM();
    if (!val4.isEmpty())
        mech.setKrokiYPerMM(val4.toUInt());

    auto val5 = ust->getOffsetX();
    if (!val5.isEmpty())
        mech.setWentOffsetX(val5.toDouble());

    auto val6 = ust->getOffsetY();
    if (!val6.isEmpty())
        mech.setWentOffsetY(val6.toDouble());

    auto val7 = ust->getKatnachylenia();
    if (!val7.isEmpty())
        mech.setWentKatNach(val7.toDouble());

    mech.calculate();
}

void MiernikPrzeplywu::on_pbUstaw_clicked()
{

    QString sx = ui->leX->text();
    QString sy = ui->leY->text();
    QString sl = ui->leL->text();
    int x = 0, y = 0, l = 0;
    bool ret = true;
    ui->errorWentylatorX->setVisible(false);
    ui->errorWentylatorY->setVisible(false);
    ui->errorWentylatorL->setVisible(false);
    if (sx.isEmpty()) {
        ui->errorWentylatorX->setVisible(true);
        ui->errorWentylatorX->setText(QString::fromUtf8("Wartość nie może być pusta"));
        ret = false;
    } else {
        bool ok;
        x = sx.toInt(&ok);
        if (!ok) {
            ui->errorWentylatorX->setVisible(true);
            ui->errorWentylatorX->setText(QString::fromUtf8("Wartość musi być liczbą"));
            ret = false;
        } else {
            if (x < 0) {
                ui->errorWentylatorX->setVisible(true);
                ui->errorWentylatorX->setText(QString::fromUtf8("Wartość musi być liczbą wiekszą od zera"));
                ret = false;
            } else if ((unsigned int)x > mech.getMaxXmm()) {
                ui->errorWentylatorX->setVisible(true);
                ui->errorWentylatorX->setText(QString::fromUtf8("Wartość musi być liczbą mniejszą %1").arg(mech.getMaxXmm()));
                ret = false;
            }
        }
    }

    if (sy.isEmpty()) {
        ui->errorWentylatorY->setVisible(true);
        ui->errorWentylatorY->setText(QString::fromUtf8("Wartość nie może być pusta"));
        ret = false;
    } else {
        bool ok;
        y = sy.toInt(&ok);
        if (!ok) {
            ui->errorWentylatorY->setVisible(true);
            ui->errorWentylatorY->setText(QString::fromUtf8("Wartość musi być liczbą"));
            ret = false;
        } else {
            if (y < 0) {
                ui->errorWentylatorY->setVisible(true);
                ui->errorWentylatorY->setText(QString::fromUtf8("Wartość musi być liczbą wiekszą od zera"));
                ret = false;
            //} else if ((unsigned int)y > mech.getMaxYmm()) {
            //    ui->errorWentylatorY->setVisible(true);
            //    ui->errorWentylatorY->setText(QString::fromUtf8("Wartość musi być liczbą mniejszą od %1").arg(mech.getMaxYmm()));
            //    ret = false;
            }
        }
    }

    if (sl.isEmpty()) {
        ui->errorWentylatorL->setVisible(true);
        ui->errorWentylatorL->setText(QString::fromUtf8("Wartość nie może być pusta"));
        ret = false;
    } else {
        bool ok;
        l = sl.toInt(&ok);
        if (!ok) {
            ui->errorWentylatorY->setVisible(true);
            ui->errorWentylatorY->setText(QString::fromUtf8("Wartość musi być liczbą"));
            ret = false;
        } else {
            if (l < 0) {
                ui->errorWentylatorY->setVisible(true);
                ui->errorWentylatorY->setText(QString::fromUtf8("Wartość musi być liczbą wiekszą od zera"));
                ret = false;
            } else if ((unsigned int)l > 5000) {
                ui->errorWentylatorY->setVisible(true);
                ui->errorWentylatorY->setText(QString::fromUtf8("Wartość musi być liczbą mniejszą od 5000"));
                ret = false;
            }
        }
    }

    impmmx = mech.getWentX(x, l);
    impmmy = mech.getWentY(y);

    //impmmx = x;
    //impmmy = y;

    if (impmmx < 0) {
        ui->errorWentylatorX->setText(QString("Wyliczona wartość %1 mniejsza od zera").arg(impmmx));
        ui->errorWentylatorX->setVisible(true);
        ret = false;
    }

    if ((unsigned int)impmmx > mech.getMaxXmm()) {
        ui->errorWentylatorX->setText(QString("Wyliczona wartość %1 jest większa niż %2").arg(impmmx).arg(mech.getMaxXmm()));
        ui->errorWentylatorX->setVisible(true);
        ret = false;
    }

    if (impmmy < 0) {
        ui->errorWentylatorY->setText(QString("Wyliczona wartość %1 mniejsza od zera").arg(impmmy));
        ui->errorWentylatorY->setVisible(true);
        ret = false;
    }

    if ((unsigned int)impmmy > mech.getMaxYmm()) {
        ui->errorWentylatorY->setText(QString("Wyliczona wartość %1 jest większa niż %2").arg(impmmy).arg(mech.getMaxYmm()));
        ui->errorWentylatorY->setVisible(true);
        ret = false;
    }

    if (ret) {
        impx = mech.getImpulsyX(impmmx);
        impy = mech.getImpulsyY(impmmy);

        ui->pbUstaw->setEnabled(false);
        debugclear();
        if (!connIsOk) {
            ui->statusWentylator->clear();
            ui->statusWentylator->append(addTime("Szukam urządzenia."));
            ui->lStatusWiatrak->setText(QString("Szukam urzadzenia ...."));
            emit connectToDevice();
        } else {
            //ui->statusWentylator->append(addTime("Kalibruje urządzenie."));
            //emit setPositionHome();
            ui->lStatusWiatrak->setText(QString("Ustawiam pozycje %1-%2 mm ....").arg(impmmx).arg(impmmx));
            emit setPosition(impx, impy);



        }

    } else {
        QPalette palette = ui->errorWentylatorX->palette();
        palette.setColor(QPalette::WindowText, Qt::red);
        ui->errorWentylatorX->setPalette(palette);

        palette = ui->errorWentylatorY->palette();
        palette.setColor(QPalette::WindowText, Qt::red);
        ui->errorWentylatorY->setPalette(palette);

        palette = ui->errorWentylatorL->palette();
        palette.setColor(QPalette::WindowText, Qt::red);
        ui->errorWentylatorL->setPalette(palette);
    }
}


void MiernikPrzeplywu::errorSerial(QString error)
{
    sMsg.closeDevice();
    connIsOk = false;
    ui->statusserial->setText(QString("Błąd : %1").arg(error));
    if (modeWork == WyborMetody::MODE_FUNSET) {
        ui->statusWentylator->append(addTime("Nie udało się połączyć z urządzeniem. Błąd : %1").arg(error));
        ui->pbUstaw->setEnabled(true);
        ui->lStatusWiatrak->setText("Błąd");
    } else {
        widget->status("Nie udało się połączyć z urządzeniem.");
        widget->setConnected(false);
    }
}

void MiernikPrzeplywu::successOpenDevice(bool open)
{
    if (open) {
        ui->statusserial->setText("Sprawdzam sterownik...");
        if (modeWork == WyborMetody::MODE_FUNSET) {
            ui->statusWentylator->append(addTime("Sprawdzam sterownik."));
        } else {
            widget->status("Sprawdzam sterownik.");
        }
        emit checkDevice();

    } else {
        ui->lConneected->setText("Nie udalo sie otworzyc portu");
        sMsg.closeDevice();
        connIsOk = false;
        if (modeWork == WyborMetody::MODE_FUNSET) {
            ui->statusWentylator->append(addTime("Nie udało się połączyć z urządzeniem."));
        }
    }


}

void MiernikPrzeplywu::readedFromRadio(int value)
{
    ui->lradio->setText(QString("Widoczny"));
    ui->lcz1mv->setText(QString::number(ust->getRatioCzujnik1().toDouble()*value, 'g', 4));
    ui->lcz1unit->setText(ust->getUnitCzujnik1());
}

void MiernikPrzeplywu::errorReadFromRadio()
{
    ui->lradio->setText(QString("Błąd odczytu"));
    ui->lcz1mv->setText("---");
    ui->lcz1unit->setText("---");
}

void MiernikPrzeplywu::deviceName(QString portname)
{
    ui->lConneected->setText(portname);
}

void MiernikPrzeplywu::controllerOK()
{
    ui->statusserial->setText("Sterownik OK");

    if (modeWork == WyborMetody::MODE_FUNSET) {
        ui->statusWentylator->append(addTime("Sterownik OK."));
        ui->statusWentylator->append(addTime("Wysyłam konfigurację."));
    } else {
        widget->status("Wysyłam konfigurację.");
        widget->setConnected(true);
    }

    emit setParams(mech.getReverseX(),mech.getReverseY(),
                   mech.getMaxImpusyX(), mech.getMaxImpusyY(),
                   mech.getMaxKrokiX(), mech.getMaxKrokiY());
}

void MiernikPrzeplywu::setParamsDone()
{
    if (modeWork == WyborMetody::MODE_FUNSET) {
        ui->statusWentylator->append(addTime("Konfiguracja ustawiona."));
        ui->statusWentylator->append(addTime("Rozpoczęcie kalibracja urządzenia."));
        connIsOk = true;
        emit setPositionHome();
    } else {
        widget->status("Rozpoczęcie kalibracja urządzenia.");
        emit setPositionHome();
    }
    ui->lStatus->setText(QString("Zakończono konfigurację kontrolera. Trwa zerowanie urządzenia...."));
}

void MiernikPrzeplywu::doneHome()
{
    if (modeWork == WyborMetody::MODE_FUNSET) {
        ui->statusWentylator->append(addTime("Kalibracja urządzenia zakończona."));
        ui->statusWentylator->append(addTime("Ustawiam pozycję na (%1,%2) mm [%3, %4] imp").arg(impmmx).arg(impmmy).arg(impx).arg(impy));
        ui->lStatus->setText(QString("Zakończono zerowanie urządzenia. Rozpoczynam ustawianie odpowiedniej pozycji..."));
        emit setPosition(impx, impy);
    } else {
        ui->lStatus->setText(QString("Rozpoczynam prace z lista pozycji"));
        widget->setIsStart(true);
        widget->setIsWait(false);
    }
}

void MiernikPrzeplywu::errorHome()
{
    QMessageBox::warning(this, QString("Kalibracja urządzenia"),
                         QString("Nie powiodła się kalibracja urządzenia. Wyłącz silniki i podjedź tacką do rogu gdzie są umieszczone silniki i spróbuje raz jeszcze"));
    if (modeWork == WyborMetody::MODE_FUNSET) {
        ui->statusWentylator->append(addTime("Błąd kalibracji."));
        ui->lStatus->setText(QString("Bład kalibracji urządzenia. Spróbuj jeszcze raz wyzerować urządzenie."));
        ui->pbUstaw->setEnabled(true);
    } else {
        widget->status("Błąd kalibracji.");
        widget->setIsStart(false);
        ui->lStatus->setText(QString("Bład kalibracji urządzenia. Spróbuj jeszcze raz wyzerować urządzenie."));
    }
}

void MiernikPrzeplywu::donePosition()
{
    if (modeWork == WyborMetody::MODE_FUNSET) {
        ui->statusWentylator->append(addTime("Pozycja ustawiona."));
        ui->lStatusWiatrak->setText(QString("Pozycja (%1,%2) ustawiona.").arg(impmmx).arg(impmmy));
        ui->lStatus->setText(QString("Pozycja dla wentylatora ustawiona."));
        ui->pbUstaw->setEnabled(true);
    } else {
        widget->setIsWait(false);
        widget->status("Pozycja ustawiona.");
        ui->lStatus->setText(QString("Pozycja ustawiona. Rozpoczynam pomiary ..."));
    }
}

void MiernikPrzeplywu::debug(QString dbg)
{
    qDebug() << dbg;
    ui->debug->append(dbg);
    if (widget)
        widget->setDebug(dbg);
}

void MiernikPrzeplywu::statusMiernik(QString txt)
{
    ui->lStatus->setText(txt);
}

void MiernikPrzeplywu::start()
{
    lockTab = true;
}

void MiernikPrzeplywu::noweDane()
{
    WyborMetody::ModeWork actWork = modeWork;
    modeWork = WyborMetody::MODE_SERVICE;
    on_tabWidget_currentChanged(actWork);
}

void MiernikPrzeplywu::end()
{
    lockTab = false;
}

void MiernikPrzeplywu::on_pbZeruj_clicked()
{
    if (!connIsOk) {
        ui->statusWentylator->append(addTime("Wysyłam konfigurację."));
        ui->lStatusWiatrak->setText("Wysyłam konfiguracje do sterownika...");
        emit setParams(mech.getReverseX(),mech.getReverseY(),
                       mech.getMaxImpusyX(), mech.getMaxImpusyY(),
                       mech.getMaxKrokiX(), mech.getMaxKrokiY());
    } else
        emit setPositionHome();
}
