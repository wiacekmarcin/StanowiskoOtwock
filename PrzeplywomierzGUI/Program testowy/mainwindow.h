#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serialmessage.h"
#include "mechanika.h"
#include "ustawienia.h"

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

private slots:
    void pbFindSerial_clicked();

    void connectedToPort(QString);

    void positionDone(SerialMessage::StatusWork);
    void homeDone(SerialMessage::StatusWork);

    void errorReadFromRadio();
    void readFromRadio(int val);

    void errorSerial(QString);
    void successOpenDevice(bool);

    void deviceName(QString);
    void controllerOK();

    void setParamsDone();

    void debug(QString);

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

signals:
    void connectToDevice();
    void checkDevice();
    void setPositionHome();
    void setPosition(uint32_t x, uint32_t y);
    void setRoletaHome();
    void setRoleta(uint32_t r);

    void setParams(bool reverseX, bool reverseY, bool reverseR,
                   uint32_t maxImpX, uint32_t maxImpY, uint32_t maxStepX, uint32_t maxStepY,
                   uint32_t maxStepR);

    void readRadio();

private:
    Ui::MainWindow *ui;
    SerialMessage sMsg;
    Ruch rpos;
    RoletaRuch rr;
    Ustawienia ust;
    QTimer tmr;
};

#endif // MAINWINDOW_H
