#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include "roletakroki.h"

#include <QPushButton>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Program testowy");

    connect(&sMsg, SIGNAL(positionStatus(SerialMessage::StatusWork)), this, SLOT(positionDone(SerialMessage::StatusWork)));
    connect(&sMsg, SIGNAL(homeStatus(SerialMessage::StatusWork)), this, SLOT(homeDone(SerialMessage::StatusWork)));


    connect(&sMsg, SIGNAL(errorReadFromRadio()), this, SLOT(errorReadFromRadio()));
    connect(&sMsg, SIGNAL(readFromRadio(int)), this, SLOT(readFromRadio(int)));



    connect(&sMsg, SIGNAL(errorSerial(QString)), this, SLOT(errorSerial(QString)));
    connect(&sMsg, SIGNAL(debug(QString)), this, SLOT(debug(QString)));


    connect(&sMsg, SIGNAL(deviceName(QString)), this, SLOT(deviceName(QString)));
    connect(&sMsg, SIGNAL(controllerOK()), this, SLOT(controllerOK()));
    connect(&sMsg, SIGNAL(successOpenDevice(bool)), this, SLOT(successOpenDevice(bool)));

    connect(&sMsg, SIGNAL(setParamsDone()), this, SLOT(setParamsDone()));

    connect(this, SIGNAL(connectToDevice()), &sMsg, SLOT(connectToSerial()));
    connect(this, SIGNAL(checkDevice()), &sMsg, SLOT(checkController()));


    connect(this, SIGNAL(setPositionHome()), &sMsg, SLOT(setPositionHome()));
    connect(this, SIGNAL(setPosition(uint32_t,uint32_t)), &sMsg, SLOT(setPosition(uint32_t,uint32_t)));

    connect(this, SIGNAL(setRoletaHome()), &sMsg, SLOT(setRoletaHome()));
    connect(this, SIGNAL(setRoleta(uint32_t)), &sMsg, SLOT(setRoleta(uint32_t)));

    connect(this, SIGNAL(setParams(bool,bool,bool,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t)),
            &sMsg,  SLOT(setParams(bool,bool,bool,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t)));


    connect(this, SIGNAL(readRadio()), &sMsg, SLOT(readRadio()));

    rpos.setImpusyXPerMM(ust.getImpulsyXperMM().toUInt());
    rpos.setImpusyYPerMM(ust.getImpulsyYperMM().toUInt());

    rpos.setKrokiXPerMM(ust.getKrokiXperMM().toUInt());
    rpos.setKrokiYPerMM(ust.getKrokiYperMM().toUInt());

    rr.setKrokiPerObrot(ust.getRolStepObrot().toUInt());
    rr.setMaxMM(ust.getRolDlugosc().toUInt());
    rr.setMaxKroki(160000);

    ui->maxImpx->setText("750000");
    ui->maxImpY->setText("750000");

    ui->maxStepX->setText("300000");
    ui->maxStepY->setText("300000");
    ui->maxStepR->setText("160000");

    ui->pbRadioOff->setEnabled(false);

    connect(ui->pbHome, &QPushButton::clicked, this, &MainWindow::on_pbHome_clicked);
    connect(ui->pbUstaw, &QPushButton::clicked, this, &MainWindow::on_pbUstaw_clicked);
    connect(ui->pbClose, &QPushButton::clicked, this, &MainWindow::on_pbClose_clicked);
    connect(ui->pbSettings, &QPushButton::clicked, this, &MainWindow::on_pbSettings_clicked);
    connect(ui->tbRoletaR, &QToolButton::clicked, this, &MainWindow::on_tbRoletaR_clicked);
    connect(ui->pbRoletaHome, &QPushButton::clicked, this, &MainWindow::on_pbRoletaHome_clicked);
    connect(ui->pbRoletaUstaw, &QPushButton::clicked, this, &MainWindow::on_pbRoletaUstaw_clicked);
    connect(ui->pbRadioOff, &QPushButton::clicked, this, &MainWindow::on_pbRadioOff_clicked);
    connect(ui->pbRadioOn, &QPushButton::clicked, this, &MainWindow::on_pbRadioOn_clicked);

    tmr.setInterval(1000);
    connect(&tmr, &QTimer::timeout, this, &MainWindow::radioTimeout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pbFindSerial_clicked()
{
    ui->statusserial->setText("");
    ui->errorserial->setText("");
    emit connectToDevice();
}

void MainWindow::connectedToPort(QString portname)
{
    ui->portname->setText(portname);
}

void MainWindow::positionDone(SerialMessage::StatusWork work)
{
    switch(work) {
    case SerialMessage::START_XY:
        ui->cbPosStart->setChecked(true);
        ui->pbUstaw->setEnabled(false);
        break;
    case SerialMessage::START_X:
        ui->cbPosStartLP->setChecked(true);
        break;
    case SerialMessage::END_X:
        ui->cbPosKoniecLP->setChecked(true);
        ui->stepX->setText(QString::number(sMsg.getMoveStepX()));
        ui->posImpX->setText(QString::number(sMsg.getPosImpX()));
        break;
    case SerialMessage::START_Y:
        ui->cbPosStartGD->setChecked(true);
        break;
    case SerialMessage::END_Y:
        ui->cbPosKoniecGD->setChecked(true);
        ui->stepY->setText(QString::number(sMsg.getMoveStepY()));
        ui->posImpY->setText(QString::number(sMsg.getPosImpY()));
        break;
    case SerialMessage::END_XY:
        ui->cbPosKoniec->setChecked(true);
        ui->pbUstaw->setEnabled(true);
        break;
    case SerialMessage::START_R:
        ui->cbRolHomeStart->setChecked(true);
        break;
    case SerialMessage::END_R:
        ui->cbRolPosKoniec->setChecked(true);
        ui->pbRoletaUstaw->setEnabled(true);
        ui->stepR->setText(QString::number(sMsg.getMoveStepR()));
        ui->posStepR->setText(QString::number(sMsg.getPosStepR()));
        break;
    case SerialMessage::ERROR_XY:
        QMessageBox::critical(this, "Pozycja X/Y", "Nie udało się ustawić pozycji");
        ui->pbUstaw->setEnabled(true);
        break;
    case SerialMessage::ERROR_R:
        ui->pbRoletaUstaw->setEnabled(true);
        break;
    default:
        break;
    }
}

void MainWindow::homeDone(SerialMessage::StatusWork work)
{
    switch(work) {
    case SerialMessage::START_XY:
        ui->cbHomeStart->setChecked(true);
        ui->pbHome->setEnabled(false);
        break;
    case SerialMessage::START_X:
        ui->cbHomeStartLP->setChecked(true);
        break;
    case SerialMessage::END_X:
        ui->cbHomeKoniecLP->setChecked(true);
        ui->homeStepsX->setText(QString::number(sMsg.getMoveStepX()));
        break;
    case SerialMessage::START_Y:
        ui->cbHomeStartGD->setChecked(true);
        break;
    case SerialMessage::END_Y:
        ui->cbHomeKoniecGD->setChecked(true);
        ui->homeStepsY->setText(QString::number(sMsg.getMoveStepY()));
        break;
    case SerialMessage::END_XY:
        ui->cbHomeKoniec->setChecked(true);
        ui->pbHome->setEnabled(true);
        break;
    case SerialMessage::START_R:
        ui->cbRolHomeStart->setChecked(true);
        ui->pbRoletaHome->setEnabled(false);
        break;
    case SerialMessage::END_R:
        ui->cbRolHomeKoniec->setChecked(true);
        ui->pbRoletaHome->setEnabled(true);
        ui->homeStepsR->setText(QString::number(sMsg.getMoveStepR()));
        break;
    case SerialMessage::ERROR_XY:
        QMessageBox::critical(this, "Powrót do pozycji startowej", "Nie udało się ustawić pozycji bazowej");
        ui->pbHome->setEnabled(true);
        break;
    case SerialMessage::ERROR_R:
        break;
    default:
        break;
    }
}

void MainWindow::errorReadFromRadio()
{

}

void MainWindow::readFromRadio(int val)
{
    qDebug("readFromRadio %d", val);
}

void MainWindow::errorSerial(QString error)
{
    ui->pbHome->setEnabled(false);
    ui->pbUstaw->setEnabled(false);

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

void MainWindow::deviceName(QString portname)
{
    ui->portname->setText(portname);
}

void MainWindow::controllerOK()
{
    ui->statusserial->setText("Sterownik OK");
    ui->pbHome->setEnabled(true);
    ui->pbUstaw->setEnabled(true);
}

void MainWindow::setParamsDone()
{
    ui->statusparams->setText("Parametry ustawione");
}

void MainWindow::debug(QString text)
{
    ui->debug->appendPlainText(text);
}

void MainWindow::on_pbHome_clicked()
{
    ui->cbHomeStart->setChecked(false);
    ui->cbHomeStartLP->setChecked(false);
    ui->cbHomeKoniecLP->setChecked(false);
    ui->cbHomeStartGD->setChecked(false);
    ui->cbHomeKoniecGD->setChecked(false);
    ui->cbHomeKoniec->setChecked(false);
    ui->homeStepsX->setText("-");
    ui->homeStepsY->setText("-");
    emit setPositionHome();
}

void MainWindow::on_pbUstaw_clicked()
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

    emit setPosition(ui->pos_X->text().toUInt(), ui->pos_Y->text().toUInt());
}

void MainWindow::on_pbClose_clicked()
{
    ui->pbFindSerial->setEnabled(true);
    sMsg.closeDevice();
}

void MainWindow::on_pbSettings_clicked()
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
    emit setParams(reverseX, reverseY, reverseR, impX, impY, stepX, stepY, stepR);
}

void MainWindow::on_tbRoletaR_clicked()
{
    RoletaKroki * dlg = new RoletaKroki(&rr, ui->pos_R, this);
    dlg->exec();
}

void MainWindow::on_pbRoletaHome_clicked()
{
    ui->cbRolHomeKoniec->setChecked(false);
    ui->cbRolHomeStart->setChecked(false);
    ui->homeStepsR->setText("-");
    emit setRoletaHome();
}

void MainWindow::on_pbRoletaUstaw_clicked()
{
    if (ui->pos_R->text().isEmpty())
        return;
    bool ok;
    uint32_t steps = ui->pos_R->text().toULong(&ok);
    if (!ok)
        return;
    ui->cbRolPosKoniec->setChecked(false);
    ui->cbRolPosKoniec->setChecked(false);
    ui->pbRoletaUstaw->setEnabled(false);
    ui->stepR->setText("-");
    ui->posStepR->setText("-");
    emit setRoleta(steps);
}


void MainWindow::on_pbRadioOff_clicked()
{
    ui->pbRadioOff->setEnabled(false);
    ui->pbRadioOn->setEnabled(true);
    tmr.stop();

}


void MainWindow::on_pbRadioOn_clicked()
{
    ui->pbRadioOn->setEnabled(false);
    ui->pbRadioOff->setEnabled(true);
    tmr.start();
}

void MainWindow::radioTimeout()
{
    emit readRadio();
}

