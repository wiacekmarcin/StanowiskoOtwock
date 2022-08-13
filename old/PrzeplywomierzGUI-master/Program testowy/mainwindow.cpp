#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&sMsg, SIGNAL(startingPosition()), this, SLOT(startingPosition()));
    connect(&sMsg, SIGNAL(startingPositionX()), this, SLOT(startingPositionX()));
    connect(&sMsg, SIGNAL(donePositionX()), this, SLOT(donePositionX()));
    connect(&sMsg, SIGNAL(startingPositionY()), this, SLOT(startingPositionY()));
    connect(&sMsg, SIGNAL(donePositionY()), this, SLOT(donePositionY()));
    connect(&sMsg, SIGNAL(donePosition()), this, SLOT(donePosition()));

    connect(&sMsg, SIGNAL(errorReadFromRadio()), this, SLOT(errorReadFromRadio()));
    connect(&sMsg, SIGNAL(readFromRadio(int)), this, SLOT(readFromRadio(int)));

    connect(&sMsg, SIGNAL(startingHome()), this, SLOT(startingHome()));
    connect(&sMsg, SIGNAL(startingHomeX()), this, SLOT(startingHomeX()));
    connect(&sMsg, SIGNAL(doneHomeX()), this, SLOT(doneHomeX()));
    connect(&sMsg, SIGNAL(startingHomeY()), this, SLOT(startingHomeY()));
    connect(&sMsg, SIGNAL(doneHomeY()), this, SLOT(doneHomeY()));
    connect(&sMsg, SIGNAL(doneHome()), this, SLOT(doneHome()));
    connect(&sMsg, SIGNAL(errorHome()), this, SLOT(errorHome()));

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

    connect(this, SIGNAL(setParams(bool,bool,uint32_t,uint32_t,uint32_t,uint32_t)), &sMsg, SLOT(setParams(bool,bool,uint32_t,uint32_t,uint32_t,uint32_t)));

    ui->pbHome->setEnabled(false);
    ui->pbUstaw->setEnabled(false);

    numhomestart = numhomedone = numxhomestart = numxhomedone = numyhomestart = numyhomedone = 0;

    ui->maxImpx->setText("1000");
    ui->maxImpY->setText("1500");

    ui->maxStepX->setText("200");
    ui->maxStepY->setText("300");
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

void MainWindow::startingPosition()
{
    ui->startpos->setText(QString("%1").arg(++numposstart));
}

void MainWindow::startingPositionX()
{
    ui->startposx->setText(QString("%1").arg(++numxposstart));
}

void MainWindow::donePositionX()
{
    ui->doneposx->setText(QString("%1").arg(++numxposdone));
}

void MainWindow::startingPositionY()
{
    ui->startposy->setText(QString("%1").arg(++numyposstart));
}

void MainWindow::donePositionY()
{
    ui->doneposy->setText(QString("%1").arg(++numyposdone));
}

void MainWindow::donePosition()
{
    ui->donepos->setText(QString("%1").arg(++numposdone));
}

void MainWindow::errorReadFromRadio()
{

}

void MainWindow::readFromRadio(int val)
{
    qDebug("readFromRadio %d", val);
}

void MainWindow::startingHome()
{
    ui->startinghome->setText(QString("%1").arg(++numhomestart));
}

void MainWindow::startingHomeX()
{
    ui->startinghomex->setText(QString("%1").arg(++numxhomestart));
}

void MainWindow::doneHomeX()
{
    ui->donehomex->setText(QString("%1").arg(++numxhomedone));
}

void MainWindow::startingHomeY()
{
    ui->startinghomey->setText(QString("%1").arg(++numyhomestart));
}

void MainWindow::doneHomeY()
{
    ui->donehomey->setText(QString("%1").arg(++numyhomedone));
}

void MainWindow::doneHome()
{
    ui->donehome->setText(QString("%1").arg(++numhomedone));

}

void MainWindow::errorHome()
{
    ui->errorhome->setText("Error");
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
    numhomestart = numhomedone = numxhomestart = numxhomedone = numyhomestart = numyhomedone = 0;
    ui->startinghome->setText(QString("0"));
    ui->startinghomex->setText(QString("0"));
    ui->donehomex->setText(QString("0"));
    ui->startinghomey->setText(QString("0"));
    ui->donehomex->setText(QString("0"));
    ui->donehome->setText(QString("0"));
    emit setPositionHome();
}

void MainWindow::on_pbUstaw_clicked()
{
    numposstart = numposdone = numxposstart = numxposdone = numyposstart = numyposdone = 0;
    ui->startpos->setText(QString("0"));
    ui->startposx->setText(QString("0"));
    ui->doneposx->setText(QString("0"));
    ui->startposy->setText(QString("0"));
    ui->doneposy->setText(QString("0"));
    ui->donepos->setText(QString("0"));
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
    int32_t impX = ui->maxImpx->text().toUInt(&ok);
    if (!ok)
        impX = 0;

    int32_t impY = ui->maxImpY->text().toUInt(&ok);
    if (!ok)
        impY = 0;

    int32_t stepX = ui->maxStepX->text().toUInt(&ok);
    if (!ok)
        stepX = 0;

    int32_t stepY = ui->maxStepY->text().toUInt(&ok);
    if (!ok)
        stepY = 0;

    bool reverseX = ui->dirX->isChecked();

    bool reverseY = ui->dirY->isChecked();
    ui->statusparams->setText("Trwa ustawianie");
    emit setParams(reverseX, reverseY, impX, impY, stepX, stepY);
}

