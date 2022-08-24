#include "mainwindow.h"
#include "qdatetime.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include "roletakroki.h"
#include "impulsydlg.h"

#include  <QDebug>

#include <QPushButton>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Program testowy");

    connect(&sMsg, &SerialDevice::error, this, &MainWindow::errorSerial, Qt::QueuedConnection);
    connect(&sMsg, &SerialDevice::debug, this, &MainWindow::debug, Qt::QueuedConnection);

    connect(&sMsg, &SerialDevice::deviceName, this, &MainWindow::deviceName, Qt::QueuedConnection);
    connect(&sMsg, &SerialDevice::kontrolerConfigured, this, &MainWindow::kontrolerConfigured, Qt::QueuedConnection);

    connect(&sMsg, &SerialDevice::setParamsDone, this, &MainWindow::setParamsDone, Qt::QueuedConnection);
    connect(&sMsg, &SerialDevice::setPositionDone, this, &MainWindow::setPositionDone, Qt::QueuedConnection);

    connect(&sMsg, &SerialDevice::readFromRadio, this, &MainWindow::readedFromRadio, Qt::QueuedConnection);

    rpos.setUstawienia(ust);
    rr.setUstawienia(ust);

    rbRoletaPrawe_clicked();

    ui->pbRadioOff->setEnabled(false);

    connect(ui->pbHome, &QPushButton::clicked, this, &MainWindow::pbHome_clicked);
    connect(ui->pbUstaw, &QPushButton::clicked, this, &MainWindow::pbUstaw_clicked);
    connect(ui->pbClose, &QPushButton::clicked, this, &MainWindow::pbClose_clicked);
    connect(ui->pbSettings, &QPushButton::clicked, this, &MainWindow::pbSettings_clicked);
    connect(ui->tbRoletaR, &QToolButton::clicked, this, &MainWindow::tbRoletaR_clicked);
    connect(ui->pbRoletaHome, &QPushButton::clicked, this, &MainWindow::pbRoletaHome_clicked);
    connect(ui->pbRoletaUstaw, &QPushButton::clicked, this, &MainWindow::pbRoletaUstaw_clicked);
    connect(ui->pbRadioOff, &QPushButton::clicked, this, &MainWindow::pbRadioOff_clicked);
    connect(ui->pbRadioOn, &QPushButton::clicked, this, &MainWindow::pbRadioOn_clicked);
    connect(ui->tb_X, &QToolButton::clicked, this, &MainWindow::tbX_clicked);
    connect(ui->tb_Y, &QToolButton::clicked, this, &MainWindow::tbY_clicked);
    connect(ui->pbFindSerial, &QPushButton::clicked, this, &MainWindow::pbFindSerial_clicked);
    connect(ui->pbClose, &QPushButton::clicked, this, &MainWindow::pbClose_clicked);
    connect(ui->rbStac, &QPushButton::clicked, this, &MainWindow::rbStac_clicked);
    connect(ui->rbOknoPrawe, &QPushButton::clicked, this, &MainWindow::rbOknoPrawe_clicked);
    connect(ui->rbOknoLewe, &QPushButton::clicked, this, &MainWindow::rbOknoLewe_clicked);
    connect(ui->rbRoletaPrawe, &QPushButton::clicked, this, &MainWindow::rbRoletaPrawe_clicked);
    connect(ui->rbRoletaLewe, &QPushButton::clicked, this, &MainWindow::rbRoletaLewe_clicked);
    connect(ui->pbStop, &QPushButton::clicked, this, &MainWindow::pbStop_clicked);
    connect(ui->pbSettings2, &QPushButton::clicked, this, &MainWindow::pbSettings2_clicked);
    connect(ui->speedRDown, &QLineEdit::editingFinished, this, &MainWindow::changeTimeObroty);
    connect(ui->speedRUp, &QLineEdit::editingFinished, this, &MainWindow::changeTimeObroty);
    connect(ui->minR, &QLineEdit::editingFinished, this, &MainWindow::changeRoletaPostoj);

    tmr.setInterval(1000);
    connect(&tmr, &QTimer::timeout, this, &MainWindow::radioTimeout);
    ui->frame->setEnabled(false);
    ui->frame_2->setEnabled(false);
    ui->frame_3->setEnabled(false);
    ui->frame_4->setEnabled(false);
    ui->frame_5->setEnabled(false);
    ui->frame_6->setEnabled(false);
    ui->progresHome->setValue(0);
    ui->progressPos->setValue(0);
    ui->progressRoletaPos->setValue(0);
    ui->progressRoletaHome->setValue(0);


    //changeTimeObroty();
    //changeRoletaPostoj();

    sMsg.setThread(&thSterownik);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::errorSerial(const QString &)
{

}

void MainWindow::setPositionDone(bool success, bool home, int work)
{
    if (success && home) {
        homeDone(work);
    } else if (success && !home) {
        positionDone(work);
    } else {
        if (work == SerialMessage::START_R || work == SerialMessage::END_R || work == SerialMessage::ERROR_R)
            home ? errorHomeRoleta() : errorRoleta();
        else
            home ? errorHome() : errorPosition();
    }
}

void MainWindow::positionDone(int work)
{
    switch(work) {
    case SerialMessage::START_XY:
        debug("Rozpoczynam ustawianie pozycji...");
        ui->progressPos->setValue(1);
        ui->pbUstaw->setEnabled(false);
        break;
    case SerialMessage::START_X:
        debug("Rozpoczynam ustawianie pozycji na osi X...");
        ui->progressPos->setValue(2);
        break;
    case SerialMessage::END_X:
        debug("Pozycja na osi X ustalona");
        ui->progressPos->setValue(3);
        ui->stepX->setText(QString::number(sMsg.getValues().moveStepX));
        ui->posImpX->setText(QString::number(sMsg.getValues().posImpX));
        break;
    case SerialMessage::START_Y:
        debug("Rozpoczynam ustawianie pozycji na osi Y...");
        ui->progressPos->setValue(4);
        break;
    case SerialMessage::END_Y:
        debug("Pozycja na osi Y ustalona");
        ui->progressPos->setValue(5);
        ui->stepY->setText(QString::number(sMsg.getValues().moveStepY));
        ui->posImpY->setText(QString::number(sMsg.getValues().posImpY));
        break;
    case SerialMessage::END_XY:
        debug("Pozycja czujnika ustalona na obu osiach");
        ui->progressPos->setValue(6);
        ui->pbUstaw->setEnabled(true);
        break;
    case SerialMessage::START_R:
        debug("Ustawiam roletę w odpowiedniej pozycji...");
        ui->progressRoletaPos->setValue(1);
        break;
    case SerialMessage::END_R:
        debug("Roleta ustawiona");
        ui->progressRoletaPos->setValue(3);
        ui->pbRoletaUstaw->setEnabled(true);
        ui->stepR->setText(QString::number(sMsg.getValues().moveStepR));
        ui->posStepR->setText(QString::number(sMsg.getValues().posStepR));
        break;
    case SerialMessage::ERROR_XY:
        QMessageBox::critical(this, "Pozycja X/Y", "Nie udało się ustawić pozycji");
        ui->pbUstaw->setEnabled(true);
        errorPosition();
        break;
    case SerialMessage::ERROR_R:
        QMessageBox::critical(this, "Roleta", "Nie udało się ustawić pozycji rolety");
        ui->pbRoletaUstaw->setEnabled(true);
        errorRoleta();
        break;
    default:
        break;
    }
}

void MainWindow::homeDone(int work)
{
    switch(work) {
    case SerialMessage::START_XY:
        debug("Rozpoczynam zerowanie...");
        ui->progresHome->setValue(1);
        ui->pbHome->setEnabled(false);
        break;
    case SerialMessage::START_X:
        debug("Zeruje oś X....");
        ui->progresHome->setValue(2);
        break;
    case SerialMessage::END_X:
        debug("Oś X wyzerowana");
        ui->progresHome->setValue(3);
        ui->homeStepsX->setText(QString::number(sMsg.getValues().moveStepX));
        break;
    case SerialMessage::START_Y:
        debug("Zeruje oś Y.....");
        ui->progresHome->setValue(4);
        break;
    case SerialMessage::END_Y:
        debug("Oś Y wyzerowana");
        ui->progresHome->setValue(5);
        ui->homeStepsY->setText(QString::number(sMsg.getValues().moveStepY));
        break;
    case SerialMessage::END_XY:
        debug("Urządzenie skalibrowane");
        ui->progresHome->setValue(6);
        ui->pbHome->setEnabled(true);
        break;
    case SerialMessage::START_R:
        debug("Zeruje roletę.....");
        ui->progressRoletaHome->setValue(1);
        ui->pbRoletaHome->setEnabled(false);
        break;
    case SerialMessage::END_R:
        debug("Roleta zamknięta");
        ui->progressRoletaHome->setValue(2);
        ui->pbRoletaHome->setEnabled(true);
        ui->homeStepsR->setText(QString::number(sMsg.getValues().moveStepR));
        break;
    case SerialMessage::ERROR_XY:
        QMessageBox::critical(this, "Powrót do pozycji startowej", "Nie udało się ustawić pozycji bazowej");
        ui->pbHome->setEnabled(true);
        errorHome();
        break;
    case SerialMessage::ERROR_R:
        QMessageBox::critical(this, "Roleta", "Nie udało się ustawić pozycji startowej rolety");
        ui->pbRoletaHome->setEnabled(true);
        errorHomeRoleta();
        break;
    default:
        break;
    }
}

void MainWindow::wyboStanowiska()
{
    bool lewaR = modeWork == MODE_ROLETAL;
    bool lewaO = modeWork == MODE_1000L;
    switch(modeWork) {
    case MODE_ROLETAP:
    case MODE_ROLETAL:
        rpos.setPrzestrzen(ust.getRolOsXReal().toUInt(), ust.getRolOsYReal().toUInt());
        rpos.setReverseY(false);
        rpos.setReverseX(!lewaR);
        rr.setReverse(lewaR);
        rpos.setReverseR(true);
        ui->rbOknoLewe->setChecked(false);
        ui->rbOknoPrawe->setChecked(false);
        ui->rbStac->setChecked(false);
        ui->rbRoletaPrawe->setChecked(!lewaR);
        ui->rbRoletaLewe->setChecked(lewaR);
        break;
    case MODE_2700:
        rpos.setPrzestrzen(ust.getStacOsXReal().toUInt(), ust.getStacOsYReal().toUInt());
        rpos.setReverseY(false);
        rpos.setReverseX(false);
        rpos.setReverseR(true);
        ui->rbOknoLewe->setChecked(false);
        ui->rbOknoPrawe->setChecked(false);
        ui->rbStac->setChecked(true);
        ui->rbRoletaPrawe->setChecked(false);
        ui->rbRoletaLewe->setChecked(false);
        break;
    case MODE_1000L:
    case MODE_1000P:
        rpos.setPrzestrzen(ust.getOknoOsXReal().toUInt(), ust.getOknoOsYReal().toUInt());
        rpos.setReverseY(false);
        rpos.setReverseX(!lewaO);
        rpos.setReverseR(true);
        ui->rbOknoLewe->setChecked(lewaO);
        ui->rbOknoPrawe->setChecked(!lewaO);
        ui->rbStac->setChecked(false);
        ui->rbRoletaPrawe->setChecked(false);
        ui->rbRoletaLewe->setChecked(false);
        break;
    case MODE_NONE:
    default:
        Q_ASSERT(true);
        rpos.setPrzestrzen(0, 0);
        rpos.setReverseY(false);
        rpos.setReverseX(false);
        rpos.setReverseR(false);
        rr.setMaxKroki(0);
        break;
    }
    rpos.setMaxKrokiR(ust.getMaxRolKroki().toUInt());
    rr.setMinKroki(ust.getRolMinHomeStep().toUInt());
    rr.setSpeedHome(ust.getRolSpeedHomeStep().toUInt());
    rr.setSpeedPos(ust.getRolSpeedPosStep().toUInt());

    ui->maxImpx->setText(QString::number(rpos.getMaxImpusyX()));
    ui->maxImpY->setText(QString::number(rpos.getMaxImpusyY()));

    ui->maxStepX->setText(QString::number(rpos.getMaxKrokiX()));
    ui->maxStepY->setText(QString::number(rpos.getMaxKrokiY()));
    ui->maxStepR->setText(QString::number(rpos.getMaxKrokiR()));

    ui->dirX->setChecked(rpos.getReverseX());
    ui->dirY->setChecked(rpos.getReverseY());
    ui->dirR->setChecked(rpos.getReverseR());

    ui->minR->setText(QString::number(rr.getMinKroki()));
    ui->speedRDown->setText(QString::number(rr.getSpeedHome()));
    ui->speedRUp->setText(QString::number(rr.getSpeedPos()));

    sMsg.setParams(rpos.getReverseX(), rpos.getReverseY(), rpos.getReverseR(),
                      rpos.getMaxImpusyX(), rpos.getMaxImpusyY(),
                      rpos.getMaxKrokiX(), rpos.getMaxKrokiY(),
                      rpos.getMaxKrokiR(), rr.getMinKroki(),
                      rr.getSpeedHome(), rr.getSpeedPos());

    ui->max_wysokosc->setText(QString("%1 mm").arg(rr.getMaxMM()));
    changeTimeObroty();
    changeRoletaPostoj();
}

void MainWindow::kontrolerConfigured(bool success, int state)
{
    const char* st[] = { "NO_FOUND","FOUND","NO_OPEN","OPEN","NO_READ","IDENT_FAILD","IDENT_OK","PARAMS_FAILD","PARAMS_OK","ALL_OK","CLOSE" };
    debug(QString("Open device open = %1, state = %2 ").arg(success).arg(st[state]));

    switch(state) {

    case SerialDevice::NO_FOUND:
        debug("Nie znaleziono kontrolera");
        ui->frame->setEnabled(false);
        ui->frame_2->setEnabled(false);
        ui->frame_3->setEnabled(false);
        ui->frame_4->setEnabled(false);
        ui->frame_5->setEnabled(false);
        ui->frame_6->setEnabled(false);

        ui->portName->setText("");

        ui->statusserial->setText("Nie znaleziono kontrolera");
        break;

    case SerialDevice::FOUND:
        ui->statusserial->setText(QString("Znaleziono kontroler. Otwieram port ...."));
        break;

    case SerialDevice::NO_OPEN:
        ui->statusserial->setText(QString("Nie udało się otworzyć."));

        break;

    case SerialDevice::OPEN:
        ui->statusserial->setText(QString("Sprawdzam kontroler..."));
        break;

    case SerialDevice::NO_READ:
        ui->statusserial->setText(QString("Problem z odczytem z portu."));

        break;

    case SerialDevice::IDENT_FAILD:
        ui->statusserial->setText(QString("Nieprawidłowy kontroler."));

        break;

    case SerialDevice::IDENT_OK:
        ui->statusserial->setText(QString("Trwa konfiguracja..."));
        break;

    case SerialDevice::PARAMS_FAILD:
        ui->statusserial->setText(QString("Nie skonfigurowano."));

        break;

    case SerialDevice::PARAMS_OK:
        ui->statusserial->setText(QString("Skonfigurowano."));
        break;

    case SerialDevice::ALL_OK:
        ui->statusserial->setText(QString("OK"));
        ui->frame->setEnabled(true);
        ui->frame_6->setEnabled(true);
        setParamsDone(true);
        break;

    case SerialDevice::CLOSE:
        ui->statusserial->setText("Kontroler rozłączony");
        ui->portName->setText("");
        ui->frame->setEnabled(false);
        ui->frame_2->setEnabled(false);
        ui->frame_3->setEnabled(false);
        ui->frame_4->setEnabled(false);
        ui->frame_5->setEnabled(false);
        ui->frame_6->setEnabled(false);
        break;

    default:
        break;
    }
}

void MainWindow::readedFromRadio(bool sucess, int val1, int val2, int val3, int val4)
{
    (void)val2;(void)val3;(void)val4;
    if (sucess)
        ui->radioInfo->setText(QString::number(val1));
    else
        ui->radioInfo->setText("Błąd");
}

void MainWindow::deviceName(const QString &portname)
{
    ui->portName->setText(portname);
}

void MainWindow::setParamsDone(bool success)
{
    if (success) {
        ui->frame->setEnabled(true);
        ui->frame_2->setEnabled(true);
        ui->frame_3->setEnabled(true);
        ui->frame_4->setEnabled(true);
        ui->frame_5->setEnabled(true);
        ui->statusparams->setText("Konfiguracja ustawiona");
    } else {
        ui->frame->setEnabled(false);
        ui->frame_2->setEnabled(false);
        ui->frame_3->setEnabled(false);
        ui->frame_4->setEnabled(false);
        ui->frame_5->setEnabled(false);
        ui->statusparams->setText("Nie udało się skonfigurować");
    }
}

void MainWindow::pbFindSerial_clicked()
{

    connectToDevice();
}

void MainWindow::roletaHome()
{

}

/*
void MainWindow::errorSerial(QString error)
{
    ui->pbHome->setEnabled(false);
    ui->pbUstaw->setEnabled(false);

    ui->pbFindSerial->setEnabled(true);
    ui->errorserial->setText(error);
    ui->debug->appendPlainText(error);
}
*/

void MainWindow::errorHome()
{
    QMessageBox::critical(this, "Zerowanie", "Nie udało się wyzerować");
    ui->pbHome->setEnabled(true);
}

void MainWindow::errorPosition()
{

    QMessageBox::critical(this, "Pozycjonowanie", "Nie udało się ustawić pozycji");
}

void MainWindow::errorHomeRoleta()
{
    QMessageBox::critical(this, "Roleta", "Nie udało się opuścić rolety");
}


void MainWindow::errorRoleta()
{
    QMessageBox::critical(this, "Roleta", "Nie udało podnieść rolety");
}

QString MainWindow::addTime(QString status)
{
    return QString("[%1] %2").arg(QTime::currentTime().toString("HH:mm:ss.zzz")).arg(status);
}


void MainWindow::debug(QString text)
{
    ui->debug->appendPlainText(addTime(text));
}

void MainWindow::pbHome_clicked()
{
    ui->progresHome->setValue(0);
    ui->progressPos->setValue(0);
    ui->progressRoletaPos->setValue(0);
    ui->progressRoletaHome->setValue(0);
    ui->homeStepsX->setText("-");
    ui->homeStepsY->setText("-");
    setPositionHome();
}

void MainWindow::pbUstaw_clicked()
{
    ui->progresHome->setValue(0);
    ui->progressPos->setValue(0);
    ui->progressRoletaPos->setValue(0);
    ui->progressRoletaHome->setValue(0);

    ui->stepX->setText("-");
    ui->stepY->setText("-");
    ui->posImpX->setText("-");
    ui->posImpY->setText("-");

    setPosition(ui->pos_X->text().toUInt(), ui->pos_Y->text().toUInt());
}

void MainWindow::pbClose_clicked()
{
    ui->progresHome->setValue(0);
    ui->progressPos->setValue(0);
    ui->progressRoletaPos->setValue(0);
    ui->progressRoletaHome->setValue(0);
    ui->pbFindSerial->setEnabled(true);
    sMsg.closeDevice(false);
}

void MainWindow::pbSettings_clicked()
{
    bool ok;
    int32_t impX =0, impY = 0, stepX = 0, stepY = 0, stepR = 0;
    impX = ui->maxImpx->text().toUInt(&ok);
    if (!ok)
        impX = 0;

    impY = ui->maxImpY->text().toUInt(&ok);
    if (!ok)
        impY = 0;

    stepX = ui->maxStepX->text().toUInt(&ok);
    if (!ok)
        stepX = 0;

    stepY = ui->maxStepY->text().toUInt(&ok);
    if (!ok)
        stepY = 0;

    stepR = ui->maxStepR->text().toUInt(&ok);
    if (!ok)
        stepR = 0;

    bool reverseX = ui->dirX->isChecked();
    bool reverseY = ui->dirY->isChecked();
    bool reverseR = ui->dirR->isChecked();

    uint16_t mStepR, spdHome, spdPos;
    mStepR = ui->minR->text().toUInt(&ok);
    if (!ok)
        mStepR = 0;

    spdHome = ui->speedRDown->text().toUInt(&ok);
    if (!ok)
        spdHome = 400;

    spdPos = ui->speedRUp->text().toUInt(&ok);
    if (!ok)
        spdPos = 1000;


    ui->max_wysokosc->setText(QString("% 1 mm").arg(rr.getMaxMM()));
    float basemm = rr.getMinKroki() * rr.getMaxMM() / rr.getMaxKroki();
    ui->minrR_mm->setText(QString("~ %1 mm").arg(basemm, 0, 'f', 2));
    float obr1 = 1.0 / (1000.0 / (rr.getSpeedHome() + 50)) * 60;
    float obr2 = 1.0 / (1000.0 / (rr.getSpeedPos() + 50)) * 60;
    ui->speedHomObroty->setText(QString("%1 obr/min").arg(obr1, 0, 'f', 3));
    ui->speedPosObroty->setText(QString("%2 obr/min").arg(obr2, 0, 'f', 3));
    
    ust.setRolMinHomeStep(ui->minR->text());
    ust.setRolSpeedHomeStep(ui->speedRDown->text());
    ust.setRolSpeedPosStep(ui->speedRUp->text());
    rr.setMinKroki(ui->minR->text().toUInt());
    rr.setSpeedHome(ui->speedRDown->text().toUInt());
    rr.setSpeedPos(ui->speedRUp->text().toUInt());
    ui->statusparams->setText("Trwa ustawianie parametrów");

    setParams(reverseX, reverseY, reverseR, impX, impY, stepX, stepY, stepR, mStepR, spdHome, spdPos);
}

void MainWindow::tbRoletaR_clicked()
{
    RoletaKroki * dlg = new RoletaKroki(&rr, ui->pos_R, ui->lroletamm, this);
    dlg->exec();
}

void MainWindow::pbRoletaHome_clicked()
{
    ui->progresHome->setValue(0);
    ui->progressPos->setValue(0);
    ui->progressRoletaPos->setValue(0);
    ui->progressRoletaHome->setValue(0);
    ui->homeStepsR->setText("-");
    setRoletaHome();
}

void MainWindow::pbRoletaUstaw_clicked()
{
    if (ui->pos_R->text().isEmpty())
        return;
    bool ok;
    uint32_t steps = ui->pos_R->text().toULong(&ok);
    if (!ok)
        return;
    ui->progresHome->setValue(0);
    ui->progressPos->setValue(0);
    ui->progressRoletaPos->setValue(0);
    ui->progressRoletaHome->setValue(0);
    //ui->pbRoletaUstaw->setEnabled(false);
    ui->stepR->setText("-");
    ui->posStepR->setText("-");
    setRoleta(steps);
}


void MainWindow::pbRadioOff_clicked()
{
    ui->pbRadioOff->setEnabled(false);
    ui->pbRadioOn->setEnabled(true);
    tmr.stop();

}


void MainWindow::pbRadioOn_clicked()
{
    ui->pbRadioOn->setEnabled(false);
    ui->pbRadioOff->setEnabled(true);
    tmr.start();
}

//unsigned int imp2mm, unsigned int step2mm, const QString & title, QLineEdit * value, QWidget *parent = nullptr);

void MainWindow::tbX_clicked()
{
    ImpulsyDlg * dlg = new ImpulsyDlg(ust.getImpulsyXperMM().toFloat(), ust.getKrokiXperMM().toFloat(),
                                       QString("Oś lewo-prawo"), ui->pos_X,
                                      ui->posXsteps, ui->posXmm, this);
    dlg->exec();
}

void MainWindow::tbY_clicked()
{
    ImpulsyDlg * dlg = new ImpulsyDlg(ust.getImpulsyYperMM().toFloat(), ust.getKrokiYperMM().toFloat(),
                                       QString("Oś góra-dół"), ui->pos_Y,
                                       ui->posYsteps, ui->posYmm, this);
    dlg->exec();
}

void MainWindow::radioTimeout()
{
    readRadio();
}

void MainWindow::connectToDevice()
{
    ui->statusserial->setText("");
    ui->portName->setText("");
    sMsg.connectToDevice();
}

void MainWindow::setPositionHome()
{
    sMsg.setPositionHome();
}

void MainWindow::setPosition(uint32_t x, uint32_t y)
{
    sMsg.setPosition(x, y);
}

void MainWindow::setRoletaHome()
{
    sMsg.setRoletaHome();
}

void MainWindow::setRoleta(uint32_t r)
{
    sMsg.setRoleta(r);
}

void MainWindow::setParams(bool reverseX, bool reverseY, bool reverseR, uint32_t maxImpX, uint32_t maxImpY, 
    uint32_t maxStepX, uint32_t maxStepY, uint32_t maxStepR, uint16_t minStepR, uint16_t speedHome, uint16_t speedPos)
{
    sMsg.setParams(reverseX, reverseY, reverseR, maxImpX, maxImpY, maxStepX, maxStepY, maxStepR, minStepR, speedHome, speedPos);
}

void MainWindow::readRadio()
{
    sMsg.readRadio();
}




void MainWindow::rbStac_clicked()
{
    modeWork = MODE_2700;
    wyboStanowiska();
}


void MainWindow::rbOknoPrawe_clicked()
{
    modeWork = MODE_1000P;
    wyboStanowiska();
}


void MainWindow::rbOknoLewe_clicked()
{
    modeWork = MODE_1000L;
    wyboStanowiska();
}


void MainWindow::rbRoletaPrawe_clicked()
{
    modeWork = MODE_ROLETAP;
    wyboStanowiska();
}


void MainWindow::rbRoletaLewe_clicked()
{
    modeWork = MODE_ROLETAL;
    wyboStanowiska();
}


void MainWindow::pbStop_clicked()
{
    sMsg.setReset();
}

void MainWindow::pbSettings2_clicked()
{
    pbSettings_clicked();
}

void MainWindow::changeTimeObroty()
{
    bool ok;
    unsigned int speedDown = ui->speedRDown->text().toInt(&ok);
    if (!ok)
        return;

    unsigned int speedUp = ui->speedRUp->text().toInt(&ok);
    if (!ok)
        return;


    float obr1 = (1000.0 / (speedDown + 50.0)) * 60.0;
    float obr2 = (1000.0 / (speedUp + 50.0)) * 60.0;
    ui->speedHomObroty->setText(QString("%1 obr/min").arg(obr1, 0, 'f', 3));
    ui->speedPosObroty->setText(QString("%2 obr/min").arg(obr2, 0, 'f', 3));

    ui->infoObroty->setText(QString("<html><head/><body><p><span style=\"font-size:6pt;\">Prędkość obrotowa wyznaczona jest dla przekladni 1:1 i 1000 kroków na obrót .Aby uzyskać wartość dla 8000 kroków i przekładni 1:3 należy podzielić wartość przez 3*8 [%1, %2]</span></p></body></html>")
        .arg(obr1 / 24.0, 0, 'f', 3).arg(obr2 / 24.0, 0, 'f', 3));
}

void  MainWindow::changeRoletaPostoj()
{
    bool ok;
    unsigned int step = ui->minR->text().toInt(&ok);
    if (!ok)
        return;
    unsigned int maxStep = ui->maxStepR->text().toInt(&ok);
    if (!ok)
        return;
    unsigned int maxmm = rr.getMaxMM();
    if (!ok)
        return;
    ui->minrR_mm->setText(QString("~ %1 mm").arg(QString::number((unsigned int)(1.0 * step * maxmm / maxStep))));
}