#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QWidget>
#include <QDebug>
#include "mechanika.h"
#include "pozycje.h"
#include "wybormetody.h"
#include "serialmessage.h"

class MiernikPrzeplywu;

class TabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TabWidget(QWidget *parent = nullptr);

    void setMechanika(const Ruch & m);

    bool getConnect() const;
    void setConnect(bool newIsConnect);
    void setConnect(bool newIsConnect, const QString & error);

    const Ruch &getMechanika() const;


    WyborMetody::ModeWork getModeWork() const;
    void setModeWork(const WyborMetody::ModeWork &newModework);

    const QString &getFileName() const;
    void setFileName(const QString &newFileName);

    WyborMetody::MethodInsData getInsData() const;
    void setInsData(const WyborMetody::MethodInsData &newInsData);


    virtual void positionStatus(bool base, SerialMessage::StatusWork work);
    virtual void positionDone(bool base);

    virtual void readedFromRadio(const double &);
    virtual void errorReadFromRadio();

    virtual void setStatus(const QString & st);
    virtual void setStop();

    void errorSerial(const QString &err);
    void connectToDevice();
    void setPositionHome();
    void setPosition(uint32_t x, uint32_t y);
    void setRoletaHome();
    void setRoleta(uint32_t r);
    void debug(const QString &);
    void debugClear();
    void setParams(bool reverseX, bool reverseY, bool reverseR, uint32_t maxImpX, uint32_t maxImpY,
                                                                uint32_t maxStepX, uint32_t maxStepY,
                                                                uint32_t maxStepR);
    void readRadio();


    void setMiernikPrzeplywu(MiernikPrzeplywu *newMiernikPrzeplywu);

    bool getIsReady() const;
    void setIsReady(bool newIsReady);

protected:
    bool isConnect;
    Ruch mech;
    WyborMetody::ModeWork modeWork;
    WyborMetody::MethodInsData insData;
    QString fileName;

    QString addTime(const QString &status);

    MiernikPrzeplywu * miernikPrzeplywu;

    bool isReady;
};

#endif // TABWIDGET_H
