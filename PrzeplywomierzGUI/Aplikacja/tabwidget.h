#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QWidget>
#include <QDebug>
#include "mechanika.h"
#include "pozycje.h"
#include "wybormetody.h"
#include "serialmessage.h"
#include <QMutex>


class MiernikPrzeplywu;

class TabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TabWidget(QWidget *parent = nullptr);

    void setMechanika(const Ruch & m);
    const Ruch &getMechanika() const;

    void setMechanikaRolety(const RoletaRuch & m);
    const RoletaRuch &getMechanikaRolety() const;

    WyborMetody::ModeWork getModeWork() const;
    void setModeWork(const WyborMetody::ModeWork &newModework);

    const QString &getFileName() const;
    void setFileName(const QString &newFileName);

    WyborMetody::MethodInsData getInsData() const;
    void setInsData(const WyborMetody::MethodInsData &newInsData);


    virtual void positionStatus(bool base, SerialMessage::StatusWork work);
    virtual void positionDone(bool base);
    virtual void roletaDone(bool base);

    virtual void readedFromRadio(const double &);
    virtual void errorReadFromRadio();

    virtual void setStatus(const QString & st);
    virtual void setStop();
    virtual void setStart();
    virtual void setError();

    void connectToDevice();
    void setPositionHome();
    void setPosition(uint32_t x, uint32_t y);
    void setRoletaHome();
    void setRoleta(uint32_t r);
    void debug(const QString &);
    void debugClear();
    void readRadio();
    void setClose(bool afterBase);
    void closeRoleta();

    void setMiernikPrzeplywu(MiernikPrzeplywu *newMiernikPrzeplywu);

    bool getIsReady() const;
    void setIsReady(bool newIsReady);

    void setRadioVal(const QString val);

protected:
    Ruch mech;
    RoletaRuch mechR;

    WyborMetody::ModeWork modeWork;
    WyborMetody::MethodInsData insData;
    QString fileName;

    QString addTime(const QString &status);

    MiernikPrzeplywu * miernikPrzeplywu;

    bool isReady;
    QMutex * mutex;
};

#endif // TABWIDGET_H
