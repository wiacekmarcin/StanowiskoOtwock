#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../Aplikacja/ustawienia.h"
namespace Ui {
class MainWindow;
}
class QLineEdit;
class QLabel;

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

    void on_pbSaveWymiary_clicked();

    void on_pbcalculate_clicked();

private:
    Ui::MainWindow *ui;
    Ustawienia ust;
    QLineEdit *RolObrotTab[30];
    QLabel *sumLabel[30];
};

#endif // MAINWINDOW_H
