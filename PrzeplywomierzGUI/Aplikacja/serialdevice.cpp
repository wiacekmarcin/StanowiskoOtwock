#include "serialdevice.h"
#include <QTime>
#include <QThread>
#include <QSerialPortInfo>
#include "rs232.h"
#include <QDebug>
#include <QMutexLocker>
#include <QElapsedTimer>

#define DEBUGSER(X) debugFun(QString("%1:%2 %3").arg(__FILE__).arg(__LINE__).arg(X))

const char* const SerialWorker::mapTask[] = { "IDLE","CONNECT","SET_PARAMS","SET_POSITION","SET_HOME","SET_ROLETA","SET_ROLETA_HOME","READ_RADIO","DISCONNECT","RESET","QUIET_ON","QUIET_OFF" };

SerialWorker::SerialWorker(SerialDevice * device):
    QThread(nullptr),
    sd(device)
{
    actTask = IDLE;
    runWorker = true;

    connect(this, &SerialWorker::debug, device, &SerialDevice::debug, Qt::QueuedConnection);
}

SerialWorker::~SerialWorker()
{
    mutex.lock();
    runWorker = false;
    actTask = IDLE;
    newTask.wakeOne();
    mutex.unlock();
    wait(120000);
}

bool SerialWorker::command(Task curr)
{
    DEBUGSER(QString("New command %1").arg(mapTask[curr]));
    {
        const QMutexLocker locker(&mutexRun);
        if (!runWorker)
            return false;
    }
    const QMutexLocker locker(&mutex);

    futureTask.push_back(curr);
    newTask.wakeOne();
    return true;
}

void SerialWorker::setStop()
{
    {
        const QMutexLocker locker(&mutexRun);
        runWorker = false;    
    }

    //mutex.lock();
    const QMutexLocker locker(&mutex);
    futureTask.clear();
    futureTask.push_back(DISCONNECT);
    newTask.wakeOne();
    //mutex.unlock();
    //wait();
}

void SerialWorker::setReset()
{
    const QMutexLocker locker(&mutex);
    futureTask.clear();
}

void SerialWorker::run()
{
    mutex.lock();
    short zadanie = actTask;
    mutex.unlock();
    bool quit = false;
    DEBUGSER(QString("actTask = %1").arg(mapTask[actTask]));
    do {
        mutex.lock();
        if (futureTask.size() == 0) {
            actTask = IDLE;
            newTask.wait(&mutex);
        } else {
            actTask = futureTask.front();
            futureTask.pop_front();
        }
        zadanie = actTask;
        mutex.unlock();
        DEBUGSER(QString("actTask = %1").arg(mapTask[zadanie]));
        switch(zadanie) {
        case IDLE:
            break;
            
        case CONNECT:
            sd->connectToSerialJob();
            break;

        case SET_PARAMS:
            sd->setParamsJob();
              break;

        case SET_HOME:
            sd->setHomeJob();
            break;

        case SET_POSITION:
            sd->setPosJob();
            break;
        
        case SET_ROLETA:
            sd->setRoletaJob();
            break;

        case SET_ROLETA_HOME:
            sd->setRoletaHomeJob();
            break;

        case READ_RADIO:
            sd->readRadioJob();
            break;

        case DISCONNECT:
            sd->closeDeviceJob();
            break;

        case RESET:
            sd->resetJob();
            break;
        
        case QUIET_ON:
            sd->setQuiteMode(true);
            break;
        
        case QUIET_OFF:
            sd->setQuiteMode(false);
            break;

        default:
            break;
        }
        mutexRun.lock();
        quit = !runWorker;
        mutexRun.unlock();
    } while (!quit);
    
   DEBUGSER("SerialWorker::run() quit");
   this->quit();
}

void SerialWorker::debugFun(const QString &s)
{
    emit debug(s);
}

SerialWorker::Task SerialWorker::getActTask()
{
    Task ret;
    mutex.lock();
    ret = actTask;
    mutex.unlock();
    return ret;
}
/**************************************************************************/

SerialDevice::SerialDevice(QObject *parent)
    : QObject(parent),
      m_portName(""), m_portNr(-1),
      m_connected(false),  m_worker(this),
      emitSignal(true)

{
    m_reverseX = false;
    m_reverseY = false;
    m_reverseR = false;

    m_maxImpX = 0;
    m_maxImpY = 0;

    m_maxStepX = 0;
    m_maxStepY = 0;
    m_maxStepR = 0;

    m_portNr = -1;
#ifdef SERIALLINUX
     m_serialPort = new QSerialPort(&m_worker);
#endif

}

SerialDevice::~SerialDevice()
{
    setStop();
}

void SerialDevice::setThread(QThread *trh)
{
    m_worker.moveToThread(trh);
    m_worker.start();
    m_worker.command(SerialWorker::IDLE);
}

void SerialDevice::setStop()
{
    m_worker.setReset();
    m_worker.setStop();
}

void SerialDevice::connectToDevice()
{
    if (connected()) {
        emit kontrolerConfigured(true, ALL_OK);
    } else {
        m_worker.command(SerialWorker::CONNECT);
    }
}

void SerialDevice::setParams(bool reverseX, bool reverseY, bool reverseR,
               uint32_t maxImpX, uint32_t maxImpY,
               uint32_t maxStepX, uint32_t maxStepY,
               uint32_t maxStepR, uint16_t minStepR,
               uint16_t speedRolHome, uint16_t speedRolPos)
{
    m_reverseX = reverseX;
    m_reverseY = reverseY;
    m_reverseR = reverseR;

    m_maxImpX = maxImpX;
    m_maxImpY = maxImpY;

    m_maxStepX = maxStepX;
    m_maxStepY = maxStepY;
    m_maxStepR = maxStepR;

    m_minStepR = minStepR;
    m_speedRolHome = speedRolHome;
    m_speedRolPos = speedRolPos;

    if (connected()) {
        insertParams();
    }
}

void SerialDevice::setPositionHome()
{
    if (!connected())
        m_worker.command(SerialWorker::CONNECT);
    m_worker.command(SerialWorker::SET_HOME);
}

void SerialDevice::setPosition(uint32_t x, uint32_t y)
{
    m_impX = x;
    m_impY = y;
    if (!connected()) {
        m_worker.command(SerialWorker::CONNECT);
        m_worker.command(SerialWorker::SET_HOME);
    }
    m_worker.command(SerialWorker::SET_POSITION);
}

void SerialDevice::setRoletaHome()
{
    if (!connected())
        m_worker.command(SerialWorker::CONNECT);
    m_worker.command(SerialWorker::SET_ROLETA_HOME);
}

void SerialDevice::setRoleta(uint32_t r)
{
    m_stepR = r;
    if (!connected()) {
        m_worker.command(SerialWorker::CONNECT);
        m_worker.command(SerialWorker::SET_ROLETA_HOME);
    }
    m_worker.command(SerialWorker::SET_ROLETA);
}

void SerialDevice::readRadio()
{
    if (!connected())
        m_worker.command(SerialWorker::CONNECT);
    m_worker.command(SerialWorker::READ_RADIO);
}


void SerialDevice::insertParams()
{
    if (!connected())
        m_worker.command(SerialWorker::CONNECT);
    m_worker.command(SerialWorker::SET_PARAMS);
}



bool SerialDevice::configureDevice()
{
    DEBUGSER(QString("Konfiguracja %1%2%3 imp=[%4,%5] steps=[%6,%7] R=%8 Rdown=%9").arg(m_reverseX).arg(m_reverseY).arg(m_reverseR)
    .arg(m_maxImpX).arg(m_maxImpY).arg(m_maxStepX).arg(m_maxStepY).arg(m_maxStepR).arg(m_minStepR) + 
        QString(" Speed [down %1, up %2]").arg(m_speedRolHome).arg(m_speedRolPos));
    QElapsedTimer timer;
    auto s = write(SerialMessage::welcomeMsg(), 200, 10000).getParseReply();

    if (s != SerialMessage::WELCOME_REPLY) {
        emit kontrolerConfigured(false, IDENT_FAILD);
        return false;
    }

    emit kontrolerConfigured(false, IDENT_OK);

    //s = write(SerialMessage::setReset(), 100, 3000).getParseReply();
    //if (s != SerialMessage::RESET_REPLY)
    //    return false;

    //QThread::currentThread()->sleep(2);

    s = write(SerialMessage::settings1Msg(m_reverseX, m_reverseY, m_reverseR, m_maxImpX, m_maxImpY, m_speedRolHome, m_speedRolPos),
              200, 200).getParseReply();

    if (s != SerialMessage::SETPARAMS1_REPLY) {
        emit kontrolerConfigured(false, PARAMS_FAILD);
        return false;
    }

    s = write(SerialMessage::settings2Msg(m_maxStepX, m_maxStepY, m_maxStepR, m_minStepR),
              200, 200).getParseReply();

    if (s != SerialMessage::SETPARAMS2_REPLY) {
        emit kontrolerConfigured(false, PARAMS_FAILD);
        return false;
    }

    emit kontrolerConfigured(true, PARAMS_OK);
    return true;
}

void SerialDevice::setParamsJob()
{
    DEBUGSER(QString("Konfiguracja %1%2%3 imp=[%4,%5] steps=[%6,%7] R=%8 Rdown=%9").arg(m_reverseX).arg(m_reverseY).arg(m_reverseR)
        .arg(m_maxImpX).arg(m_maxImpY).arg(m_maxStepX).arg(m_maxStepY).arg(m_maxStepR).arg(m_minStepR) +
        QString(" Speed [down %1, up %2]").arg(m_speedRolHome).arg(m_speedRolPos));

    auto s = write(SerialMessage::settings1Msg(m_reverseX, m_reverseY, m_reverseR, m_maxImpX, m_maxImpY, m_speedRolHome, m_speedRolPos),
              200, 1000).getParseReply();

    if (s != SerialMessage::SETPARAMS1_REPLY) {
        if (emitSignal) emit setParamsDone(false);
        return;
    }

    s = write(SerialMessage::settings2Msg(m_maxStepX, m_maxStepY, m_maxStepR, m_minStepR),
              200, 1000).getParseReply();

    if (s != SerialMessage::SETPARAMS2_REPLY) {
        if (emitSignal) emit setParamsDone(false);
        return;
    }

    if (emitSignal) emit setParamsDone(true);
}


void SerialDevice::setPosJobLocal(bool home)
{
    SerialMessage::ParseReply orp = home ? SerialMessage::MOVEHOME_REPLY : SerialMessage::POSITION_REPLY;

    auto s = write((home ? SerialMessage::setPositionHome() :
                    SerialMessage::setPosition(m_impX, m_impY)),
            100, 1000);
    auto rp = s.getParseReply();

    auto actSt = s.getPosWork();
    if (rp != orp) {
        if (emitSignal)  emit setPositionDone(false, home, actSt);
        return;
    } else {
        if (actSt != SerialMessage::START_XY) {
            if (emitSignal) emit setPositionDone(false, home, actSt);
            return;
        }   
        if (emitSignal) emit setPositionDone(true, home, actSt);
    }

    s = write(QByteArray(), 0, 1000);
    rp = s.getParseReply();
    actSt = s.getPosWork();
    if (rp != orp) {
        if (emitSignal) emit setPositionDone(false, home, actSt);
        return;
    } else {
        if (actSt != SerialMessage::START_X) {
            if (emitSignal) emit setPositionDone(false, home, actSt);
            return;
        }   
        if (emitSignal) emit setPositionDone(true, home, actSt);
    }
    
    s = write(QByteArray(), 0, 200000);
    rp = s.getParseReply();
    actSt = s.getPosWork();
    if (rp != orp) {
        if (emitSignal) emit setPositionDone(false, home, actSt);
        return;
    } else {
        if (actSt == SerialMessage::ERROR_XY) {
            if (emitSignal) emit setPositionDone(false, home, actSt);
            return;
        }
        if (actSt != SerialMessage::END_X) {
            if (emitSignal) emit setPositionDone(false, home, actSt);
            return;
        }
        values = s.getValues();
        if (emitSignal) emit setPositionDone(true, home, actSt);
    }

    s = write(QByteArray(), 0, 1000);
    rp = s.getParseReply();
    actSt = s.getPosWork();
    if (rp != orp) {
        if (emitSignal) emit setPositionDone(home, false, actSt);
        return;
    } else {
        if (actSt != SerialMessage::START_Y) {
            if (emitSignal) emit setPositionDone(false, home, actSt);
            return;
        }   
        if (emitSignal) emit setPositionDone(true, home, actSt);
    }

    s = write(QByteArray(), 0, 200000);
    rp = s.getParseReply();
    actSt = s.getPosWork();
    if (rp != orp) {
        if (emitSignal) emit setPositionDone(false, home, actSt);
        return;
    } else {
        if (actSt == SerialMessage::ERROR_XY) {
            if (emitSignal) emit setPositionDone(false, home, actSt);
            return;
        }
        if (actSt != SerialMessage::END_Y) {
            if (emitSignal) emit setPositionDone(false, home, actSt);
            return;
        }   
        values = s.getValues();
        if (emitSignal) emit setPositionDone(true, home, actSt);
    }

    s = write(QByteArray(), 0, 1000);
    rp = s.getParseReply();
    actSt = s.getPosWork();
    if (rp != orp) {
        if (emitSignal) emit setPositionDone(false, home, actSt);
        return;
    } else {
        if (actSt != SerialMessage::END_XY) {
            if (emitSignal) emit setPositionDone(false, home, actSt);
            return;
        }   
        if (emitSignal) emit setPositionDone(true, home, actSt);
    }
}

void SerialDevice::setHomeJob()
{
    DEBUGSER(QString("Ustaw pozycje startowa dla czujnika"));
    setPosJobLocal(true);
}

void SerialDevice::setPosJob()
{
    DEBUGSER(QString("Ustaw pozycje x=%1 y=%2").arg(m_impX).arg(m_impY));
    setPosJobLocal(false);
}

void SerialDevice::setRoletaJobLocal(bool home)
{
    SerialMessage::ParseReply orp = home ? SerialMessage::MOVEHOME_REPLY : SerialMessage::POSITION_REPLY;

    auto s = write((home ? SerialMessage::setRoletaHome() :
                    SerialMessage::setRoleta(m_stepR)),
            500, 1000);
    auto rp = s.getParseReply();
    auto actSt = s.getPosWork();
    if (rp != orp) {
        emit setPositionDone(false, home, actSt);
        return;
    } else {
        if (actSt != SerialMessage::START_R) {
            emit setPositionDone(false, home, actSt);
            return;
        }   
        if (emitSignal) emit setPositionDone(true, home, actSt);
    }

    s = write(QByteArray(), 0, 10000000);
    rp = s.getParseReply();
    actSt = s.getPosWork();
    if (rp != orp) {
        emit setPositionDone(false, home, actSt);
        return;
    } else {
        if (actSt == SerialMessage::ERROR_R) {
            emit setPositionDone(false, home, actSt);
            return;
        }
        if (actSt != SerialMessage::END_R) {
            emit setPositionDone(false, home, actSt);
            return;
        }
        values = s.getValues();
        emit setPositionDone(true, home, actSt);
    }
}


bool SerialDevice::connected()
{
    const QMutexLocker locker(&connMutex);
    return m_connected;
}

void SerialDevice::setConnected(bool connected)
{
    const QMutexLocker locker(&connMutex);
    m_connected = connected;
}

void SerialDevice::setRoletaHomeJob()
{
    DEBUGSER(QString("Ustaw pozycje startowa dla rolety"));
    setRoletaJobLocal(true);
}

void SerialDevice::setRoletaJob()
{
    DEBUGSER(QString("Ustaw pozycje rolety x=%1").arg(m_stepR));
    setRoletaJobLocal(false);
}

void SerialDevice::readRadioJob()
{
    DEBUGSER(QString("Pobieram dane z radia"));
    auto s = write(SerialMessage::measValuesMsg(),
              100, 10000);

    if (s.getParseReply() != SerialMessage::RADIOREAD_REPLY) {
        emit readFromRadio(false, 0, 0, 0, 0);
        return;
    }
    if (!s.getRadioOk()) {
        emit readFromRadio(false, 0, 0, 0, 0);
        return;
    }
    int val1, val2, val3, val4;

    s.readRadioValues(val1,val2,val3,val4);
    emit readFromRadio(true, val1, val2, val3, val4);
}

void SerialDevice::closeDeviceJob()
{
    DEBUGSER("CLOSING DEVICE");
    //setStop();
#ifdef SERIALLINUX
    m_serialPort->close();
#else
    RS232_CloseComport(m_portNr);
    m_portNr = -1;
#endif
    setConnected(false);
    emit kontrolerConfigured(false, CLOSE);
    DEBUGSER("CLOSE DEVICE");
}

void SerialDevice::resetJob()
{
    DEBUGSER(QString("Resetuje urzadzenie"));
    auto s = write(SerialMessage::resetSterownik(),
        100, 10000);

    if (s.getParseReply() != SerialMessage::RESET_REPLY) {
        return;
    }
}

void SerialDevice::setQuiteMode(bool mode)
{
    emitSignal = !mode;
}


bool SerialDevice::openDevice()
{
#ifdef SERIALLINUX
    m_serialPort->setPort(QSerialPortInfo(m_portName));

    emit deviceName(m_serialPort->portName());

    if (!m_serialPort->open(QIODevice::ReadWrite)) {
        emit error(QString(QObject::tr("Nie mozna otworzyc urzadzenia %1, error  %2")).arg(m_portName).
                   arg(m_serialPort->errorString()));
        emit kontrolerConfigured(false, NO_OPEN);
        return false;
    }

    emit kontrolerConfigured(false, OPEN);
    m_serialPort->setBaudRate(QSerialPort::Baud115200);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);

    return true;
#else
    DEBUGSER(QString("Otwieram urządzenia %1").arg(m_portName));
    char mode[]={'8','O','1',0};
    if (RS232_OpenComport(m_portNr, 115200, mode, 0)) {
        DEBUGSER(QString("Error open Port"));
        emit kontrolerConfigured(false, NO_OPEN);
        return false;
    }

    RS232_flushRXTX(m_portNr);

    const unsigned char startBuf[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int rs = RS232_SendBuf(m_portNr, (unsigned char*)startBuf, 16);

    if (rs <= 0) {
        RS232_CloseComport(m_portNr);
        m_portNr = -1;
        emit kontrolerConfigured(false, NO_READ);
        return false;
    }
    QThread::currentThread()->msleep(200);
    unsigned char recvBuf[100];
    int recv = RS232_PollComport(m_portNr, recvBuf, 1000);
    (void)recv;

    emit kontrolerConfigured(false, OPEN);
    return true;
#endif
}

SerialMessage SerialDevice::parseMessage(const QByteArray &reply)
{
    SerialMessage msg;
    msg.parseCommand(reply);
    return msg;
}

void SerialDevice::setReset()
{
    if (connected()) {
        m_worker.command(SerialWorker::RESET);
        m_worker.command(SerialWorker::SET_PARAMS);
    }
}

void SerialDevice::setRoletaClose()
{
    if (connected()) {
        m_worker.command(SerialWorker::QUIET_ON);
        m_worker.command(SerialWorker::RESET);
        m_worker.command(SerialWorker::SET_PARAMS);
        m_worker.command(SerialWorker::SET_ROLETA_HOME);
        m_worker.command(SerialWorker::DISCONNECT);
        m_worker.command(SerialWorker::QUIET_OFF);
    }
    else {
        m_worker.command(SerialWorker::QUIET_ON);
        m_worker.command(SerialWorker::CONNECT);
        m_worker.command(SerialWorker::SET_ROLETA_HOME);
        m_worker.command(SerialWorker::DISCONNECT);
        m_worker.command(SerialWorker::QUIET_OFF);
    }
}

const SerialMessageValues &SerialDevice::getValues() const
{
    return values;
}

SerialMessage SerialDevice::write(const QByteArray &currentRequest, int currentWaitWriteTimeout, int currentReadWaitTimeout)
{

    SerialMessage msg;
    msg.setInvalidReply();

#ifdef SERIALLINUX
    if (currentRequest.size() > 0) {
        DEBUGSER("Sending bytes....");
        int sendBytes = m_serialPort->write(currentRequest);
        if (!m_serialPort->waitForBytesWritten(currentWaitWriteTimeout)) {
            DEBUGSER(QString("Timeout Write %1").arg(currentWaitWriteTimeout));
            emit error(QString("Timeout Write"));
            return msg;
        }
        DEBUGSER(QString("Write %1 bytes [%2]").arg(sendBytes).arg(currentRequest.toHex(' ').constData()));
    }

    QByteArray responseData;
    DEBUGSER(QString("Wait for read"));
    if (m_serialPort->waitForReadyRead(currentReadWaitTimeout)) {
        responseData = m_serialPort->readAll();
        while (m_serialPort->waitForReadyRead(10))
            responseData += m_serialPort->readAll();
            DEBUGSER(QString("Read %1 [%2]").arg(responseData.size()).arg(responseData.toHex(' ').constData()));
            return parseMessage(responseData);
    } else {
        DEBUGSER(QString("Timeout Read %1").arg(currentReadWaitTimeout));
        emit error(QString("Timeout Read"));
        return msg;
    }
#else
    
    if (currentRequest.size() > 0)
    {

        DEBUGSER(QString("write [%1]").arg(currentRequest.toHex(' ').constData()));
        QElapsedTimer timer;
        timer.start();
        int sendBytes = RS232_SendBuf(m_portNr, (unsigned char*)currentRequest.constData(), currentRequest.size());
        if (sendBytes == 0) {
            DEBUGSER(QString("Write timeout %1").arg(sendBytes).arg(timer.elapsed()));
            msg.setTimeoutReply(true);
            return msg;
        }
        DEBUGSER(QString("write %1 bytes [%2 ms]").arg(sendBytes).arg(timer.elapsed()));
        QThread::currentThread()->msleep(currentWaitWriteTimeout);
    }
    unsigned char recvBuffor[20];
    int readTimeout = currentReadWaitTimeout;
    int rc = 0;
    QElapsedTimer timer;
    timer.start();
    unsigned short len;

    //1 znak
    memset(recvBuffor, 0, 20);
    do {
        rc = RS232_PollComport(m_portNr, recvBuffor, 1);
        if (rc > 0) {
            DEBUGSER(QString("recv %1 bytes").arg(rc));
        }
        if (rc == 0) {
            readTimeout -= 50;
            QThread::currentThread()->msleep(50);
        }
        if (recvBuffor[0] == 0xf0)
            continue;
    } while (rc == 0 && readTimeout > 0);
    QByteArray responseData;
    if (rc == 1) {
        responseData.append((const char*)recvBuffor, 1);
        len = recvBuffor[0] & 0x0f;
    }
    else {
        emit error(QString("Timeout"));
        DEBUGSER(QString("Timeout Read %1").arg(currentReadWaitTimeout));
        msg.setTimeoutReply(false);
        return msg;
    }

    //reszta znakow
    do {
        rc = RS232_PollComport(m_portNr, recvBuffor, len+1);
        if (rc > 0) {
            DEBUGSER(QString("recv %1 bytes").arg(rc+1));
        }
        responseData.append((const char*)recvBuffor, rc);
        if (responseData.size() == len + 2)
            break;
        
        readTimeout -= 50;
        QThread::currentThread()->msleep(50);
        
    } while(readTimeout > 0);
    unsigned long ms = timer.elapsed();

    if (readTimeout <= 0) {
        emit error(QString("Timeout"));
        DEBUGSER(QString("Timeout Read %1").arg(currentReadWaitTimeout));
        msg.setTimeoutReply(false);
        return msg;
    }
    //responseData.append((const char*)recvBuffor, rc);
    DEBUGSER(QString("read [%1] [%2 ms]").arg(responseData.right(rc+1).toHex(' ').constData()).arg(ms));
    return parseMessage(responseData);
#endif
}

void SerialDevice::closeDevice(bool waitForDone)
{
    DEBUGSER(QString("close device %1").arg(waitForDone));
    if (waitForDone) {
        m_worker.command(SerialWorker::DISCONNECT);
    } else {
        m_worker.setReset();
        closeDeviceJob();
    }
}

void SerialDevice::debugFun(const QString &log)
{
    emit debug(log);
}

void SerialDevice::connectToSerialJob()
{
    if (!connected()) {
        QString description;
        QString manufacturer;
        QString serialNumber;

        QString systemLocation = "";
        bool findDevice = false;

        DEBUGSER(QString("Szukam urządzenia"));

        const auto serialPortInfos = QSerialPortInfo::availablePorts();

        for (const QSerialPortInfo &serialPortInfo : serialPortInfos) {
            description = serialPortInfo.description();
            manufacturer = serialPortInfo.manufacturer();
            serialNumber = serialPortInfo.serialNumber();
            DEBUGSER(QString("Znaleziono : DESC ") + description + QString(" MENU ") + manufacturer + QString(" SERIAL") + serialNumber);

            if (serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()) {
                auto vendorId = serialPortInfo.vendorIdentifier();
                auto productId = serialPortInfo.productIdentifier();
                DEBUGSER(QString("Znaleziono kandydata"));
                if (vendorId == 6991 && productId == 37382 /* && serialNumber == serialNumberKontroler */) {
                    m_portName = serialPortInfo.portName();
                    emit deviceName(m_portName);
                    emit kontrolerConfigured(false, FOUND);
                    systemLocation = serialPortInfo.systemLocation();
                    DEBUGSER(QString("Znaleziono urządzenie"));
                    findDevice = true;
                }
            }
        }
        if (!findDevice) {
            emit kontrolerConfigured(false, NO_FOUND);
            return;
        }

    #ifndef SERIALLINUX
        m_portNr = -1;
        //GetComPortUsb(bufPortName,"1B4F","9206");
        m_portNr = RS232_GetPortnr(m_portName.toStdString().c_str());

        if (m_portNr == -1)
            return;
    #endif
        DEBUGSER("openDevice");
        setConnected(openDevice());
    }

    if (connected()) {
        setConnected(configureDevice());
    }

    if (!connected()) {
        closeDevice(false);
    }

    if (emitSignal) emit kontrolerConfigured(true, ALL_OK);
}

