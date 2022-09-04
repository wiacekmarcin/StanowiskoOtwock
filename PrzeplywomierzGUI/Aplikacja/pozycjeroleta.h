#ifndef POZYCJEROLETA_H
#define POZYCJEROLETA_H

#include <QWidget>
#include "tabwidget.h"
#include <QElapsedTimer>

class QTimer;

namespace Ui {
class PozycjeRoleta;
}

typedef struct _daneWynikoweR {
    QString nrEtap;
    QString x;
    QString y;
    QString nx;
    QString ny;
    unsigned int time;
    float val1;
} DaneWynikoweRoleta;

typedef struct _point {
    bool roleta;
    unsigned int stableRolTime;
    unsigned int rmm;
    unsigned int measurTime;
    unsigned int xmm;
    float x;
    unsigned int ymm;
    float y;
    unsigned int nrEtap; 
} PunktyPomiariowe;

class PozycjeRoleta : public TabWidget
{
    Q_OBJECT

public:
    explicit PozycjeRoleta(QWidget *parent = nullptr);
    ~PozycjeRoleta();

    void setList(const PozycjeRol & l);
    void setUstawienia(Ustawienia& ust);

    typedef enum _cols {
        col_xmm = 0,
        col_ymm,
        col_xnorma,
        col_ynorma,
        col_etap,
        col_rmm,
        col_stableTime,
        col_measTime,
        col_measValue,
        col_action,
        col_status,
    } Column;

    virtual void positionDone(bool base);
    virtual void roletaDone(bool base);
    virtual void readedFromRadio(const double &);
    virtual void errorReadFromRadio();
    virtual void setStop();
    virtual void setStart();
    virtual void setError();
    virtual void setStatus(const QString & st);

protected :
    void setPos();
    void setValue1(const float & val, const QString & unit);

private slots:
    void updateWork();

    void on_pbStart_clicked();
    void on_pbNoweDane_clicked();
    void on_pbZapisz_clicked();
    void on_pbRestart_clicked();
    void on_pbZamknijRoleta_clicked();

protected:

    void createRow(int row, const QString & c1, const QString & c2, const QString & c3, const QString & c4,
                   const QString & c5, const QString & c6, const QString & c7, const QString & c8,
                   const QString & c9, const QString & c10);

    unsigned int createRoletaRow(unsigned int row, unsigned int nrR, unsigned int rSize, unsigned int stableTime);
    unsigned int createPoint(unsigned int row, unsigned int mmX, unsigned int mmY,
                             float nX, float nY, unsigned int pomiar,
                             unsigned int nrR, unsigned int rSize);
private:
    typedef enum _statusWork {
        WAIT_FOR_READY = 0,
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
        DONE,
        MOVEROLETA,
        WAIT2STABLE,
        ROLETA,
        ROLETAWAIT,
        ROLETA_HOME,
        ROLETA_HOMEWAIT,
        END
    } statusWorkEnum;

private:
    Ui::PozycjeRoleta *ui;
    QTimer *timer;

    short actWork;
    int actPos;
    unsigned int measTime;

    double avg1;
    unsigned int cnt1;
    unsigned int cntErr;
    unsigned int stableTime;

    unsigned int xmm;
    unsigned int ymm;

    QList<DaneWynikoweRoleta> m_listawynikowa;
    bool roletaClose;
    bool firstRoleta;

    QVector<PunktyPomiariowe> punkty;
    QElapsedTimer radioTimer;
};

#endif // POZYCJEROLETA_H
