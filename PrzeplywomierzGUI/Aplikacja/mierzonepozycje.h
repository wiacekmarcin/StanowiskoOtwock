#ifndef MIERZONEPOZYCJE_H
#define MIERZONEPOZYCJE_H

#include <QWidget>
#include <QMutex>
#include "pozycje.h"
#include "mechanika.h"

#include <QList>

class SerialConnect;
class QTimer;
class QPlainTextEdit;

typedef struct DaneWynikowe_1 {
    unsigned int x;
    unsigned int y;
    unsigned int time;
    float val1;
    float val2;
    float val3;
    float val4;
} DaneWynikowe1;

typedef struct DaneWynikowe_2 {
    unsigned int x;
    unsigned int y;
    float val1;
    float val2;
    float val3;
    float val4;
} DaneWynikowe2;


namespace Ui {
class MierzonePozycje;
}

class MierzonePozycje : public QWidget
{
    Q_OBJECT

public:
    explicit MierzonePozycje(QWidget *parent = nullptr);
    ~MierzonePozycje();

    void setList(const Pozycje & pos);

    void setMechanika(const Ruch & m);

    bool getIsWait();
    void setIsWait(bool value);
    void setIsStart(bool value);
    bool getIsStart();

    void setDebug(const QString &val);

    void setValues(const float & val1);
    void setValue1(const float & val, const QString & unit);

    bool getConnected() const;
    void setConnected(bool newConnected);

    void status(const QString & st);

    void restart();

signals:
    void doConnect();
    void setPosition(uint32_t, uint32_t);
    void readRadio();
    void checkDevice();
    void statusMiernik(QString);
    void end();
    void start();
    void noweDane();

private slots:
    void update();
    //void on_pbSaveAll_clicked();
    //void on_pbSaveAll_pressed();

    void readedFromRadio(int val);

    void on_pbStart_clicked();
    void on_pbNoweDane_clicked();

    void on_pbZapisz_clicked();

protected:
    void debug(const QString &val);
private:
    typedef enum _statusWork {
        WAIT = 0,
        POSITIONING,
        MEASURING,
        NEXTPOSITION
    } statusWorkEnum;


    Ui::MierzonePozycje *ui;
    QTimer *timer;

    bool isStart;
    bool isWait;
    QMutex m_mutex;
    QMutex m_mutex2;
    short actStatus;
    int actPos;
    Ruch mech;
    Pozycje m_lista;
    unsigned int actCzas;

    double avg1;
    unsigned int cnt1;


    QList<DaneWynikowe1> m_listawynikowa1;
    QList<DaneWynikowe2> m_listawynikowa2;

    bool connected;
};

#endif // MIERZONEPOZYCJE_H
