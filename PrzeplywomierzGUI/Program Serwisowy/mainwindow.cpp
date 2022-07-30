#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

    on_pbSaveRadioCzujnik_clicked();
    on_pbSaveRatio_clicked();
    on_pbSaveWentylator_clicked();
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
