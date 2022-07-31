#ifndef IMPULSYDLG_H
#define IMPULSYDLG_H

#include <QDialog>
#include <QLineEdit>

namespace Ui {
class ImpulsyDlg;
}

class ImpulsyDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ImpulsyDlg(unsigned int imp2mm, unsigned int step2mm, const QString & title, QLineEdit * value, QWidget *parent = nullptr);
    ~ImpulsyDlg();
private slots:
    void pbUstaw_clicked();
    void rbmm_clicked();
    void rbImpulsy_clicked();
    void rbSteps_clicked();

protected:
    unsigned long steps2impulsy(unsigned long steps);
    unsigned long mm2impulsy(unsigned long mm);
private:
    Ui::ImpulsyDlg *ui;
    QLineEdit * lineEdit;
    unsigned int imp2mm;
    unsigned int step2mm;
};

#endif // IMPULSYDLG_H
