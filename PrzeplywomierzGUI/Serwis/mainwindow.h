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


    void pbSaveRadioCzujnik_clicked();

    void pbSaveRatio_clicked();

    void pbSaveWentylator_clicked();

    void pbRoletaSave_clicked();

    void pbSaveWymiary_clicked();

    void pbcalculate_clicked();

private:
    Ui::MainWindow *ui;
    Ustawienia ust;
    QLineEdit *RolObrotTab[30];
    QLabel *sumLabel[30];
};

#endif // MAINWINDOW_H
