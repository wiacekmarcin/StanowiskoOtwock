#include "podzialetapurolety.h"
#include "ui_podzialetapurolety.h"

PodzialEtapuRolety::PodzialEtapuRolety(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PodzialEtapuRolety)
{
    ui->setupUi(this);
}

PodzialEtapuRolety::~PodzialEtapuRolety()
{
    delete ui;
}
