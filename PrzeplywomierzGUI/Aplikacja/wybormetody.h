#ifndef WYBORMETODY_H
#define WYBORMETODY_H

#include "ustawienia.h"

#include <QDialog>

class QLineEdit;
class QAbstractButton;
class QCloseEvent;

namespace Ui {
class WyborMetody;
}

struct WyborMetodyData {
    unsigned int timeStopManual;
    unsigned int timeStopAuto;
    unsigned int numberPozMan;
    unsigned int numberWidth;
    unsigned int numberHeight;
    unsigned int etapNrRoleta;
    unsigned int stableTimeRoleta;
    unsigned int timeStopRoleta;
    QString fileName;
    QString fileName2;
    unsigned int offsetX;
    unsigned int offsetY;
};


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
        MODE_SERVICE,               //serwis
        MODE_ROLETA,                //roleta
    } ModeWork;


    explicit WyborMetody(QWidget *parent, ModeWork mode,
                         MethodInsData method,
                         Ustawienia & u);
    ~WyborMetody();


    ModeWork getWbMode() const;

    MethodInsData getWbMetoda() const;

    QString getFileName() const;

    unsigned int getTimeStopManual() const;

    unsigned int getTimeStopAuto() const;

    unsigned int getNumberPozMan() const;

    unsigned int getNumberWidth() const;

    unsigned int getNumberHeight() const;

    unsigned int getPartRolet() const;

    unsigned int getStableTimeCzas() const;

    QString getFileName2() const;

    unsigned int getTimeStopRoleta() const;

    const WyborMetodyData &getData() const;

    void setWbMetoda(MethodInsData newWbMetoda);

    void setWbMode(ModeWork newWbMode);

    void setData(const WyborMetodyData &newData);

    void setUstawienia(Ustawienia *u);
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

    void on_timeAuto_editingFinished();

    void on_numberAuto_editingFinished();

    void on_numberAuto_2_editingFinished();

    void on_timeAuto_textChanged(const QString &arg1);

    void on_numberAuto_textChanged(const QString &arg1);

    void on_numberAuto_2_textChanged(const QString &arg1);

    void on_timeManualDefault_textChanged(const QString &arg1);

    void on_numberManual_textChanged(const QString &arg1);

    void on_rbRoleta_toggled(bool checked);

    void on_rbRoletaDane_toggled(bool checked);

    void on_rbRoletaPlik_toggled(bool checked);

    void on_pbChooseFile_clicked();

    void on_pbNormaOffsetChange_clicked();

    void on_pbNormaOffsetSave_clicked();

    void on_normaEtapNumber_textChanged(const QString &arg1);

    void on_normaStabTime_textChanged(const QString &arg1);

    void on_NormaIloscProbek_textChanged(const QString &arg1);

    void on_rbRoletaDane_clicked();

    void on_normaEtapNumberFile_textChanged(const QString &arg1);

    void on_normaStabTimeFile_textChanged(const QString &arg1);

protected:
    void chooseFileName();
    void setEnabledContinue(bool enabled);
    bool isValidNumber(QLineEdit * number, int max);
    bool isValidTime(QLineEdit * time);

    bool isValidAutoParameters();
    bool isValidManualParameters();

    void reject();
    bool isValidNumberRolety(QLineEdit *number);
    bool isValidPostojRolety(QLineEdit *number);
    bool isValidIloscRolety(QLineEdit *number);

    void visibleRoleta(bool visible);
    void visibleOther(bool visible);

    bool isValidRoletaRB();
    bool isValidPlaszczynaRB();
    void init();
    void initMethodPosition();
private:
    Ui::WyborMetody *ui;
    MethodInsData  wbInsData;
    ModeWork wbMode;
    WyborMetodyData data;

   // static constexpr int maxPosition = 1000;
    static constexpr int maxTime = 3600;
    static constexpr int minTime = 1;
    static constexpr int maxRolet = 15;
    static constexpr int minRolet = 2;
    static constexpr int maxPostoj = 3600;


    bool startWindow;

    Ustawienia & ust;
};
#endif // WYBORMETODY_H
