#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
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

    auto val8 = ust.getWentOffsetX();
    ui->ewiatrakx->setText(val8 == "" ? "301" : val8);

    auto val9 = ust.getWentOffsetY();
    ui->ewiatraky->setText(val9 == "" ? "656,8" : val9);

    auto val10 = ust.getRolDlugosc();
    ui->dlugoscRolety->setText(val10 == "" ? "1500" : val10);

    auto val11 = ust.getRolStepObrot();
    ui->roletaImpObrot->setText(val11 == "" ? "2000" : val11);

    auto val12 = ust.getStacOsXNazwa();
    ui->stac_osX_nazwa->setText(val12 == "" ? "2700" : val12);

    auto val13 = ust.getStacOsYNazwa();
    ui->stac_osY_nazwa->setText(val13 == "" ? "3000" : val13);

    auto val14 = ust.getOknoOsXNazwa();
    ui->okno_osX_nazwa->setText(val14 == "" ? "1000" : val14);

    auto val15 = ust.getOknoOsYNazwa();
    ui->okno_osY_nazwa->setText(val15 == "" ? "2000" : val15);

    auto val16 = ust.getRolOsXNazwa();
    ui->roleta_osX_nazwa->setText(val16 == "" ? "800" : val16);

    auto val17 = ust.getRolOsYNazwa();
    ui->roleta_osY_nazwa->setText(val17 == "" ? "1500" : val17);

    auto val18 = ust.getStacOsXReal();
    ui->stac_osX_real->setText(val18 == "" ? "2700" : val18);

    auto val19 = ust.getStacOsYReal();
    ui->stac_osY_real->setText(val19 == "" ? "3000" : val19);

    auto val20 = ust.getOknoOsXReal();
    ui->okno_osX_real->setText(val20 == "" ? "1000" : val20);

    auto val21 = ust.getOknoOsYReal();
    ui->okno_osY_real->setText(val21 == "" ? "2000" : val21);

    auto val22 = ust.getRolOsXReal();
    ui->roleta_osX_real->setText(val22 == "" ? "1200" : val22);

    auto val23 = ust.getRolOsYReal();
    ui->roleta_osY_real->setText(val23 == "" ? "1800" : val23);

    auto val24 = ust.getMaxRolKroki();
    ui->rolMaxKrokow->setText(val23 == "" ? "29100" : val24);


    float sum = 0.0;
    int row;
    int col;
    int steps = val11.toUInt();
    for (short i = 0 ; i < Ustawienia::getMaxRolObrot() ; ++i) {
        row = (i % 15) + 2;
        col = 5 * (i / 15);
        QLabel * lobrot = new QLabel(ui->scrollArea);
        lobrot->setObjectName(QString::fromUtf8("lobrot")+QString::number(i));
        lobrot->setText(QString("%1").arg(i+1));
        ui->gridLayout_3->addWidget(lobrot, row, col, 1, 1);

        QLabel * lStep = new QLabel(ui->scrollArea);
        lStep->setObjectName(QString::fromUtf8("lobrot")+QString::number(i));
        lStep->setText(QString::number((i+1)*steps));
        ui->gridLayout_3->addWidget(lStep, row, col + 1, 1, 1);

        QLineEdit *RolObrot = new QLineEdit(ui->scrollArea);
        RolObrotTab[i] = RolObrot;
        RolObrot->setObjectName(QString::fromUtf8("RolObrot")+QString::number(i));
        ui->gridLayout_3->addWidget(RolObrot, row, col + 2, 1, 1);

        RolObrot->setText(ust.getRolObrot(i));
        sum += ust.getRolObrot(i).toFloat();

        QLabel * lobrotSum = new QLabel(ui->scrollArea);
        lobrotSum->setObjectName(QString::fromUtf8("lobrot")+QString::number(i));
        lobrotSum->setText(QString::number(sum));
        ui->gridLayout_3->addWidget(lobrotSum, row, col + 3, 1, 1);
        sumLabel[i] = lobrotSum;
    }

    /*
    QFrame * line_7 = new QFrame(ui->scrollArea);
    line_7->setObjectName(QString::fromUtf8("line_7"));
    line_7->setFrameShape(QFrame::HLine);
    line_7->setFrameShadow(QFrame::Sunken);

    ui->gridLayout_3->addWidget(line_7, 20, 0, 2, 1);

    QPushButton * pbRoletaSave = new QPushButton(ui->scrollArea);
    pbRoletaSave->setObjectName(QString::fromUtf8("pbRoletaSave"));
    pbRoletaSave->setText(QCoreApplication::translate("MainWindow", "Zapisz", nullptr));
    ui->gridLayout_3->addWidget(pbRoletaSave, 35, 0, 1, 1);
*/
    on_pbSaveRadioCzujnik_clicked();
    on_pbSaveRatio_clicked();
    on_pbSaveWentylator_clicked();
    on_pbRoletaSave_clicked();
    on_pbSaveRatio_clicked();
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
    ust.setWentOffsetX(ui->ewiatrakx->text());
    ust.setWentOffsetY(ui->ewiatraky->text());
}

void MainWindow::on_pbRoletaSave_clicked()
{
    for (short i = 0 ; i < Ustawienia::getMaxRolObrot(); ++i) {
        ust.setRolObrot(i, RolObrotTab[i]->text());
    }
    ust.setMaxRolKroki(ui->rolMaxKrokow->text());
    ust.setRolDlugosc(ui->dlugoscRolety->text());
    ust.setRolStepObrot(ui->roletaImpObrot->text());
}


void MainWindow::on_pbSaveWymiary_clicked()
{
    ust.setStacOsXNazwa(ui->stac_osX_nazwa->text());
    ust.setStacOsYNazwa(ui->stac_osY_nazwa->text());
    ust.setOknoOsXNazwa(ui->okno_osX_nazwa->text());
    ust.setOknoOsYNazwa(ui->okno_osY_nazwa->text());
    ust.setRolOsXNazwa(ui->roleta_osX_nazwa->text());
    ust.setRolOsYNazwa(ui->roleta_osY_nazwa->text());
    ust.setStacOsXReal(ui->stac_osX_real->text());
    ust.setStacOsYReal(ui->stac_osY_real->text());
    ust.setOknoOsXReal(ui->okno_osX_real->text());
    ust.setOknoOsYReal(ui->okno_osY_real->text());
    ust.setRolOsXReal(ui->roleta_osX_real->text());
    ust.setRolOsYReal(ui->roleta_osY_real->text());
}


void MainWindow::on_pbcalculate_clicked()
{
    float sum = 0.0;
    for (short i = 0 ; i < Ustawienia::getMaxRolObrot(); ++i) {
        sum += RolObrotTab[i]->text().toFloat();
        sumLabel[i]->setText(QString::number(sum));
    }
}

