#ifndef MIERZONEPOZYCJE_H
#define MIERZONEPOZYCJE_H

#include <QWidget>
#include <QMutex>
#include "pozycje.h"
#include "mechanika.h"
#include "tabwidget.h"
#include "wybormetody.h"

#include <QList>

class SerialConnect;
class QTimer;
class QPlainTextEdit;

typedef struct _daneWynikowe {
    unsigned int x;
    unsigned int y;
    unsigned int time;
    float val1;
} DaneWynikowe;

namespace Ui {
class MierzonePozycje;
}

class MierzonePozycje : public TabWidget
{
    Q_OBJECT

public:
    explicit MierzonePozycje(QWidget *parent = nullptr);
    ~MierzonePozycje();

    bool chooseMethod(const WyborMetody::ModeWork & work, const WyborMetody::MethodInsData & data, const WyborMetodyData &values);
    void setList(const Pozycje &pos);

    void setValue1(const float & val, const QString & unit);

    void status(const QString & st);

    void restart();

    const WyborMetodyData &getAllValues() const;

    virtual void positionDone(bool base);
    virtual void readedFromRadio(const double &);
    virtual void errorReadFromRadio();
    virtual void setStop();

    void noweDane();

private slots:
    void update();
    //void on_pbSaveAll_clicked();
    //void on_pbSaveAll_pressed();



    void on_pbStart_clicked();
    void on_pbNoweDane_clicked();

    void on_pbZapisz_clicked();

    void on_pbRestart_clicked();

private:
    typedef enum _statusWork {
        WAIT_FOR_CONN = 0,
        WAIT,
        FIRST_RUN,
        NEXT_POS,
        WAIT_POS,
        HOME_POS,
        WAIT_HPOS,
        START_MEASURING,
        NOW_MEASURING,
        END_MEASURING,
        NEXT_POS_AFTER_HPOS,
        DONE
    } statusWorkEnum;

    typedef enum _cols {
        col_X = 0,
        col_Y,
        col_time,
        col_meas,
        col_status
    } Column;

    Ui::MierzonePozycje *ui;
    QTimer *timer;

    WyborMetody::ModeWork modeWork;
    WyborMetody::MethodInsData methodIns;
    WyborMetodyData allValues;
    Pozycje m_lista;
    QString fileName;
    bool started;




    short actWork;
    int actPos;
    Ruch mech;

    unsigned int actCzas;

    double avg1;
    unsigned int cnt1;

    unsigned int cntErr;

    QList<DaneWynikowe> m_listawynikowa;

    void setPos();
};

#endif // MIERZONEPOZYCJE_H
