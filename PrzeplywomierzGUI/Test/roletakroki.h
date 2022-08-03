#ifndef ROLETAKROKI_H
#define ROLETAKROKI_H

#include <QDialog>
#include <QLineEdit>
#include "mechanika.h"
namespace Ui {
class RoletaKroki;
}

class RoletaKroki : public QDialog
{
    Q_OBJECT

public:

    explicit RoletaKroki(RoletaRuch * rr, QLineEdit *txt, QWidget *parent = nullptr);
    ~RoletaKroki();

private slots:
    void rbStep_clicked();
    void rbmm_clicked();
    void rbPercent_clicked();
    void pbUstaw_clicked();

private:
    Ui::RoletaKroki *ui;
    QLineEdit *lineEdit;
    RoletaRuch *ruch;
};

#endif // ROLETAKROKI_H
