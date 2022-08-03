#ifndef WYBRANYPLIK_H
#define WYBRANYPLIK_H

#include <QDialog>

#include "pozycje.h"

namespace Ui {
class WybranyPlik;
}

class WybranyPlik : public QDialog
{
    Q_OBJECT

public:
    explicit WybranyPlik(QWidget *parent = nullptr, const QString & file = "", unsigned int maxNumber = 2700, unsigned int maxNumber2 = 3000);
    ~WybranyPlik();

    Pozycje getList() const { return m_lista; }
    void reject();

protected:
    void parseFile();

private:
    Ui::WybranyPlik *ui;
    QString filename;
    unsigned int maxVal1;
    unsigned int maxVal2;
    Pozycje m_lista;
};

#endif // WYBRANYPLIK_H
