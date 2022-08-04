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
class Ustawienia;

QT_BEGIN_NAMESPACE
namespace Ui { class MiernikPrzeplywu; }
QT_END_NAMESPACE

class MiernikPrzeplywu : public QMainWindow
{
    Q_OBJECT

public:
    MiernikPrzeplywu(QWidget *parent = nullptr, WyborMetody::ModeWork mode = WyborMetody::MODE_SERVICE,
                     WyborMetody::MethodInsData method = WyborMetody::METHOD_NONE,
                     const QString & filename = "", unsigned int manPos = 0, unsigned manTimeStop = 0,
                     unsigned int widthPoz = 0, unsigned int heightPoz = 0, unsigned int autoTimeStop = 0,
                     unsigned int etapyNr = 0, unsigned int stableTime = 0, unsigned int timeMeas = 0);
    ~MiernikPrzeplywu();

    void closeEvent (QCloseEvent *event);

    void setUstawienia(Ustawienia * u);

protected:
    void chooseTab();
    void chooseMethod();
    void proceedChooseMethod();
    void calculateMechanika();
    QString addTime(QString status);
    void debugclear();

private slots:
    void on_tabWidget_currentChanged(int index);

    void on_pbUstaw_clicked();

    void errorSerial(QString);
    void successOpenDevice(bool);

    void readedFromRadio(int);
    void errorReadFromRadio();

    void deviceName(QString);
    void controllerOK();

    void positionStatus(SerialMessage::StatusWork work);
    void homeStatus(SerialMessage::StatusWork work);
    void setParamsDone();

    void doneHome();
    void errorHome();

    void donePosition();
    void debug(QString);

    void statusMiernik(QString);

    void end();
    void start();

    void noweDane();

    void on_pbZeruj_clicked();

    void on_pbWybierzStanowisko_clicked();

signals:
    void connectToDevice();
    void checkDevice();
    void setPositionHome();
    void setPosition(uint32_t x, uint32_t y);
    void setRoletaHome();
    void setRoleta(uint32_t r);
    void setParams(bool reverseX, bool reverseY, bool reverseR, uint32_t maxImpX, uint32_t maxImpY,
                                                                uint32_t maxStepX, uint32_t maxStepY,
                                                                uint32_t);


private:
    Ui::MiernikPrzeplywu *ui;
    WyborMetody::ModeWork modeWork;
    WyborMetody::MethodInsData methodIns;
    unsigned int manTimeStop;
    unsigned int manPos;
    unsigned int autoTimeStop;
    unsigned int widthPos;
    unsigned int heightPos;
    unsigned int etapyNr;
    unsigned int stableTime;
    unsigned int timeMeas;

    Pozycje m_lista;
    QString fileName;


    Ustawienia * ust;
    Ruch mech;

    MierzonePozycje * widget;
    MierzonePozycje * widget2700;
    MierzonePozycje * widget1000p;
    MierzonePozycje * widget1000l;
    PozycjeRoleta * widgetRoleta;

    SerialMessage sMsg;
    bool connIsOk;

    uint32_t impx;
    uint32_t impy;

    int impmmx;
    int impmmy;

    bool lockTab;
};
#endif // MIERNIKPRZEPLYWU_H

