#ifndef IMPULSYDLG_H
#define IMPULSYDLG_H

#include <QLabel>
#include <QDialog>
#include <QLineEdit>

namespace Ui {
class ImpulsyDlg;
}

class ImpulsyDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ImpulsyDlg(float imp2mm, float step2mm, const QString & title, QLineEdit * value, QLabel * lsteps, QLabel * lmm, QWidget *parent = nullptr);
    ~ImpulsyDlg();
private slots:
    void pbUstaw_clicked();
    void rbmm_clicked();
    void rbImpulsy_clicked();
    void rbSteps_clicked();

protected:
    unsigned long steps2impulsy(unsigned long steps);
    unsigned long impulsy2steps(unsigned long imps);
    unsigned long mm2impulsy(unsigned long mm);
    unsigned long impulsy2mm(unsigned long imps);
    unsigned long steps2mm(unsigned long steps);
    unsigned long mm2steps(unsigned long mm);
private:
    Ui::ImpulsyDlg *ui;
    QLineEdit * lineEdit;
    float imp2mm;
    float step2mm;
    QLabel * Lmm;
    QLabel * Lsteps;
};

#endif // IMPULSYDLG_H

