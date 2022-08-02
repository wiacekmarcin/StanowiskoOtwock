#ifndef POZYCJEROLETA_H
#define POZYCJEROLETA_H

#include <QWidget>
class QTimer;

namespace Ui {
class PozycjeRoleta;
}

class PozycjeRoleta : public QWidget
{
    Q_OBJECT

public:
    explicit PozycjeRoleta(QWidget *parent = nullptr);
    ~PozycjeRoleta();

    void setData(unsigned short etapNr, unsigned int stableTime, unsigned int cnt);

    unsigned int offsetX() const;
    void setOffsetX(unsigned int newOffsetX);

    unsigned int offsetY() const;
    void setOffsetY(unsigned int newOffsetY);

    void setValue1(const float & val, const QString & unit);

    bool getConnected() const;
    void setConnected(bool newConnected);

    void status(const QString & st);

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

    void readedFromRadio(int val);

    void on_pbStart_clicked();
    void on_pbNoweDane_clicked();

    void on_pbZapisz_clicked();

protected:
    void debug(const QString &val);

    unsigned int setNorma5(unsigned row, unsigned int wysokosc, unsigned int pomiary, unsigned int nrR, unsigned int rSize);
    unsigned int setNorma6(unsigned row, unsigned int wysokosc, unsigned int pomiary, unsigned int nrR, unsigned int rSize);
    unsigned int setNorma7(unsigned row, unsigned int wysokosc, unsigned int pomiary, unsigned int nrR, unsigned int rSize);
    unsigned int setNorma8(unsigned row, unsigned int wysokosc, unsigned int pomiary, unsigned int nrR, unsigned int rSize);
    unsigned int setNorma9(unsigned row, unsigned int wysokosc, unsigned int pomiary, unsigned int nrR, unsigned int rSize);
    unsigned int setNorma10(unsigned row, unsigned int wysokosc, unsigned int pomiary, unsigned int nrR, unsigned int rSize);
    void createRow(int row, const QString & c1, const QString & c2, const QString & c3, const QString & c4,
                   const QString & c5, const QString & c6, const QString & c7, const QString & c8,
                   const QString & c9, const QString & c10, const QString & c11);

    unsigned int createRoletaRow(unsigned int row, unsigned int nrR, unsigned int rSize, unsigned int stableTime);
    unsigned int createPoint(unsigned int row, unsigned int mmX, unsigned int mmY,
                             float nX, float nY, unsigned int numer, unsigned int pomiar,
                             unsigned int nrR, unsigned int rSize);
private:
    typedef enum _statusWork {
        WAIT = 0,
        MOVEROLETA,
        WAIT2STABLE,
        POSITIONING,
        MEASURING,
        NEXTPOSITION,
        NEXTROLETA,
    } statusWorkEnum;

private:
    Ui::PozycjeRoleta *ui;
    QTimer *timer;

    short actStatus;
    int actPos;
    bool connected;

    unsigned int m_width;
    unsigned int m_height;

    unsigned int m_offsetX;
    unsigned int m_offsetY;

    double avg1;
    unsigned int cnt1;
};

#endif // POZYCJEROLETA_H
