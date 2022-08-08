#include "tabwidget.h"
#include "qdatetime.h"

#include "miernikprzeplywu.h"

TabWidget::TabWidget(QWidget *parent)
    : QWidget{parent}
{
    isConnect = false;
}

void TabWidget::setMechanika(const Ruch &m)
{
    mech = m;
}

bool TabWidget::getConnect() const
{
    return isConnect;
}

void TabWidget::setConnect(bool newIsConnect)
{
    isConnect = newIsConnect;
}

void TabWidget::setConnect(bool newIsConnect, const QString &error)
{
    isConnect = newIsConnect;
    if (!isConnect) {
        isReady = false;
        setStop();
    }
    debug(addTime(error));
}

const Ruch &TabWidget::getMechanika() const
{
    return mech;
}

WyborMetody::ModeWork TabWidget::getModeWork() const
{
    return modeWork;
}

void TabWidget::setModeWork(const WyborMetody::ModeWork & newModework)
{
    modeWork = newModework;
}

const QString &TabWidget::getFileName() const
{
    return fileName;
}

void TabWidget::setFileName(const QString &newFileName)
{
    fileName = newFileName;
}

WyborMetody::MethodInsData TabWidget::getInsData() const
{
    return insData;
}

void TabWidget::setInsData(const WyborMetody::MethodInsData & newInsData)
{
    insData = newInsData;
}

void TabWidget::errorSerial(const QString &err)
{
    setConnect(false, err);
}

void TabWidget::readedFromRadio(const double &)
{

}

void TabWidget::errorReadFromRadio()
{

}

void TabWidget::positionStatus(bool , SerialMessage::StatusWork )
{

}

void TabWidget::positionDone(bool )
{

}

void TabWidget::roletaDone(bool base)
{

}

void TabWidget::setStatus(const QString &)
{

}

void TabWidget::setStop()
{

}

void TabWidget::connectToDevice()
{
    if (miernikPrzeplywu)
        miernikPrzeplywu->connectToDevice();
}

void TabWidget::setPositionHome()
{
    if (miernikPrzeplywu)
        miernikPrzeplywu->setPositionHome();
}

void TabWidget::setPosition(uint32_t x, uint32_t y)
{
    if (miernikPrzeplywu)
        miernikPrzeplywu->setPosition(x, y);
}

void TabWidget::setRoletaHome()
{
    if (miernikPrzeplywu)
        miernikPrzeplywu->setRoletaHome();
}

void TabWidget::setRoleta(uint32_t r)
{
    if (miernikPrzeplywu)
        miernikPrzeplywu->setRoleta(r);
}

void TabWidget::debug(const QString &s)
{
    if (miernikPrzeplywu)
        miernikPrzeplywu->debug(addTime(s));
}

void TabWidget::debugClear()
{
    if (miernikPrzeplywu)
        miernikPrzeplywu->debugClear();
}

void TabWidget::setParams(bool reverseX, bool reverseY, bool reverseR, uint32_t maxImpX, uint32_t maxImpY, uint32_t maxStepX, uint32_t maxStepY, uint32_t maxStepR)
{
    if (miernikPrzeplywu)
        miernikPrzeplywu->setParams(reverseX, reverseY, reverseR, maxImpX, maxImpY, maxStepX, maxStepY, maxStepR);
}

void TabWidget::readRadio()
{
    if (miernikPrzeplywu)
        miernikPrzeplywu->readRadio();
}

QString TabWidget::addTime(const QString& status)
{
    return QString("[%1] %2").arg(QTime::currentTime().toString("HH:mm:ss.zzz")).arg(status);
}

bool TabWidget::getIsReady() const
{
    return isReady;
}

void TabWidget::setIsReady(bool newIsReady)
{
    isReady = newIsReady;
}

void TabWidget::setMiernikPrzeplywu(MiernikPrzeplywu *newMiernikPrzeplywu)
{
    miernikPrzeplywu = newMiernikPrzeplywu;
}
