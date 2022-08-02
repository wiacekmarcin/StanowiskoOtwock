#include "serialmessage.h"
#include "crc8.h"

#include <QSerialPortInfo>
#include <QSerialPort>
#include <QTextStream>
#include <QCoreApplication>

#include <QDebug>

#ifdef Q_OS_WIN
#include <windows.h> // for Sleep
#endif
void qSleep(int ms)
{

#ifdef Q_OS_WIN
    Sleep(uint(ms));
#else
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
#endif
}

void SerialMessage::writeMessage(const QByteArray &writeData)
{

    const qint64 bytesWritten = m_serialPort.write(writeData);

    if (bytesWritten == -1) {
        emit debug(QObject::tr("Nie mozna zapisac do urzadzenia"));
        return;
    } else if (bytesWritten != writeData.size()) {
        emit debug(QObject::tr("Czesciowy zapis do urzadzenia"));
        return;
    }

    //emit debug(QString("Zapisalem %1").arg(writeData.size()));
    //m_timer.start(1000);
}




SerialMessage::SerialMessage(QObject *parent) :
    QObject(parent)
{
    portName = "N/A";
    connSerial = false;

    connect(&m_serialPort, SIGNAL(readyRead()), this, SLOT(handleReadyRead()));
    connect(&m_serialPort, SIGNAL(errorOccurred(QSerialPort::SerialPortError)), this, SLOT(serialError(QSerialPort::SerialPortError)));


    //connect(&m_timer, &QTimer::timeout, this, &SerialPortReader::handleTimeout);

    commands.clear();
    cmd.clear();

}

SerialMessage::~SerialMessage()
{
    if (m_serialPort.isOpen())
        m_serialPort.close();
}

void SerialMessage::handleReadyRead()
{
    QByteArray readData = m_serialPort.readAll();
    emit debug(QString("Odebralem:")+QString(readData.toHex(' ').toStdString().c_str()));

    while (readData.size() > 0 ) {

        //unsigned char c = readData.front();
        unsigned char c = readData.at(0);
        readData.remove(0,1);
        cmd.append(c);

        if (cmd.size() == 1) {
            lenCmd = c & 0x0f;
        } else if (cmd.size() - 2 == lenCmd) {
            parseCommand(cmd);
            cmd.clear();
            lenCmd = 0;
        }
    }

    //if (!m_timer.isActive())
    //    m_timer.start(m_timeout);
    //emit response(m_readData);
    //m_readData.clear();
    }

void SerialMessage::serialError(const QSerialPort::SerialPortError &error)
{
    emit errorSerial(QString("Blad %1").arg(error));
}

void SerialMessage::checkController()
{
    qDebug() << "checkController";
    if (setParamsWork) {
        qDebug() << __FILE__ << __LINE__ << "setParam";
        setSettings1(memoryreverseX, memoryreverseY, memoryreverseR, memorymaxImpX, memorymaxImpY);
        setParamsWork = false;
    }
}

void SerialMessage::connectToSerial()
{
    qDebug() << "Connect to serial";
    const auto serialPortInfos = QSerialPortInfo::availablePorts();

    QString description;
    QString manufacturer;
    QString serialNumber;
    const QString serialNumberKontroler = "D76ED16151514C4B39202020FF012E1C";


    for (const QSerialPortInfo &serialPortInfo : serialPortInfos) {
        description = serialPortInfo.description();
        manufacturer = serialPortInfo.manufacturer();
        serialNumber = serialPortInfo.serialNumber();
        qDebug() << "D" << description.toStdString().c_str()
                 << "M" << manufacturer.toStdString().c_str()
                 << "S" << serialNumber.toStdString().c_str();

       //if (description == "Arduino Nano Every" &&
       //         manufacturer == "Arduino LLC") {
       //Pod windowsem jest uniwersalne urzadzenie USB firmy Microsoft
            if (serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()) {

                auto vendorId = serialPortInfo.vendorIdentifier();
                auto productId = serialPortInfo.productIdentifier();
                qDebug() << vendorId << " : " << productId;
                if (vendorId == 6991 && productId == 37382 /*&& serialNumber == serialNumberKontroler*/) {
                    //if (sendMesgWelcome(serialPortInfo)) {
                    //    connSerial = true;
                    //    return;
                    //}
                    //TODO otworzmy urzadzenie
                    //a nastepnie wyslijmy wiadomosc powitalna welcomemsg i asynchronicznie oczekujmy odpowiedzi
                    emit successOpenDevice(openDevice(serialPortInfo));
                    return;
                }
            }
        //}


    }
    emit successOpenDevice(false);
}

void SerialMessage::setPositionHome()
{
    emit debug(QString(homePositionMsg().toHex().toStdString().c_str()));
    writeMessage(homePositionMsg());
}

void SerialMessage::setSettings1(bool reverseX, bool reverseY, bool reverseR, uint32_t maxImpX, uint32_t maxImpY)
{
    qDebug() << __FILE__ << __LINE__ << "set setttings 1";
    emit debug(QString(settings1Msg(reverseX, reverseY, reverseR, maxImpX, maxImpY).toHex().toStdString().c_str()));
    writeMessage(settings1Msg(reverseX, reverseY, reverseR, maxImpX, maxImpY));
}

void SerialMessage::setSettings2(uint32_t stepMaxX, uint32_t stepMaxY, uint32_t stepMaxR)
{
    qDebug() << __FILE__ << __LINE__ << "set setttings 2";
    emit debug(QString(settings2Msg(stepMaxX, stepMaxY, stepMaxR).toHex().toStdString().c_str()));
    writeMessage(settings2Msg(stepMaxX, stepMaxY, stepMaxR));
}

void SerialMessage::setPosition(uint32_t x, uint32_t y)
{
    emit debug(QString(positionMsg(x, y).toHex().toStdString().c_str()));
    writeMessage(positionMsg(x, y));
}

void SerialMessage::setRoletaHome()
{
    emit debug(QString(homeRoletaMsg().toHex().toStdString().c_str()));
    writeMessage(homeRoletaMsg());
}

void SerialMessage::setRoleta(uint32_t r)
{
    emit debug(QString(roletaMsg(r).toHex().toStdString().c_str()));
    writeMessage(roletaMsg(r));
}

void SerialMessage::setParams(bool reverseX, bool reverseY, bool reverseR,
                              uint32_t maxImpX, uint32_t maxImpY,
                              uint32_t maxStepX, uint32_t maxStepY,
                              uint32_t maxStepR)
{
    qDebug() << __FILE__ << __LINE__ << "Reset";
    setParamsWork = true;

    memoryreverseX = reverseX;
    memoryreverseY = reverseY;
    memoryreverseR = reverseR;
    memorymaxImpX = maxImpX;
    memorymaxImpY = maxImpY;


    memoryStepX = maxStepX;
    memoryStepY = maxStepY;
    memoryStepR = maxStepR;

    //sendReset();
    setSettings1(reverseX,reverseY,reverseR,maxImpX,maxImpY);
}

void SerialMessage::readRadio()
{
    emit debug(QString(measValuesMsg().toHex().toStdString().c_str()));
    writeMessage(measValuesMsg());
}

void SerialMessage::sendReset()
{
    qDebug() << __FILE__ << __LINE__ << "Reset";
    emit debug(QString(welcomeMsg().toHex().toStdString().c_str()));
    writeMessage(welcomeMsg());
}

void SerialMessage::closeDevice()
{
    if (m_serialPort.isOpen())
        m_serialPort.close();
}

void SerialMessage::response(const QByteArray &s)
{
    QByteArray resp = s;
    while(resp.size() > 0) {
        parseCommand(resp);
    }
}

bool SerialMessage::openDevice(const QSerialPortInfo &port)
{
    m_serialPort.setPort(port);
    portName = port.portName();
    emit deviceName(portName);

    if (!m_serialPort.open(QIODevice::ReadWrite)) {
        emit errorSerial(QString(QObject::tr("Nie mozna otworzyc urzadzenia %1, error  %2")).arg(portName).arg(m_serialPort.errorString()));
        return false;
    }
    connSerial = true;

    m_serialPort.setBaudRate(QSerialPort::Baud115200);
    m_serialPort.setDataBits(QSerialPort::Data8);
    m_serialPort.setFlowControl(QSerialPort::NoFlowControl);
    m_serialPort.setParity(QSerialPort::NoParity);
    m_serialPort.setStopBits(QSerialPort::OneStop);

    //emit checkController();
    writeMessage(welcomeMsg());
    return true;
}

QByteArray SerialMessage::welcomeMsg()
{
    return prepareMessage(WELCOME_REQ, NULL, 0);
}

QByteArray SerialMessage::homePositionMsg()
{
    uint8_t tab[1] = {'P'};
    return prepareMessage(MOVEHOME_REQ, tab, 1);
}

QByteArray SerialMessage::positionMsg(uint32_t x, const uint32_t y)
{
    uint8_t tab[9];
    tab[0] = 'P';
    tab[1] = (y >> 24) & 0xff;
    tab[2] = (y >> 16) & 0xff;
    tab[3] = (y >> 8) & 0xff;
    tab[4] = y & 0xff;
    tab[5] = (x >> 24) & 0xff;
    tab[6] = (x >> 16) & 0xff;
    tab[7] = (x >> 8) & 0xff;
    tab[8] = x & 0xff;
    return prepareMessage(POSITION_REQ, tab, 9);
}

QByteArray SerialMessage::homeRoletaMsg()
{
    uint8_t tab[1] = {'R'};
    return prepareMessage(MOVEHOME_REQ, tab, 1);
}

QByteArray SerialMessage::roletaMsg(uint32_t r)
{
    uint8_t tab[5];
    tab[0] = 'R';
    tab[1] = (r >> 24) & 0xff;
    tab[2] = (r >> 16) & 0xff;
    tab[3] = (r >> 8) & 0xff;
    tab[4] = r & 0xff;
    return prepareMessage(POSITION_REQ, tab, 5);
}

QByteArray SerialMessage::measValuesMsg()
{
    return prepareMessage(MEASVALUE_REQ, NULL, 0);
}

QByteArray SerialMessage::measUnitMsg(short index, const float &ratio, QString &unit)
{
    union tmp {
        float f;
        unsigned char a[4];
    } t;
    t.f = ratio;
    uint8_t tab[15];
    tab[0] = 0;
    tab[0] = (index & 0x0f) | (unit.size() & 0x0f) << 4;
    for (int i = 0; i < 4; i++) {
        tab[i+1] = t.a[i];
    }
    for (int i = 0; i < 10; i++) {
        if (i < unit.size())
            tab[i+5] = unit.at(i).toLatin1();
        else
            break;
    }
    return prepareMessage(MEASUNIT_REQ, tab, 5+unit.size());
}

QByteArray SerialMessage::settings1Msg(bool reverseX, bool reverseY, bool reverseR, uint32_t maxImpX, uint32_t maxImpY)
{
    uint8_t tab[10];
    tab[0] = 0x01;
    tab[1] = (reverseX ? 0x01 : 0x00) | (reverseY ? 0x02 : 0x00) | (reverseR ? 0x04 : 0x00) ;
    tab[2] = (maxImpX >> 24) & 0xff;
    tab[3] = (maxImpX >> 16) & 0xff;
    tab[4] = (maxImpX >> 8) & 0xff;
    tab[5] = maxImpX & 0xff;
    tab[6] = (maxImpY >> 24) & 0xff;
    tab[7] = (maxImpY >> 16) & 0xff;
    tab[8] = (maxImpY >> 8) & 0xff;
    tab[9] = maxImpY & 0xff;
    return prepareMessage(SET_PARAM_REQ, tab, 10);
}

QByteArray SerialMessage::settings2Msg(uint32_t maxStepX, uint32_t maxStepY, uint32_t maxStepR)
{
    uint8_t tab[13];
    tab[0] = 0x02;
    tab[1] = (maxStepX >> 24) & 0xff;
    tab[2] = (maxStepX >> 16) & 0xff;
    tab[3] = (maxStepX >> 8) & 0xff;
    tab[4] = maxStepX & 0xff;
    tab[5] = (maxStepY >> 24) & 0xff;
    tab[6] = (maxStepY >> 16) & 0xff;
    tab[7] = (maxStepY >> 8) & 0xff;
    tab[8] = maxStepY & 0xff;
    tab[9] = (maxStepR >> 24) & 0xff;
    tab[10] = (maxStepR >> 16) & 0xff;
    tab[11] = (maxStepR >> 8) & 0xff;
    tab[12] = maxStepR & 0xff;
    return prepareMessage(SET_PARAM_REQ, tab, 13);
}

uint32_t SerialMessage::getNumber(const QByteArray &data)
{
    qDebug() << data.toHex().toStdString().c_str();
    return ((data[0] & 0xff) << 24) +  ((data[1] & 0xff) << 16) + ((data[2] & 0xff) << 8) + (data[3] & 0xff);
}

int32_t SerialMessage::getPosStepR() const
{
    return posStepR;
}

int32_t SerialMessage::getPosImpY() const
{
    return posImpY;
}

int32_t SerialMessage::getPosImpX() const
{
    return posImpX;
}

int32_t SerialMessage::getMoveStepY() const
{
    return moveStepY;
}

int32_t SerialMessage::getMoveStepX() const
{
    return moveStepX;
}

int32_t SerialMessage::getMoveStepR() const
{
    return moveStepR;
}

bool SerialMessage::checkHead(const QByteArray &arr, uint8_t & cmd, uint8_t & len,  QByteArray & data)
{
    if (arr.length() == 0) {
        emit debug("Pusta komenda");
        return false;
    }

    cmd = (arr[0] >> 4) & 0x0f;
    len = arr[0] & 0x0f;

    CRC8 crc;
    data = QByteArray(len, Qt::Uninitialized);
    //crc.add(arr.at(0));
    int i = 0;
    for (i = 0; i < arr.length() - 1 && i <= len; i++) {
        crc.add(arr.at(i));
        if (i) {
            data[i-1] = arr.at(i);
        }
    }

    if (i-1 != len) {
        emit debug(QString("Nie poprawna dlugosc rozkazu"));
        return false;
    }
    if (i >= arr.size()) {
        emit debug(QString("Za dlugi rozkaz"));
        return false;
    }
    unsigned short msgcrc = arr.at(i) & 0xff;

    if (crc.getCRC() != msgcrc) {
        emit debug(QString("crc = %1x val=%2x").arg(crc.getCRC(), 16).arg(msgcrc,16));
        return false;
    }
    emit debug("Naglowek OK");
    return true;
}

bool SerialMessage::parseCommand(const QByteArray &arr)
{
    uint8_t cmd;
    uint8_t len;
    QByteArray data;

    emit debug(QString("Parse cmd:") + QString(arr.toHex(' ').toStdString().c_str()));

    if (!checkHead(arr, cmd, len, data)) {
        emit debug(QString("CheckHead faild"));
        return false;
    }

    switch (cmd) {
        case WELCOME_REP:
        {
            if (len != 15) {
                qDebug("len != 15");
                return false;
            }
            uint8_t wzorzec[15] = {'K','O','N','T','R','O','L','E','R','W','I','A','T','R', '2'};
            for (int i = 0; i < 15; ++i) {
                if (wzorzec[i] != data[i]) {
                    qDebug("wzorzec != data");
                    return false;
                }
            }

            emit debug("welcome msg");
            emit controllerOK();
            checkController();
            return true;
        }
        case MOVEHOME_REP:
        {
            //set home position
            //81 S/l/P/d/G/K/R/r/ CRC8 - rep home position
            //                                S=start, l=start lewoprawo, P=end lewoprawo,
            //                                d=start goradol G=end goradol K=endboth, R - start rolet, r - koniec rolet
            if (len == 1) {
                switch(data[0]) {
                    case 's':
                        emit debug("Starting home-base positioning");
                        emit homeStatus(START_XY);
                        return true;
                    case 'l':
                        emit debug("Starting X home-base positioning");
                        emit homeStatus(START_X);
                        return true;
                    case 'd':
                        emit debug("Starting Y home-base positioning");
                        emit homeStatus(START_Y);
                        return true;
                    case 'K':
                        emit debug("Home-base positioning done");
                        emit homeStatus(END_XY);
                        return true;
                    case 'r':
                        emit debug("Roleta home-base start");
                        emit homeStatus(START_R);
                        return true;
                    default:
                        return false;
                }
            } else if (len == 5) {
                switch(data[0]) {
                case 'P':
                    moveStepX = getNumber(data.mid(1, 4));
                    emit debug(QString("Ending X home-base positioning. Steps = %1").arg(moveStepX));
                    emit homeStatus(END_X);
                    return true;
                case 'G':
                    moveStepY = getNumber(data.mid(1, 4));
                    emit debug(QString("Ending Y home-base positioning. Steps = %1").arg(moveStepY));
                    emit homeStatus(END_Y);
                    return true;
                case 'R':
                    moveStepR = getNumber(data.mid(1, 4));
                    emit debug(QString("Ending Roleta. Steps = %1").arg(moveStepR));
                    emit homeStatus(END_R);
                    return true;
                default:
                    return false;
                }
            } else if (len == 2) {
                if (data[0] == 'E' && data[1] == 'P') {
                    emit debug("Error home-base positioning");
                    emit errorHome();
                    emit homeStatus(ERROR_XY);
                    return true;
                } else if (data[0] == 'E' && data[1] == 'R') {
                    emit debug("Error roleta home-base positioning");
                    emit errorHome();
                    emit homeStatus(ERROR_R);
                    return true;
                }
            }
            return false;
        }

        case POSITION_REP:
        {
        // 61 s/l/P/d/G/K/R/r/  CRC8 - reply setting position in proges
        //                              S=start,
        //                              l=start lewoprawo,
        //                              P=end lewoprawo,
        //                              d=start goradol
        //                              G=end goradol
        //                              K=endboth,
        //                              r - start rolet,
        //                              R - koniec rolet
        //62 E X/Y/R  CRC8 - reply error setting position X - os x, Y - os y, R - rolety
        //69 P STEP4 STEP3 STEP2 STEP1 POS4 POS3 POS2 POS1 CRC8 - reply ustawienie pozycji
        //69 G STEP4 STEP3 STEP2 STEP1 POS4 POS3 POS2 POS1 CRC8 - reply ustawienie pozycji
        //69 R STEP4 STEP3 STEP2 STEP1 POS4 POS3 POS2 POS1 CRC8 - reply ustawienie pozycji
        if (len == 1) {
            switch(data[0]) {
                case 's':
                    emit debug("Starting positioning");
                    emit positionStatus(START_XY);
                    return true;
                case 'l':
                    emit debug("Starting X positioning");
                    emit positionStatus(START_X);
                    return true;
                case 'd':
                    emit debug("Starting Y positioning");
                    emit positionStatus(START_Y);
                    return true;
                case 'K':
                    emit debug("Positioning done");
                    emit positionStatus(END_XY);
                    return true;
                case 'r':
                    emit debug("Roleta move start");
                    emit positionStatus(START_R);
                    return true;
                default:
                    return false;
            }
        } else if (len == 9) {
            switch(data[0]) {
                case 'P':
                    moveStepX = getNumber(data.mid(1, 4));
                    posImpX = getNumber(data.mid(5, 4));
                    emit debug(QString("Position X done: Steps = %1. Global pos = %2").arg(moveStepX).arg(posImpX));

                    emit positionStatus(END_X);
                    return true;
                case 'G':
                    moveStepY = getNumber(data.mid(1, 4));
                    posImpY = getNumber(data.mid(5, 4));
                    emit debug(QString("Position Y done: Steps = %1. Global pos = %2").arg(moveStepY).arg(posImpY));
                    emit positionStatus(END_Y);
                    return true;
                case 'R':
                    moveStepR = getNumber(data.mid(1, 4));
                    posStepR = getNumber(data.mid(5, 4));
                    emit debug(QString("Position R done: Steps = %1. Global pos = %2").arg(moveStepR).arg(posStepR));
                    emit positionStatus(END_R);
                    return true;
                default:
                    return false;
            }
        }
        return false;
            return false;
        }

        case MEASVALUE_REP:
        {
            //getCzujVal
            //0xa0 CRC8 - req
            //0xbA 'O' X2 X1 Y2 Y1 W2 W1 Z2 Z1 CRC8 - ok, wartosci odczytane z radia
            //0xb1 'E' CRC8 - error polaczenia z radiem
            if (len == 1 && data[0] == 'E') {
                emit errorReadFromRadio();
                return true;
            }
            unsigned int val1, val2, val3, val4;
            if (len == 9 && data[0] == 'O') {
                val1 = 0;
                val2 = 0;
                val3 = 0;
                val4 = 0;
                val1 = data[1] << 8 | data[2];
                val2 = data[3] << 8 | data[4];
                val3 = data[5] << 8 | data[6];
                val4 = data[7] << 8 | data[8];

                (void)val2;(void)val3;(void)val4;

                emit readFromRadio(val1);
            }
            return true;
        }

        case SET_PARAM_REP:
        {
            qDebug() << "setParams reply" << (unsigned int) data[0];
            if (data[0] == (char)1) {

                setSettings2(memoryStepX, memoryStepY, memoryStepR);
                return true;
            } 
            if (data[0] == (char)2) {
                emit setParamsDone();
                return true;
            }
            return false;
        }

        default:
            emit debug("Nieznana komenda");
            return false;
    }

    return false;
}

QByteArray SerialMessage::prepareMessage(uint8_t cmd, uint8_t tab[], uint8_t len)
{
    QByteArray arr(len+2,0);
    arr[0] = (cmd << 4) | (len & 0xff);
    CRC8 c;
    c.add(arr[0]);
    for (int i = 0; i < len; i++) {
        arr[i+1] = tab[i];
        c.add(tab[i]);
    }
    arr[len+1] = c.getCRC();

    QByteArray ret;
    //ret.append('U');
    ret.append(arr);
    //ret.append(0xaa);
    emit debug(QString("Wysylam:")+QString(ret.toHex(' ').data()));
    return ret;
}

