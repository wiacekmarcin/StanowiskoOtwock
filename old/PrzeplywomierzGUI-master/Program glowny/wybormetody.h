#ifndef WYBORMETODY_H
#define WYBORMETODY_H

#include <QDialog>

class QLineEdit;
class QAbstractButton;
class QCloseEvent;

namespace Ui {
class WyborMetody;
}

class WyborMetody : public QDialog
{
    Q_OBJECT
public:
    typedef enum _Method {
        METHOD_NONE = -1,
        METHOD_FILE,                    //z pliku
        METHOD_MANUAL,                  //wprowadzanie danych
        METHOD_SQUERE                   //kwardaty
    } MethodInsData;

    typedef enum _Mode {
        MODE_NONE = -1,
        MODE_2700,                  //przestrzen robocza 2700x3000
        MODE_1000P,                 //przestrzen robocza prawa 1000x2000
        MODE_1000L,                 //przestrzen robocza lewa 1000x2000
        MODE_FUNSET,                //ustawianie wiatraka
        MODE_SERVICE                //serwis
    } ModeWork;


    explicit WyborMetody(QWidget *parent = nullptr, ModeWork mode = ModeWork::MODE_2700);
    ~WyborMetody();



    ModeWork getWbMode() const;

    MethodInsData getWbMetoda() const;

    QString getFileName() const;

    unsigned int getTimeStopManual() const;

    unsigned int getTimeStopAuto() const;

    unsigned int getNumberPozMan() const;

    unsigned int getNumberWidth() const;

    unsigned int getNumberHeight() const;

private slots:

    void on_rbfile_toggled(bool checked);

    void on_rbmanual_toggled(bool checked);

    void on_rbhalfmanual_toggled(bool checked);

    void on_pushButton_clicked();

    void on_pushButton_pressed();

    void on_rb2700_toggled(bool checked);

    void on_rb1000l_toggled(bool checked);

    void on_rb1000p_toggled(bool checked);

    void on_rbFunSet_toggled(bool checked);

    void on_timeManualDefault_editingFinished();

    void on_buttonBox_rejected();

    void on_timeAuto_editingFinished();

    void on_numberAuto_editingFinished();

    void on_numberAuto_2_editingFinished();

    void on_timeAuto_textChanged(const QString &arg1);

    void on_numberAuto_textChanged(const QString &arg1);

    void on_numberAuto_2_textChanged(const QString &arg1);

    void on_timeManualDefault_textChanged(const QString &arg1);

    void on_numberManual_textChanged(const QString &arg1);

protected:
    void chooseFileName();
    void setEnabledContinue(bool enabled);
    bool isValidNumber(QLineEdit * number, int max);
    bool isValidTime(QLineEdit * time);

    bool isValidAutoParameters();
    bool isValidManualParameters();

    void reject();
private:
    Ui::WyborMetody *ui;
    MethodInsData  wbMetoda;
    ModeWork wbMode;
    unsigned int timeStopManual;
    unsigned int timeStopAuto;
    unsigned int numberPozMan;
    unsigned int numberWidth;
    unsigned int numberHeight;

   // static constexpr int maxPosition = 1000;
    static constexpr int maxTime = 3600;
    static constexpr int minTime = 1;

    QString fileName;

    bool startWindow;
};

#endif // WYBORMETODY_H
