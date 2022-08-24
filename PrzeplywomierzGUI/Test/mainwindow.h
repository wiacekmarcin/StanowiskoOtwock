#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../Aplikacja/serialdevice.h"
#include "../Aplikacja/mechanika.h"
#include "../Aplikacja/ustawienia.h"

#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    typedef enum _ModeWork {
        MODE_NONE = -1,
        MODE_2700,                  //przestrzen robocza 2700x3000
        MODE_1000P,                 //przestrzen robocza prawa 1000x2000
        MODE_1000L,                 //przestrzen robocza lewa 1000x2000
        MODE_FUNSET,                //ustawianie wiatraka
        MODE_SERVICE,               //serwis
        MODE_ROLETAP,               //roleta po prawej stronie
        MODE_ROLETAL,               //roleta po lewej stronie
    } ModeWork;


private slots:

    void errorSerial(const QString &);
    void setPositionDone(bool success, bool home, int work);
    void kontrolerConfigured(bool success, int state);
    void readedFromRadio(bool sucess, int val1 , int val2, int val3, int val4);
    void deviceName(const QString &portname);
    void setParamsDone(bool success);

protected:
    void positionStatus(int work);
    void homeStatus(int work);

    void errorHome();
    void errorPosition();
    void errorHomeRoleta();
    void errorRoleta();
    void roletaHome();

    void debug(QString);

private slots:
    void pbFindSerial_clicked();
    void pbHome_clicked();
    void pbUstaw_clicked();
    void pbClose_clicked();
    void pbSettings_clicked();
    void tbRoletaR_clicked();
    void pbRoletaHome_clicked();
    void pbRoletaUstaw_clicked();
    void pbRadioOff_clicked();
    void pbRadioOn_clicked();
    void tbX_clicked();
    void tbY_clicked();
    void radioTimeout();
    void rbStac_clicked();
    void rbOknoPrawe_clicked();
    void rbOknoLewe_clicked();
    void rbRoletaPrawe_clicked();
    void rbRoletaLewe_clicked();
    void pbStop_clicked();
    void pbSettings2_clicked();
    void changeTimeObroty();
    void changeRoletaPostoj();
    
protected:
    void connectToDevice();

    void setPositionHome();
    void setPosition(uint32_t x, uint32_t y);
    void setRoletaHome();
    void setRoleta(uint32_t r);

    void setParams(bool reverseX, bool reverseY, bool reverseR, uint32_t maxImpX, uint32_t maxImpY,
        uint32_t maxStepX, uint32_t maxStepY, 
        uint32_t maxStepR, uint16_t minStepR, 
        uint16_t speedHome, uint16_t speedPos);

    void readRadio();

    void positionDone(int work);
    void homeDone(int work);

    void wyboStanowiska();
    QString addTime(QString status);
private:
    Ui::MainWindow *ui;
    SerialDevice sMsg;
    Ruch rpos;
    RoletaRuch rr;

    Ustawienia ust;
    QTimer tmr;

    ModeWork modeWork;
    QThread thSterownik;
};

#endif // MAINWINDOW_H
