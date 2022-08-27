#ifndef ROLETACLOSE_H
#define ROLETACLOSE_H

#include <QDialog>
#include "serialdevice.h"
#include "mechanika.h"
#include "ustawienia.h"
#include "mechanika.h"
namespace Ui {
class RoletaClose;
}

class RoletaClose : public QDialog
{
    Q_OBJECT

public:
    explicit RoletaClose(const Ruch &r, const RoletaRuch & rr, SerialDevice * sd, QWidget *parent = nullptr);
    ~RoletaClose();

private slots:
    
    void setPositionDone(bool success, bool home, int work);
    void successOpenDevice(bool succ, int state);
    void setParamsDone(bool success);

    void debug(const QString &);
    void errorSerial(const QString &);

protected:
    void setCloseRoleta();
    void visibleFound(bool vis);
    void visibleIdent(bool vis);
    void visibleConf(bool vis);
    void visibleOpen(bool vis);
    
private:
    Ui::RoletaClose *ui;
    SerialDevice * sDev;
    Ruch mech;
    RoletaRuch mechRol;
};

#endif // ROLETACLOSE_H
