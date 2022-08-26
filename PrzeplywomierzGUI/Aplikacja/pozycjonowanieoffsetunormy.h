#ifndef POZYCJONOWANIEOFFSETUNORMY_H
#define POZYCJONOWANIEOFFSETUNORMY_H

#include <QDialog>
#include "serialdevice.h"
#include "mechanika.h"
#include "ustawienia.h"
namespace Ui {
class PozycjonowanieOffsetuNormy;
}

class PozycjonowanieOffsetuNormy : public QDialog
{
    Q_OBJECT

public:
    explicit PozycjonowanieOffsetuNormy(Ustawienia &ust, SerialDevice * sd, bool left, unsigned int offsetX, unsigned int offsetY, QWidget *parent = nullptr);
    ~PozycjonowanieOffsetuNormy();

    int getOX() const { return oX; };
    int getOY() const { return oY; };

private slots:
    void rb1_clicked();
    void rb10_clicked();
    void rb100_clicked();
    void up();
    void down();
    void left();
    void right();

    void setPositionDone(bool success, bool home, int work);
    void successOpenDevice(bool succ, int state);
    void setParamsDone(bool success);

    void debug(const QString &);
    void errorSerial(const QString &);
protected:
    void setPos();

    void visibleFound(bool vis);
    void visibleIdent(bool vis);
    void visibleConf(bool vis);
    void visibleHomePos(bool vis);
    void visibleOpen(bool vis);
    void visibleProgres(bool vis);
private:
    Ui::PozycjonowanieOffsetuNormy *ui;
    int oX;
    int oY;
    SerialDevice * sDev;
    Ruch mech;
    RoletaRuch mechRol;
    unsigned int stepMM;
    bool reverse;


};

#endif // POZYCJONOWANIEOFFSETUNORMY_H
