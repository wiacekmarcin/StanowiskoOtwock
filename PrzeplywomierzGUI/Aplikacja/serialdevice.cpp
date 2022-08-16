#include "serialdevice.h"
#include <QTime>
#include <QThread>
#include <QSerialPortInfo>
#include "rs232.h"
#include <QDebug>

#define DEBUGSER(X) debugFun(QString("%1:%2 %3").arg(__FILE__).arg(__LINE__).arg(X))

SerialWorker::SerialWorker(SerialDevice * device):
    QThread(nullptr),
    sd(device)
{
    actTask = IDLE;
    runWorker = true;

    connect(this, &SerialWorker::debug, device, &SerialDevice::debug);
}

SerialWorker::~SerialWorker()
{
    mutex.lock();
    runWorker = false;
    actTask = IDLE;
    newTask.wakeOne();
    mutex.unlock();
}

bool SerialWorker::command(Task curr)
{
    DEBUGSER(QString("New command %1").arg(curr));
    const QMutexLocker locker(&mutex);
    if (!runWorker)
        return false;
    actTask = curr;
    newTask.wakeOne();
    return true;
}

void SerialWorker::setStop()
{
    mutex.lock();
    runWorker = false;
    actTask = IDLE;
    
    newTask.wakeOne();
    mutex.unlock();
    //wait();
}

void SerialWorker::run()
{
    mutex.lock();
    short zadanie = actTask;
    mutex.unlock();
    short nrTrying = 0;
    bool quit = false;
    DEBUGSER(QString("actTask = %1").arg(actTask));
    do {
        switch(zadanie) {
        case IDLE:
                mutex.lock();
                newTask.wait(&mutex);
                zadanie = actTask;
                mutex.unlock();
            break;
        case CONNECT:
            sd->connectToSerialJob();
            if (!sd->m_connected) {
                zadanie = IDLE;
            } else if (!sd->m_configured) {
                if (nrTrying++ > 5) {
                    nrTrying = 0;
                    zadanie = IDLE;
                    sd->closeDevice();
                    sd->m_connected = false;
                    sd->m_configured = false;
                } else {
                    zadanie = CONFIGURE;
                }
            } else if (sd->m_connected && sd->m_configured) {
                nrTrying = 0;
                zadanie = IDLE;
            }
            break;

        case CONFIGURE:
            sd->m_configured = sd->configureDeviceJob();
            if (!sd->m_configured) {
                sd->closeDevice();
                sd->m_connected = false;
                sd->m_configured = false;
            }
            zadanie = IDLE;
            break;

        case SET_PARAMS:
            if (!sd->m_connected) {
                zadanie = CONNECT;
                sd->m_configured = false;

            } else {
                sd->setParamsJob();
                zadanie = IDLE;
            }
            break;

        case SET_HOME:
            sd->setHomeJob();
            zadanie = IDLE;
            break;

        case SET_POSITION:
            sd->setPosJob();
            zadanie = IDLE;
            break;
        
        case SET_ROLETA:
            sd->setRoletaJob();
            zadanie = IDLE;
            break;

        case SET_ROLETA_HOME:
            sd->setRoletaHomeJob();
            zadanie = IDLE;
            break;

        case READ_RADIO:
            sd->readRadioJob();
            zadanie = IDLE;
            break;

        default:
            zadanie = IDLE;
            break;
        }
        mutex.lock();
        quit = !runWorker;
        mutex.unlock();
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
      m_connected(false), m_configured(false), m_worker(this),
      waitForClose(false)
{
    m_reverseX = false;
    m_reverseY = false;
    m_reverseR = false;

    m_maxImpX = 0;
    m_maxImpY = 0;

    m_maxStepX = 0;
    m_maxStepY = 0;
    m_maxStepR = 0;

    m_worker.command(SerialWorker::IDLE);
}

SerialDevice::~SerialDevice()
{
    setStop();
}

void SerialDevice::setThread(QThread *trh)
{
    m_worker.moveToThread(trh);
    //m_worker.start();
}

void SerialDevice::setStop()
{
    m_worker.setStop();
}

void SerialDevice::connectToDevice()
{
    if (m_connected) {

    } else {
        m_worker.start();
        if (!m_worker.command(SerialWorker::CONNECT)) {
            emit kontrolerConfigured(false, true, false, false, false, false);
        }
    }
}

void SerialDevice::setParams(bool reverseX, bool reverseY, bool reverseR,
               uint32_t maxImpX, uint32_t maxImpY,
               uint32_t maxStepX, uint32_t maxStepY,
               uint32_t maxStepR)
{
    m_reverseX = reverseX;
    m_reverseY = reverseY;
    m_reverseR = reverseR;

    m_maxImpX = maxImpX;
    m_maxImpY = maxImpY;

    m_maxStepX = maxStepX;
    m_maxStepY = maxStepY;
    m_maxStepR = maxStepR;

}

void SerialDevice::insertParams()
{
    m_worker.command(SerialWorker::SET_PARAMS);
}

void SerialDevice::setPositionHome()
{
    m_worker.command(SerialWorker::SET_HOME);
}

void SerialDevice::setPosition(uint32_t x, uint32_t y)
{
    m_impX = x;
    m_impY = y;
    m_worker.command(SerialWorker::SET_POSITION);
}

void SerialDevice::setRoletaHome()
{
    m_worker.command(SerialWorker::SET_ROLETA_HOME);
}

void SerialDevice::setRoleta(uint32_t r)
{
    m_stepR = r;
    m_worker.command(SerialWorker::SET_ROLETA);
}

void SerialDevice::readRadio()
{
    m_worker.command(SerialWorker::READ_RADIO);
}

SerialMessage SerialDevice::write(const QByteArray &currentRequest, int currentWaitWriteTimeout, int currentReadWaitTimeout)
{

    SerialMessage msg;
    msg.setInvalidReply();
    
#ifdef SERIALLINUX
    if (currentRequest.size() > 0) {
        DEBUGSER("Sending bytes....");
        int sendBytes = m_serialPort.write(currentRequest);
        if (!m_serialPort.waitForBytesWritten(currentWaitWriteTimeout)) {
            qDebug() << "Timeout write";
            emit error(QString("Timeout Write"));
            return msg;
        }
        DEBUGSER(QString("Write %1 bytes").arg(sendBytes));
    }

    QByteArray responseData;
    DEBUGSER(QString("Wait for read"));
    if (m_serialPort.waitForReadyRead(currentReadWaitTimeout)) {
        responseData = m_serialPort.readAll();
        while (m_serialPort.waitForReadyRead(10))
            responseData += m_serialPort.readAll();
        DEBUGSER(QString("Read %1").arg(responseData.size()));
        return parseMessage(responseData);
    } else {
        DEBUGSER(QString("Error for read"));
        emit error(QString("Timeout Read"));
        return msg;
    }
#else

    if (currentRequest.size() > 0)
    {

        DEBUGSER(QString("write [%1]").arg(currentRequest.toHex().constData()));
        int sendBytes = RS232_SendBuf(m_portNr, (unsigned char*)currentRequest.constData(), currentRequest.size());
        DEBUGSER(QString("write %1 bytes").arg(sendBytes));
        QThread::currentThread()->msleep(currentWaitWriteTimeout);
    }
    unsigned char recvBuffor[20];
    int readTimeout = currentReadWaitTimeout;
    int rc = 0;
    do {

        rc = RS232_PollComport(m_portNr, recvBuffor, 20);
        DEBUGSER(QString("recv %1 bytes").arg(rc));
        if (rc == 0) {
            readTimeout -= 50;
            QThread::currentThread()->msleep(50);
        }
    } while(rc == 0 && readTimeout > 0);
    if (readTimeout <= 0) {
        emit error(QString("Timeout"));
    }
    QByteArray responseData((const char*)recvBuffor, rc);
    DEBUGSER(QString("read [%1]").arg(responseData.toHex().constData()));
    return parseMessage(responseData);
#endif
}

bool SerialDevice::configureDeviceJob()
{
    DEBUGSER(QString("Konfiguracja %1%2%3 imp=[%4,%5] steps=[%6,%7] R=%8").arg(m_reverseX).arg(m_reverseY).arg(m_reverseR)
    .arg(m_maxImpX).arg(m_maxImpY).arg(m_maxStepX).arg(m_maxStepY).arg(m_maxStepR));

    auto s = write(SerialMessage::welcomeMsg(), 100, 6000).getParseReply();

    if (s != SerialMessage::WELCOME_REPLY) {
        emit setParamsDone(false);
        emit kontrolerConfigured(true, false, true, false, true, true);
        return false;
    }

    //s = write(SerialMessage::setReset(), 100, 3000).getParseReply();
    //if (s != SerialMessage::RESET_REPLY)
    //    return false;

    //QThread::currentThread()->sleep(2);

    s = write(SerialMessage::settings1Msg(m_reverseX, m_reverseY, m_reverseR, m_maxImpX, m_maxImpY),
              100, 100).getParseReply();

    if (s != SerialMessage::SETPARAMS1_REPLY) {
        emit setParamsDone(false);
        emit kontrolerConfigured(true, false, true, false, true, true);
        return false;
    }

    s = write(SerialMessage::settings2Msg(m_maxStepX, m_maxStepY, m_maxStepR),
              100, 100).getParseReply();

    if (s != SerialMessage::SETPARAMS2_REPLY) {
        emit setParamsDone(false);
        emit kontrolerConfigured(true, false, true, false, true, true);
        return false;
    }

    emit kontrolerConfigured(true, false, true, false, true, false);
    emit setParamsDone(true);
    return true;
}

void SerialDevice::setParamsJob()
{
    DEBUGSER(QString("Konfiguracja %1%2%3 imp=[%4,%5] steps=[%6,%7] R=%8").arg(m_reverseX).arg(m_reverseY).arg(m_reverseR)
    .arg(m_maxImpX).arg(m_maxImpY).arg(m_maxStepX).arg(m_maxStepY).arg(m_maxStepR));

    auto s = write(SerialMessage::settings1Msg(m_reverseX, m_reverseY, m_reverseR, m_maxImpX, m_maxImpY),
              100, 100).getParseReply();

    if (s != SerialMessage::SETPARAMS1_REPLY) {
        emit setParamsDone(false); 
        return;
    }

    s = write(SerialMessage::settings2Msg(m_maxStepX, m_maxStepY, m_maxStepR),
              100, 100).getParseReply();

    if (s != SerialMessage::SETPARAMS2_REPLY) {
        emit setParamsDone(false);
        return;
    }

    emit setParamsDone(true);
}

void SerialDevice::setPosJobLocal(bool home)
{
    SerialMessage::ParseReply orp = home ? SerialMessage::MOVEHOME_REPLY : SerialMessage::POSITION_REPLY;

    auto s = write((home ? SerialMessage::setPositionHome() :
                    SerialMessage::setPosition(m_impX, m_impY)),
            100, 100);
    auto rp = s.getParseReply();
    auto actSt = s.getPosWork();
    if (rp != orp) {
        emit setPositionDone(false, home, actSt);
        return;
    } else {
        if (actSt != SerialMessage::START_XY) {
            emit setPositionDone(false, home, actSt);
            return;
        }   
        emit setPositionDone(true, home, actSt);
    }

    s = write(QByteArray(), 100, 2000);
    rp = s.getParseReply();
    actSt = s.getPosWork();
    if (rp != orp) {
        emit setPositionDone(false, home, actSt);
        return;
    } else {
        if (actSt != SerialMessage::START_X) {
            emit setPositionDone(false, home, actSt);
            return;
        }   
        emit setPositionDone(true, home, actSt);
    }
    
    s = write(QByteArray(), 100, 200000);
    rp = s.getParseReply();
    actSt = s.getPosWork();
    if (rp != orp) {
        emit setPositionDone(false, home, actSt);
        return;
    } else {
        if (actSt == SerialMessage::ERROR_XY) {
            emit setPositionDone(false, home, actSt);
            return;
        }
        if (actSt != SerialMessage::END_X) {
            emit setPositionDone(false, home, actSt);
            return;
        }   
        emit setPositionDone(true, home, actSt);
    }

    s = write(QByteArray(), 100, 2000);
    rp = s.getParseReply();
    actSt = s.getPosWork();
    if (rp != orp) {
        emit setPositionDone(home, false, actSt);
        return;
    } else {
        if (actSt != SerialMessage::START_Y) {
            emit setPositionDone(false, home, actSt);
            return;
        }   
        emit setPositionDone(true, home, actSt);
    }

    s = write(QByteArray(), 100, 200000);
    rp = s.getParseReply();
    actSt = s.getPosWork();
    if (rp != orp) {
        emit setPositionDone(false, home, actSt);
        return;
    } else {
        if (actSt == SerialMessage::ERROR_XY) {
            emit setPositionDone(false, home, actSt);
            return;
        }
        if (actSt != SerialMessage::END_Y) {
            emit setPositionDone(false, home, actSt);
            return;
        }   
        emit setPositionDone(true, home, actSt);
    }

    s = write(QByteArray(), 100, 2000);
    rp = s.getParseReply();
    actSt = s.getPosWork();
    if (rp != orp) {
        emit setPositionDone(false, home, actSt);
        return;
    } else {
        if (actSt != SerialMessage::END_XY) {
            emit setPositionDone(false, home, actSt);
            return;
        }   
        emit setPositionDone(true, home, actSt);
    }
}

void SerialDevice::setHomeJob()
{
    DEBUGSER(QString("Ustaw pozycje startowa dla czujnika"));
    setPosJobLocal(true);
    DEBUGSER(QString("setHomeJob - waitForClose = %1").arg(waitForClose));
    if (waitForClose) {
        waitForClose = false;
        closeDevice();
    }
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
            100, 100);
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
        emit setPositionDone(true, home, actSt);
    }

    s = write(QByteArray(), 100, 200000);
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
        emit setPositionDone(true, home, actSt);
    }
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
              100, 1000);

    if (s.getParseReply() != SerialMessage::RADIOREAD_REPLY) {
        emit readFromRadio(false, 0, 0, 0, 0);
        return;
    }
    int val1, val2, val3, val4;

    s.readRadioValues(val1,val2,val3,val4);
    emit readFromRadio(true, val1, val2, val3, val4);
}

bool SerialDevice::openDevice()
{
#ifdef SERIALLINUX
    m_serialPort.setPort(QSerialPortInfo(m_portName));

    emit deviceName(m_serialPort.portName());

    if (!m_serialPort.open(QIODevice::ReadWrite)) {
        emit error(QString(QObject::tr("Nie mozna otworzyc urzadzenia %1, error  %2")).arg(m_portName).arg(m_serialPort.errorString()));
        return false;
    }

    m_serialPort.setBaudRate(QSerialPort::Baud115200);
    m_serialPort.setDataBits(QSerialPort::Data8);
    m_serialPort.setFlowControl(QSerialPort::NoFlowControl);
    m_serialPort.setParity(QSerialPort::NoParity);
    m_serialPort.setStopBits(QSerialPort::OneStop);

    emit kontrolerConfigured(true, false, true, false, false, false);
    return true;
#else
    DEBUGSER(QString("Otwieram urządzenia %1").arg(m_portName));
    char mode[]={'8','O','1',0};
    if (RS232_OpenComport(m_portNr, 115200, mode, 0)) {
        DEBUGSER(QString("Error open Port"));
        m_connected = false;
        emit kontrolerConfigured(true, true, false, false, false, false);
        return false;
    }

    RS232_flushRXTX(m_portNr);

    const unsigned char startBuf[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int rs = RS232_SendBuf(m_portNr, (unsigned char*)startBuf, 16);

    if (rs <= 0) {
        RS232_CloseComport(m_portNr);
        emit kontrolerConfigured(true, false, true, true, false, false);
        return false;
    }
    QThread::currentThread()->msleep(200);
    unsigned char recvBuf[100];
    int recv = RS232_PollComport(m_portNr, recvBuf, 100);
    (void)recv;

    emit kontrolerConfigured(true, false, true, false, false, false);
    return true;
#endif
}

SerialMessage SerialDevice::parseMessage(const QByteArray &reply)
{
    SerialMessage msg;
    msg.parseCommand(reply);
    return msg;
}

void SerialDevice::closeDevice(bool afterBase)
{
    DEBUGSER(QString("close device %1").arg(afterBase));
    if (!afterBase) {
        closeDevice();
    } else {
        waitForClose = true;
        if (m_worker.getActTask() != SerialWorker::SET_HOME) {
            DEBUGSER(QString("Close now"));
            closeDevice();
        } else {
            DEBUGSER(QString("Waiting for close"));
        }
    }
}

void SerialDevice::closeDevice()
{
    DEBUGSER("CLOSING DEVICE");
    //setStop();
#ifdef SERIALLINUX
    m_serialPort.close();
#else
    RS232_CloseComport(m_portNr);
#endif
    m_connected = false;
    m_configured = false;
    emit kontrolerConfigured(false, false, false, false, false, false);
    DEBUGSER("CLOSE DEVICE");
}

void SerialDevice::debugFun(const QString &log)
{
    emit debug(log);
}

void SerialDevice::connectToSerialJob()
{
    QString description;
    QString manufacturer;
    QString serialNumber;

    QString systemLocation = "";
    bool findDevice = false;
    m_configured = false;
    DEBUGSER(QString("Szukam urządzenia"));
    emit kontrolerConfigured(false, false, false, false, false, false);

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
                emit kontrolerConfigured(true, false, false, false, false, false);
                systemLocation = serialPortInfo.systemLocation();
                DEBUGSER(QString("Znaleziono urządzenie"));
                findDevice = true;
            }
        }
    }
    if (!findDevice) {
        emit kontrolerConfigured(false, true, false, false, false, false);
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
    m_connected = openDevice();

    if (m_connected) {
        insertParams();
    }
}

