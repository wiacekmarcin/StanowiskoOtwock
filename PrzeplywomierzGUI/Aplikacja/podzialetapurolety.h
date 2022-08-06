#ifndef PODZIALETAPUROLETY_H
#define PODZIALETAPUROLETY_H

#include "pozycje.h"

#include <QDialog>
class QSpinBox;
namespace Ui {
class PodzialEtapuRolety;
}

class PodzialEtapuRolety : public QDialog
{
    Q_OBJECT

public:
    explicit PodzialEtapuRolety(QWidget *parent, unsigned int nrEtapow,
                                unsigned int iloscProbek, unsigned int czasStablizacji,
                                unsigned int maxR_, unsigned int width_,
                                unsigned int offsetX_, unsigned int offsetY_);
    ~PodzialEtapuRolety();

    const PozycjeRol &getLista() const;

protected:
    void jedenWidget(unsigned int etap);
private slots:
    void on_buttonBox_accepted();

private:
    Ui::PodzialEtapuRolety *ui;
    unsigned int nrEtapow;
    unsigned int iloscProbek;
    unsigned int czasStablizacji;
    unsigned int maxR;

    QSpinBox * xspinbox[15];
    QSpinBox * yspinbox[15];

    PozycjeRol pozycje;

};

#endif // PODZIALETAPUROLETY_H