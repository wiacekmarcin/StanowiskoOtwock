#include "roletakroki.h"
#include "ui_roletakroki.h"

RoletaKroki::RoletaKroki(RoletaRuch * rr, QLineEdit *txt, QLabel * mm_, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RoletaKroki),
    lineEdit(txt),
    ruch(rr),
    mmLab(mm_)
{
    ui->setupUi(this);
    connect(ui->rbStep, &QRadioButton::clicked, this, &RoletaKroki::rbStep_clicked);
    connect(ui->rbmm, &QRadioButton::clicked, this, &RoletaKroki::rbmm_clicked);
    connect(ui->rbPercent, &QRadioButton::clicked, this, &RoletaKroki::rbPercent_clicked);
    connect(ui->pbUstaw, &QPushButton::clicked, this, &RoletaKroki::pbUstaw_clicked);

    setWindowTitle("Wybierz pozycję rolety");
}

RoletaKroki::~RoletaKroki()
{
    delete ui;
}

void RoletaKroki::rbStep_clicked()
{
    ui->steps->setEnabled(ui->rbStep->isChecked());
    ui->percent->setEnabled(ui->rbPercent->isChecked());
    ui->mm->setEnabled(ui->rbmm->isChecked());
}


void RoletaKroki::rbmm_clicked()
{
    ui->steps->setEnabled(ui->rbStep->isChecked());
    ui->percent->setEnabled(ui->rbPercent->isChecked());
    ui->mm->setEnabled(ui->rbmm->isChecked());
}


void RoletaKroki::rbPercent_clicked()
{
    ui->steps->setEnabled(ui->rbStep->isChecked());
    ui->percent->setEnabled(ui->rbPercent->isChecked());
    ui->mm->setEnabled(ui->rbmm->isChecked());
}


void RoletaKroki::pbUstaw_clicked()
{
    if (ui->rbStep->isChecked()) {
        if (ui->steps->text().isEmpty())
            return;
        bool ok;
        unsigned long steps = ui->steps->text().toULong(&ok);

        if (!ok)
            return;
        //unsigned int mm = ruch->podniescMM(steps);
        for (unsigned int m = 0 ; m < ruch->getMaxMM(); m++) {
            if (ruch->podniescMM(m) > steps) {
                mmLab->setText(QString("~ %1 ").arg(m));
                break;
            }
        }
        lineEdit->setText(QString::number(steps));
        reject();
    } else if (ui->rbmm->isChecked()) {
        if (ui->mm->text().isEmpty())
            return;
        bool ok;
        unsigned int mm = ui->mm->text().toUInt(&ok);
        if (!ok)
            return;
        lineEdit->setText(QString::number(ruch->podniescMM(mm)));
        mmLab->setText(ui->mm->text());
        reject();
    } else if (ui->rbPercent->isChecked()) {
        if (ui->percent->text().isEmpty())
            return;
        bool ok;
        float p = ui->percent->text().toFloat(&ok);
        if (!ok)
            return;
        lineEdit->setText(QString::number(ruch->poniescPercent(0.01*p)));
        mmLab->setText(QString::number((int)(0.01*p*ruch->getMaxMM())));
        reject();
    }
}

