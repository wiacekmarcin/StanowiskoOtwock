#include "etapyroletdlg.h"
#include "ui_etapyroletdlg.h"

EtapyRoletDlg::EtapyRoletDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EtapyRoletDlg)
{
    ui->setupUi(this);
}

EtapyRoletDlg::~EtapyRoletDlg()
{
    delete ui;
}
