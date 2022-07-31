#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ustawienia.h"
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


    void on_pbSaveRadioCzujnik_clicked();

    void on_pbSaveRatio_clicked();

    void on_pbSaveWentylator_clicked();

    void on_pbRoletaSave_clicked();

private:
    Ui::MainWindow *ui;
    Ustawienia ust;
};

#endif // MAINWINDOW_H
