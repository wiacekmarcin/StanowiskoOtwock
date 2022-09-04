#ifndef MIERZONEPOZYCJE_H
#define MIERZONEPOZYCJE_H

#include <QWidget>
#include <QMutex>
#include <QElapsedTimer>
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

    void setList(const Pozycje &pos);

    void setValue1(const float & val, const QString & unit);


    void restart();

    const WyborMetodyData &getAllValues() const;

    virtual void positionDone(bool base);
    virtual void readedFromRadio(const double &);
    virtual void errorReadFromRadio();
    virtual void setStop();
    virtual void setStart();
    virtual void setError();
    virtual void setStatus(const QString & st);

    void noweDane();

private slots:
    void updateWork();
    //void on_pbSaveAll_clicked();
    //void on_pbSaveAll_pressed();



    void on_pbStart_clicked();
    void on_pbNoweDane_clicked();

    void on_pbZapisz_clicked();

    void on_pbRestart_clicked();

private:
    typedef enum _statusWork {
        WAIT_FOR_READY = 0,
        WAIT,
        READY_FOR_WORK,
        FIRST_RUN,
        NEXT_POS,
        WAIT_POS,
        HOME_POS,
        WAIT_HPOS,
        START_MEASURING,
        NOW_MEASURING,
        END_MEASURING,
        NEXT_POS_AFTER_HPOS,
        DONE,
        END_WORK
    } statusWorkEnum;

    typedef enum _cols {
        col_X = 0,
        col_Y,
        col_time,
        col_meas,
        col_status
    } Column;

    Ui::MierzonePozycje *ui;
    QTimer* timer;
    WyborMetody::ModeWork modeWork;
    WyborMetody::MethodInsData methodIns;
    WyborMetodyData allValues;
    Pozycje m_lista;
    QString fileName;
    bool started;
    QElapsedTimer radioTimer;
    //QTimer



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
