#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serialmessage.h"
#include "mechanika.h"

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
    void on_pbFindSerial_clicked();

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

    void on_pbHome_clicked();
    void on_pbUstaw_clicked();

    void on_pbClose_clicked();
    void on_pbSettings_clicked();
    void on_tbRoletaR_clicked();

    void on_pbRoletaHome_clicked();

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
private:
    Ui::MainWindow *ui;
    SerialMessage sMsg;
    Ruch rpos;
    RoletaRuch rr;
};

#endif // MAINWINDOW_H
