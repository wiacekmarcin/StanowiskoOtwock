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
    explicit WybranyPlikNorma(QWidget *parent, bool reverse, const QString & file, unsigned int etapMax,
                              unsigned int timeStable, unsigned int maxR, unsigned int width,
                              unsigned int offsetx, unsigned int offsety);
    ~WybranyPlikNorma();

    PozycjeRol getList() const { return m_lista; }
    void reject();

protected:
    void parseFile();

private:
    Ui::WybranyPlikNorma *ui;
    bool reverse;
    QString filename;
    unsigned int etapMax;
    unsigned int timeStable;
    PozycjeRol m_lista;
    int prevNorma;
};

#endif // WYBRANYPLIK_H
