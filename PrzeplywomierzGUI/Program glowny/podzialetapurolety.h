#ifndef PODZIALETAPUROLETY_H
#define PODZIALETAPUROLETY_H

#include <QDialog>

namespace Ui {
class PodzialEtapuRolety;
}

class PodzialEtapuRolety : public QDialog
{
    Q_OBJECT

public:
    explicit PodzialEtapuRolety(QWidget *parent = 0);
    ~PodzialEtapuRolety();

private:
    Ui::PodzialEtapuRolety *ui;
};

#endif // PODZIALETAPUROLETY_H
