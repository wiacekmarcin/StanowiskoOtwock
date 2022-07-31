#include "mainwindow.h"
#include "ui_mainwindow.h"

#define setRolObrot(N, R, M) auto val##N = ust.getRolObrot##R(); ui->RolObrot##R->setText(val##N == "" ? QString::number(M) : val##N)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Program serwisowy");
    ust.read();
    auto val1 = ust.getImpulsyXperMM();
    ui->eimpX->setText(val1 == "" ? "250" : val1);

    auto val2 = ust.getImpulsyYperMM();
    ui->eimpY->setText(val2 == "" ? "250" : val2);

    auto val3 = ust.getKrokiXperMM();
    ui->estepX->setText(val3 == "" ? "100" : val3);

    auto val4 = ust.getKrokiYperMM();
    ui->estepY->setText(val4 == "" ? "100" : val4);

    auto val5 = ust.getRatioCzujnik1();
    ui->ecz1ratio->setText(val5 == "" ? "0.01" : val5);

    auto val6 = ust.getUnitCzujnik1();
    ui->ecz1unit->setText(val6 == "" ? "m/s" : val6);

    auto val7 = ust.getKatnachylenia();
    ui->ewiatrakkat->setText(val7 == "" ? "3.73" : val7);

    auto val8 = ust.getOffsetX();
    ui->ewiatrakx->setText(val8 == "" ? "301" : val8);

    auto val9 = ust.getOffsetY();
    ui->ewiatraky->setText(val9 == "" ? "656,8" : val9);

    auto val10 = ust.getRolDlugosc();
    ui->dlugoscRolety->setText(val10 == "" ? "1700" : val10);

    auto val11 = ust.getRolImpObrot();
    ui->roletaImpObrot->setText(val11 == "" ? "12000" : val11);

//101.3, 105.9, 108.9, 112.4, 115.8, 118.6, 121.9, 125.5, 128.4, 132.1, 135, 138.7, 141.9


    setRolObrot(12, 1, 101.3);
    setRolObrot(13, 2, 101.3+105.9);
    setRolObrot(14, 3, 101.3+105.9+108.9);
    setRolObrot(15, 4, 101.3+105.9+108.9+112.4);
    setRolObrot(16, 5, 101.3+105.9+108.9+112.4+115.8);
    setRolObrot(17, 6, 101.3+105.9+108.9+112.4+115.8+118.6);
    setRolObrot(18, 7, 101.3+105.9+108.9+112.4+115.8+118.6+121.9);
    setRolObrot(19, 8, 101.3+105.9+108.9+112.4+115.8+118.6+121.9+125.5);
    setRolObrot(20, 8, 101.3+105.9+108.9+112.4+115.8+118.6+121.9+125.5+128.4);
    setRolObrot(21, 9, 101.3+105.9+108.9+112.4+115.8+118.6+121.9+125.5+128.4+123.1);
    setRolObrot(22, 10, 101.3+105.9+108.9+112.4+115.8+118.6+121.9+125.5+128.4+123.1+135);
    setRolObrot(23, 11, 101.3+105.9+108.9+112.4+115.8+118.6+121.9+125.5+128.4+123.1+135+138.7);
    setRolObrot(24, 12, 101.3+105.9+108.9+112.4+115.8+118.6+121.9+125.5+128.4+123.1+135+138.7+141.9);

    ui->lobrot13->setVisible(false);
    ui->RolObrot13->setVisible(false);

    on_pbSaveRadioCzujnik_clicked();
    on_pbSaveRatio_clicked();
    on_pbSaveWentylator_clicked();
    on_pbRoletaSave_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pbSaveRadioCzujnik_clicked()
{
    ust.setRatioCzujnik1(ui->ecz1ratio->text());
    ust.setUnitCzujnik1(ui->ecz1unit->text());
}

void MainWindow::on_pbSaveRatio_clicked()
{
    ust.setImpulsyXperMM(ui->eimpX->text());
    ust.setImpulsyYperMM(ui->eimpY->text());
    ust.setKrokiXperMM(ui->estepX->text());
    ust.setKrokiYperMM(ui->estepY->text());
}

void MainWindow::on_pbSaveWentylator_clicked()
{
    ust.setKatnachylenia(ui->ewiatrakkat->text());
    ust.setOffsetX(ui->ewiatrakx->text());
    ust.setOffsetY(ui->ewiatraky->text());
}

void MainWindow::on_pbRoletaSave_clicked()
{
    ust.setRolObrot1(ui->RolObrot1->text());
    ust.setRolObrot2(ui->RolObrot2->text());
    ust.setRolObrot3(ui->RolObrot3->text());
    ust.setRolObrot4(ui->RolObrot4->text());
    ust.setRolObrot5(ui->RolObrot5->text());
    ust.setRolObrot6(ui->RolObrot6->text());
    ust.setRolObrot7(ui->RolObrot7->text());
    ust.setRolObrot8(ui->RolObrot8->text());
    ust.setRolObrot9(ui->RolObrot9->text());
    ust.setRolObrot10(ui->RolObrot10->text());
    ust.setRolObrot11(ui->RolObrot11->text());
    ust.setRolObrot12(ui->RolObrot12->text());
    ust.setRolObrot13(ui->RolObrot13->text());
}

