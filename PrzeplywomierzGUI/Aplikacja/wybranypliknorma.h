#ifndef WYBRANYPLIKNORMA_H
#define WYBRANYPLIKNORMA_H

#include <QDialog>

#include "pozycje.h"

namespace Ui {
class WybranyPlikNorma;
}

class WybranyPlikNorma : public QDialog
{
    Q_OBJECT

public:
    explicit WybranyPlikNorma(QWidget *parent = nullptr, const QString & file = "", unsigned int etapMax = 15,
                              unsigned int timeStable = 3600, unsigned int maxR=1500, unsigned int width = 870,
                              unsigned int offsetx = 0, unsigned int offsety = 0);
    ~WybranyPlikNorma();

    PozycjeRol getList() const { return m_lista; }
    void reject();

protected:
    void parseFile();

private:
    Ui::WybranyPlikNorma *ui;
    QString filename;
    unsigned int etapMax;
    unsigned int timeStable;
    PozycjeRol m_lista;
    int prevNorma;
};

#endif // WYBRANYPLIK_H
