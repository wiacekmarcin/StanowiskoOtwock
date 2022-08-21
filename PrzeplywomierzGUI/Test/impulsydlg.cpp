#include "impulsydlg.h"
#include "ui_impulsydlg.h"

ImpulsyDlg::ImpulsyDlg(float imp2mm_, float step2mm_, const QString & title,
                       QLineEdit * value, QLabel * lsteps, QLabel * lmm, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImpulsyDlg),
    lineEdit(value),
    imp2mm(imp2mm_),
    step2mm(step2mm_),
    Lmm(lmm),
    Lsteps(lsteps)
{
    ui->setupUi(this);
    connect(ui->pbUstaw, &QPushButton::clicked, this, &ImpulsyDlg::pbUstaw_clicked);
    connect(ui->rbmm, &QRadioButton::clicked, this, &ImpulsyDlg::rbmm_clicked);
    connect(ui->rbImpulsy, &QRadioButton::clicked, this, &ImpulsyDlg::rbImpulsy_clicked);
    connect(ui->rbSteps, &QRadioButton::clicked, this, &ImpulsyDlg::rbSteps_clicked);

    setWindowTitle(title);
}

ImpulsyDlg::~ImpulsyDlg()
{
    delete ui;
}

void ImpulsyDlg::pbUstaw_clicked()
{
    if (ui->rbSteps->isChecked()) {
        if (ui->steps->text().isEmpty())
            return;
        bool ok;
        unsigned long steps = ui->steps->text().toULong(&ok);
        if (!ok)
            return;

        lineEdit->setText(QString::number(steps2impulsy(steps)));
        Lsteps->setText(ui->steps->text());
        Lmm->setText(QString::number(steps2mm(steps)));
        reject();
    } else if (ui->rbmm->isChecked()) {
        if (ui->mm->text().isEmpty())
            return;
        bool ok;
        unsigned int mm = ui->mm->text().toUInt(&ok);
        if (!ok)
            return;
        lineEdit->setText(QString::number(mm2impulsy(mm)));
        Lmm->setText(ui->mm->text());
        Lsteps->setText(QString::number(mm2steps(mm)));
        reject();
    } else if (ui->rbImpulsy->isChecked()) {
        if (ui->impulsy->text().isEmpty())
            return;
        bool ok;
        unsigned long imp = ui->impulsy->text().toULong(&ok);
        if (!ok)
            return;
        lineEdit->setText(QString::number(imp));
        Lmm->setText(QString::number(impulsy2mm(imp)));
        Lsteps->setText(QString::number(impulsy2steps(imp)));
        reject();
    }
}

void ImpulsyDlg::rbmm_clicked()
{
    ui->mm->setEnabled(ui->rbmm->isChecked());
    ui->steps->setEnabled(ui->rbSteps->isChecked());
    ui->impulsy->setEnabled(ui->rbImpulsy->isChecked());
}

void ImpulsyDlg::rbImpulsy_clicked()
{
    ui->mm->setEnabled(ui->rbmm->isChecked());
    ui->steps->setEnabled(ui->rbSteps->isChecked());
    ui->impulsy->setEnabled(ui->rbImpulsy->isChecked());
}

void ImpulsyDlg::rbSteps_clicked()
{
    ui->mm->setEnabled(ui->rbmm->isChecked());
    ui->steps->setEnabled(ui->rbSteps->isChecked());
    ui->impulsy->setEnabled(ui->rbImpulsy->isChecked());
}

unsigned long ImpulsyDlg::steps2impulsy(unsigned long steps)
{
    return imp2mm*steps/step2mm;
}

unsigned long ImpulsyDlg::impulsy2steps(unsigned long imps)
{
    return step2mm*imps/imp2mm;
}

unsigned long ImpulsyDlg::mm2impulsy(unsigned long mm)
{
    return mm*imp2mm;
}

unsigned long ImpulsyDlg::impulsy2mm(unsigned long imps)
{
    return imps/imp2mm;
}

unsigned long ImpulsyDlg::steps2mm(unsigned long steps)
{
    return steps/step2mm;
}

unsigned long ImpulsyDlg::mm2steps(unsigned long mm)
{
    return mm*step2mm;
}

