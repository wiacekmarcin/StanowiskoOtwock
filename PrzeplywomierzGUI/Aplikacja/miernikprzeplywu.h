#ifndef MIERNIKPRZEPLYWU_H
#define MIERNIKPRZEPLYWU_H

#include <QMainWindow>
#include <QString>
#include <QList>
#include <QThread>

#include "wybormetody.h"
#include "pozycje.h"
#include "mechanika.h"
#include "serialdevice.h"

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
    explicit MiernikPrzeplywu();
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
    void setClose(bool afterBase);
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
    void setPositionDone(bool success, bool home, int work);
    void kontrolerConfigured(bool success, int state);
    void readedFromRadio(bool sucess, int val1 , int val2, int val3, int val4);
    void deviceName(const QString &);
    void setParamsDone(bool success);

protected:
    void positionStatus(SerialMessage::StatusWork work);
    void homeStatus(SerialMessage::StatusWork work);

    void errorHome();
    void errorPosition();
    void errorHomeRoleta();
    void errorRoleta();
    void errorReadFromRadio();
    void positionHome();
    void roletaHome();


private:

    void chooseWork();
    void enableStatusWidget(bool enable);

private:

    Ui::MiernikPrzeplywu *ui;
    WyborMetody::ModeWork modeWork;
    WyborMetody::MethodInsData methodIns;
    WyborMetodyData data;

    Ustawienia ust;
    Ruch mech;
    RoletaRuch mechR;

    TabWidget * widget;
    /*MierzonePozycje * widget2700;
    MierzonePozycje * widget1000p;
    MierzonePozycje * widget1000l;
    PozycjeRoleta * widgetRoletal;
    PozycjeRoleta * widgetRoletap;
    Wentylator *  widgetWentylator;*/

    SerialDevice sMsg;


    bool firstRun;
    bool firstRun2;
    bool checkRadio;

    QString m_portName;
    QThread thSterownik;
};
#endif // MIERNIKPRZEPLYWU_H

