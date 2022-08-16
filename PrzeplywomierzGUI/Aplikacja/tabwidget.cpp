#include "tabwidget.h"
#include "qdatetime.h"

#include "miernikprzeplywu.h"

#include <QMutexLocker>

TabWidget::TabWidget(QWidget *parent)
    : QWidget{parent}
{
    isReady = false;
    mutex = new QMutex();
}

void TabWidget::setMechanika(const Ruch &m)
{
    mech = m;
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

void TabWidget::positionStatus(bool, SerialMessage::StatusWork)
{

}

void TabWidget::positionDone(bool)
{

}

void TabWidget::roletaDone(bool)
{

}

void TabWidget::readedFromRadio(const double &)
{

}

void TabWidget::errorReadFromRadio()
{

}

void TabWidget::setStatus(const QString &)
{

}

void TabWidget::setStop()
{

}

void TabWidget::setStart()
{

}


void TabWidget::setError()
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
        miernikPrzeplywu->debug(s);
}

void TabWidget::debugClear()
{
    if (miernikPrzeplywu)
        miernikPrzeplywu->debugClear();
}

void TabWidget::readRadio()
{
    if (miernikPrzeplywu)
        miernikPrzeplywu->readRadio();
}

void TabWidget::setClose(bool afterBase)
{
    if (miernikPrzeplywu)
        miernikPrzeplywu->setClose(afterBase);
}

QString TabWidget::addTime(const QString& status)
{
    return QString("[%1] %2").arg(QTime::currentTime().toString("HH:mm:ss.zzz")).arg(status);
}

bool TabWidget::getIsReady() const
{
    QMutexLocker locker(mutex);
    return isReady;
}

void TabWidget::setIsReady(bool newIsReady)
{
    QMutexLocker locker(mutex);
    isReady = newIsReady;
}

void TabWidget::setMiernikPrzeplywu(MiernikPrzeplywu *newMiernikPrzeplywu)
{
    miernikPrzeplywu = newMiernikPrzeplywu;
}
