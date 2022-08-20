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

    on_rbRoletaPrawe_clicked();

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

    tmr.setInterval(1000);
    connect(&tmr, &QTimer::timeout, this, &MainWindow::radioTimeout);
    ui->frame->setEnabled(false);
    ui->frame_2->setEnabled(false);
    ui->frame_3->setEnabled(false);
    ui->frame_4->setEnabled(false);
    ui->frame_5->setEnabled(false);
    ui->frame_6->setEnabled(false);
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
    }
}

void MainWindow::positionDone(int work)
{
    switch(work) {
    case SerialMessage::START_XY:
        qDebug() << "start both";
        ui->cbPosStart->setChecked(true);
        ui->pbUstaw->setEnabled(false);
        break;
    case SerialMessage::START_X:
        qDebug() << "start lewoprawo";
        ui->cbPosStartLP->setChecked(true);
        break;
    case SerialMessage::END_X:
        qDebug() << "end lewo";
        ui->cbPosKoniecLP->setChecked(true);
        ui->stepX->setText(QString::number(sMsg.getValues().moveStepX));
        ui->posImpX->setText(QString::number(sMsg.getValues().posImpX));
        break;
    case SerialMessage::START_Y:
        qDebug() << "Start gora dol";
        ui->cbPosStartGD->setChecked(true);
        break;
    case SerialMessage::END_Y:
        qDebug() << "Koniec gora dol";
        ui->cbPosKoniecGD->setChecked(true);
        ui->stepY->setText(QString::number(sMsg.getValues().moveStepY));
        ui->posImpY->setText(QString::number(sMsg.getValues().posImpY));
        break;
    case SerialMessage::END_XY:
        qDebug() << "Koniec pozycjoowanie";
        ui->cbPosKoniec->setChecked(true);
        ui->pbUstaw->setEnabled(true);
        break;
    case SerialMessage::START_R:
        qDebug() << "Poczatek bazowania";
        ui->cbRolPosStart->setChecked(true);
        break;
    case SerialMessage::END_R:
        qDebug() << "Koniec bazowania";
        ui->cbRolPosKoniec->setChecked(true);
        ui->pbRoletaUstaw->setEnabled(true);
        ui->stepR->setText(QString::number(sMsg.getValues().moveStepR));
        ui->posStepR->setText(QString::number(sMsg.getValues().posStepR));
        break;
    case SerialMessage::ERROR_XY:
        QMessageBox::critical(this, "Pozycja X/Y", "Nie udało się ustawić pozycji");
        ui->pbUstaw->setEnabled(true);
        break;
    case SerialMessage::ERROR_R:
        QMessageBox::critical(this, "Roleta", "Nie udało się ustawić pozycji rolety");
        ui->pbRoletaUstaw->setEnabled(true);
        break;
    default:
        break;
    }
}

void MainWindow::homeDone(int work)
{
    qDebug() << "homeDone work" << work;
    switch(work) {
    case SerialMessage::START_XY:
        qDebug() << "Start home";
        ui->cbHomeStart->setChecked(true);
        ui->pbHome->setEnabled(false);
        break;
    case SerialMessage::START_X:
        qDebug() << "Start X";
        ui->cbHomeStartLP->setChecked(true);
        break;
    case SerialMessage::END_X:
        qDebug() << "End X";
        ui->cbHomeKoniecLP->setChecked(true);
        ui->homeStepsX->setText(QString::number(sMsg.getValues().moveStepX));
        break;
    case SerialMessage::START_Y:
        qDebug() << "Start Y";
        ui->cbHomeStartGD->setChecked(true);
        break;
    case SerialMessage::END_Y:
        qDebug() << "End Y";
        ui->cbHomeKoniecGD->setChecked(true);
        ui->homeStepsY->setText(QString::number(sMsg.getValues().moveStepY));
        break;
    case SerialMessage::END_XY:
        qDebug() << "Koniec Baza";
        ui->cbHomeKoniec->setChecked(true);
        ui->pbHome->setEnabled(true);
        break;
    case SerialMessage::START_R:
        qDebug() << "Start R";
        ui->cbRolHomeStart->setChecked(true);
        ui->pbRoletaHome->setEnabled(false);
        break;
    case SerialMessage::END_R:
        qDebug() << "End R";
        ui->cbRolHomeKoniec->setChecked(true);
        ui->pbRoletaHome->setEnabled(true);
        ui->homeStepsR->setText(QString::number(sMsg.getValues().moveStepR));
        break;
    case SerialMessage::ERROR_XY:
        QMessageBox::critical(this, "Powrót do pozycji startowej", "Nie udało się ustawić pozycji bazowej");
        ui->pbHome->setEnabled(true);
        break;
    case SerialMessage::ERROR_R:
        QMessageBox::critical(this, "Roleta", "Nie udało się ustawić pozycji rolety");
        ui->pbRoletaHome->setEnabled(true);
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

    ui->maxImpx->setText(QString::number(rpos.getMaxImpusyX()));
    ui->maxImpY->setText(QString::number(rpos.getMaxImpusyY()));

    ui->maxStepX->setText(QString::number(rpos.getMaxKrokiX()));
    ui->maxStepY->setText(QString::number(rpos.getMaxKrokiY()));
    ui->maxStepR->setText(QString::number(rpos.getMaxKrokiR()));

    ui->dirX->setChecked(rpos.getReverseX());
    ui->dirY->setChecked(rpos.getReverseY());
    ui->dirR->setChecked(rpos.getReverseR());

    sMsg.setParams(rpos.getReverseX(), rpos.getReverseY(), rpos.getReverseR(),
                      rpos.getMaxImpusyX(), rpos.getMaxImpusyY(),
                      rpos.getMaxKrokiX(), rpos.getMaxKrokiY(),
                      rpos.getMaxKrokiR());
}

void MainWindow::kontrolerConfigured(bool success, int state)
{

    debug(QString("Open device open = %1, state = %2 ").arg(success).arg(state));

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
    //ui->pbHome->setEnabled(false);
    //ui->pbUstaw->setEnabled(false);

    ui->pbFindSerial->setEnabled(true);
    ui->errorserial->setText(error);
    ui->debug->appendPlainText(error);
}


void MainWindow::successOpenDevice(bool open)
{
    debug(QString("%1 open port").arg(open ? "Success" : "Unsucces"));
    if (open) {
        ui->pbFindSerial->setEnabled(false);
        ui->statusserial->setText("Sprawdzam sterownik...");
        ui->errorserial->setText("Port poprawnie otwarty");
        emit checkDevice();

    } else {
        ui->errorserial->setText("Nie udalo sie otworzyc portu");
    }
    ui->pbFindSerial->setEnabled(false);

}


void MainWindow::controllerOK()
{
    ui->statusserial->setText("Sterownik OK");
    //ui->pbHome->setEnabled(true);
    //ui->pbUstaw->setEnabled(true);
}

void MainWindow::setParamsDone()
{
    ui->statusparams->setText("Parametry ustawione");
}
*/

void MainWindow::errorHome()
{

}

void MainWindow::errorPosition()
{

}

void MainWindow::errorHomeRoleta()
{

}

void MainWindow::errorRoleta()
{

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
    ui->cbHomeStart->setChecked(false);
    ui->cbHomeStartLP->setChecked(false);
    ui->cbHomeKoniecLP->setChecked(false);
    ui->cbHomeStartGD->setChecked(false);
    ui->cbHomeKoniecGD->setChecked(false);
    ui->cbHomeKoniec->setChecked(false);
    ui->homeStepsX->setText("-");
    ui->homeStepsY->setText("-");
    setPositionHome();
}

void MainWindow::pbUstaw_clicked()
{
    ui->cbPosStart->setChecked(false);
    ui->cbPosStartLP->setChecked(false);
    ui->cbPosKoniecLP->setChecked(false);
    ui->cbPosStartGD->setChecked(false);
    ui->cbPosKoniecGD->setChecked(false);
    ui->cbPosKoniec->setChecked(false);

    ui->stepX->setText("-");
    ui->stepY->setText("-");
    ui->posImpX->setText("-");
    ui->posImpY->setText("-");

    setPosition(ui->pos_X->text().toUInt(), ui->pos_Y->text().toUInt());
}

void MainWindow::pbClose_clicked()
{
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
    ui->statusparams->setText("Trwa ustawianie parametrów");
    setParams(reverseX, reverseY, reverseR, impX, impY, stepX, stepY, stepR);
}

void MainWindow::tbRoletaR_clicked()
{
    RoletaKroki * dlg = new RoletaKroki(&rr, ui->pos_R, ui->lroletamm, this);
    dlg->exec();
}

void MainWindow::pbRoletaHome_clicked()
{
    ui->cbRolHomeKoniec->setChecked(false);
    ui->cbRolHomeStart->setChecked(false);
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
    ui->cbRolPosStart->setChecked(false);
    ui->cbRolPosKoniec->setChecked(false);
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
    ImpulsyDlg * dlg = new ImpulsyDlg(ust.getImpulsyXperMM().toUInt(), ust.getKrokiXperMM().toUInt(),
                                       QString("Oś lewo-prawo"), ui->pos_X, this);
    dlg->exec();
}

void MainWindow::tbY_clicked()
{
    ImpulsyDlg * dlg = new ImpulsyDlg(ust.getImpulsyYperMM().toUInt(), ust.getKrokiYperMM().toUInt(),
                                       QString("Oś góra-dół"), ui->pos_Y, this);
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

void MainWindow::setParams(bool reverseX, bool reverseY, bool reverseR, uint32_t maxImpX, uint32_t maxImpY, uint32_t maxStepX, uint32_t maxStepY, uint32_t maxStepR)
{
    sMsg.setParams(reverseX, reverseY, reverseR, maxImpX, maxImpY, maxStepX, maxStepY, maxStepR);
}

void MainWindow::readRadio()
{
    sMsg.readRadio();
}




void MainWindow::on_rbStac_clicked()
{
    modeWork = MODE_2700;
    wyboStanowiska();
}


void MainWindow::on_rbOknoPrawe_clicked()
{
    modeWork = MODE_1000P;
    wyboStanowiska();
}


void MainWindow::on_rbOknoLewe_clicked()
{
    modeWork = MODE_1000L;
    wyboStanowiska();
}


void MainWindow::on_rbRoletaPrawe_clicked()
{
    modeWork = MODE_ROLETAP;
    wyboStanowiska();
}


void MainWindow::on_rbRoletaLewe_clicked()
{
    modeWork = MODE_ROLETAL;
    wyboStanowiska();
}


void MainWindow::on_tbRoletaR_clicked()
{

}

