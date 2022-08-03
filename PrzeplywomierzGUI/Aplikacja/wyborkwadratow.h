#ifndef WYBORKWADRATOW_H
#define WYBORKWADRATOW_H

#include <QDialog>
#include <QPair>
#include <QMap>

#include "pozycje.h"

class QPushButton;

namespace Ui {
class WyborKwadratow;
}

class WyborKwadratow : public QDialog
{
    Q_OBJECT

public:
    explicit WyborKwadratow(QWidget *parent = nullptr, int cols = 1, int rows = 1, unsigned int time = 10, unsigned int maxX = 2700, unsigned int maxY = 3000);
    ~WyborKwadratow();

    Pozycje getList() const { return m_lista; }

private slots:
    void on_pbBack_clicked();

    void on_pbClear_clicked();

    void wybranyKwadrat();

private:
    Ui::WyborKwadratow *ui;
    QMap<QPushButton*, QPair<int, int> > m_kwadraty;
    Pozycje m_lista;

    unsigned int maxX;
    unsigned int maxY;
    unsigned int posX;
    unsigned int posY;
    unsigned int timeStop;

    QPushButton * lastChoose;


};

#endif // WYBORKWADRATOW_H
