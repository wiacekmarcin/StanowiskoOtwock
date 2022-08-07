#ifndef MIERNIKPRZEPLYWU_H
#define MIERNIKPRZEPLYWU_H

#include <QMainWindow>
#include <QString>
#include <QList>

#include "wybormetody.h"
#include "pozycje.h"
#include "mechanika.h"
#include "serialmessage.h"

class QCloseEvent;
class MierzonePozycje;
class QSerialPortInfo;
class PozycjeRoleta;
class Wentylator;
class Ustawienia;
class TabWidget;

QT_BEGIN_NAMESPACE
namespace Ui { class MiernikPrzeplywu; }
QT_END_NAMESPACE

class MiernikPrzeplywu : public QMainWindow
{
    Q_OBJECT

public:
    MiernikPrzeplywu(Ustawienia & ust);
    ~MiernikPrzeplywu();

    void closeEvent (QCloseEvent *event);

    bool chooseMethod(const WyborMetody::ModeWork &modeWork, const WyborMetody::MethodInsData &methodIns, const WyborMetodyData &values);
    WyborMetody::ModeWork getModeWork() const;

    WyborMetody::MethodInsData getMethodIns() const;

    const WyborMetodyData &getData() const;

    void connectToDevice();

    void setPositionHome();
    void setPosition(uint32_t x, uint32_t y);
    void setRoletaHome();
    void setRoleta(uint32_t r);
    void setParams(bool reverseX, bool reverseY, bool reverseR, uint32_t maxImpX, uint32_t maxImpY,
                                                                uint32_t maxStepX, uint32_t maxStepY,
                                                                uint32_t maxStepR);
    void readRadio();

    void noweDane();
public slots:
    void debug(const QString &);
    void debugClear();


protected:
    void chooseTab();

    void calculateMechanika();
    QString addTime(QString status);


    void setUstawienia();
private slots:


    void errorSerial(const QString &);
    void successOpenDevice(bool);

    void readedFromRadio(int);
    void errorReadFromRadio();

    void deviceName(QString);
    void controllerOK();

    void positionStatus(SerialMessage::StatusWork work);
    void homeStatus(SerialMessage::StatusWork work);
    void setParamsDone();

    void errorHome();
    void errorHomeRoleta();

    void connectAndConfigureDone();

signals:
    void connectAndConfigureSig();
    void connectToDeviceSig();
    void setPositionHomeSig();
    void setPositionSig(uint32_t x, uint32_t y);
    void setRoletaHomeSig();
    void setRoletaSig(uint32_t r);
    void setParamsSig(bool reverseX, bool reverseY, bool reverseR, uint32_t maxImpX, uint32_t maxImpY,
                                                                uint32_t maxStepX, uint32_t maxStepY,
                                                                uint32_t);
    void readRadioSig();


private:
    Ui::MiernikPrzeplywu *ui;
    WyborMetody::ModeWork modeWork;
    WyborMetody::MethodInsData methodIns;
    WyborMetodyData data;

    Ustawienia & ust;
    Ruch mech;

    TabWidget * widget;
    MierzonePozycje * widget2700;
    MierzonePozycje * widget1000p;
    MierzonePozycje * widget1000l;
    PozycjeRoleta * widgetRoleta;
    Wentylator *  widgetWentylator;

    SerialMessage sMsg;
    bool connIsOk;

    //uint32_t impx;
    //uint32_t impy;

    //int impmmx;
    //int impmmy;
    void chooseWork();
};
#endif // MIERNIKPRZEPLYWU_H

