#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serialmessage.h"
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
    void startingPosition();
    void startingPositionX();
    void donePositionX();
    void startingPositionY();
    void donePositionY();
    void donePosition();
    void errorReadFromRadio();
    void readFromRadio(int val);

    void startingHome();
    void startingHomeX();
    void doneHomeX();
    void startingHomeY();
    void doneHomeY();
    void doneHome();
    void errorHome();

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

signals:
    void connectToDevice();
    void checkDevice();
    void setPositionHome();
    void setPosition(uint32_t x, uint32_t y);

    void setParams(bool reverseX, bool reverseY, uint32_t maxImpX, uint32_t maxImpY, uint32_t maxStepX, uint32_t maxStepY);
private:
    Ui::MainWindow *ui;
    SerialMessage sMsg;
    unsigned int numhomestart, numhomedone, numxhomestart, numxhomedone, numyhomestart, numyhomedone;
    unsigned int numposstart, numposdone, numxposstart, numxposdone, numyposstart, numyposdone;
};

#endif // MAINWINDOW_H
