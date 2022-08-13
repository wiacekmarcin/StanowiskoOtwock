#include "serialmessage.h"
#include "crc8.h"

#include <QSerialPortInfo>
#include <QSerialPort>
#include <QTextStream>
#include <QCoreApplication>

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

    emit debug(QString("Wyslalem %1").arg(writeData.toHex().toStdString().c_str()));
    //m_timer.start(1000);
}




SerialMessage::SerialMessage(QObject *parent) :
    QObject(parent)
{
    portName = "N/A";
    connSerial = false;

    connect(&m_serialPort, SIGNAL(readyRead()), this, SLOT(handleReadyRead()));
    connect(&m_serialPort, SIGNAL(errorOccurred(QSerialPort::SerialPortError)), this, SLOT(serialError(QSerialPort::SerialPortError)));

    connect(this, SIGNAL(setParams1()), this, SLOT(doneSettings1()));
    connect(this, SIGNAL(setParams2()), this, SLOT(doneSettings2()));

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
    switch (error) {
    case QSerialPort::NoError:
    default:
        break;

    case QSerialPort::DeviceNotFoundError:
        emit errorSerial("Nie znaleziono urządzenia");
        break;

    case QSerialPort::PermissionError:
        emit errorSerial("Brak uprawnień do urządzenia");
        break;

    case QSerialPort::OpenError:
        emit errorSerial("Błąd podczas otwierania urządzenia");
        break;

    case QSerialPort::ParityError:
        emit errorSerial("Błąd parzystości");
        break;

    case QSerialPort::FramingError:
    case QSerialPort::BreakConditionError:
        emit errorSerial("Błąd danych");
        break;

    case QSerialPort::WriteError:
        emit errorSerial("Błąd zapisu");
        break;

    case QSerialPort::ReadError:
        emit errorSerial("Błąd odczytu");
        break;

    case QSerialPort::ResourceError:
        emit errorSerial("Błąd zasobu");
        break;

    case QSerialPort::UnsupportedOperationError:
        emit errorSerial("Niedozwolona operacja");
        break;

    case QSerialPort::UnknownError:
        emit errorSerial("Nieznany błąd");
        break;

    case QSerialPort::TimeoutError:
        emit errorSerial("Błąd timeout");
        break;

    case QSerialPort::NotOpenError:
        emit errorSerial("Inny błąd");
        break;

    }
}

void SerialMessage::checkController()
{

}

void SerialMessage::connectToSerial()
{
    const auto serialPortInfos = QSerialPortInfo::availablePorts();

    QString description;
    QString manufacturer;
    QString serialNumber;
    const QString serialNumberKontroler = "D76ED16151514C4B39202020FF012E1C";
    closeDevice();

    for (const QSerialPortInfo &serialPortInfo : serialPortInfos) {
        description = serialPortInfo.description();
        manufacturer = serialPortInfo.manufacturer();
        serialNumber = serialPortInfo.serialNumber();


       //if (description == "Arduino Nano Every" &&
       //         manufacturer == "Arduino LLC") {
       //Pod windowsem jest uniwersalne urzadzenie USB firmy Microsoft
            if (serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()) {
                auto vendorId = serialPortInfo.vendorIdentifier();
                auto productId = serialPortInfo.productIdentifier();
                if (vendorId == 9025 && productId == 88 && serialNumber == serialNumberKontroler) {
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
    //emit debug(QString(homePositionMsg().toHex().toStdString().c_str()));
    emit debug(QString("Ustawiam pozycje startowa"));
    writeMessage(homePositionMsg());
}

void SerialMessage::setSettings1(bool reverseX, bool reverseY, uint32_t maxImpX, uint32_t maxImpY)
{
    //emit debug(QString(settings1Msg(reverseX, reverseY, maxImpX, maxImpY).toHex().toStdString().c_str()));
    writeMessage(settings1Msg(reverseX, reverseY, maxImpX, maxImpY));
}

void SerialMessage::setSettings2(uint32_t stepImpX, uint32_t stepImpY)
{
    //emit debug(QString(settings2Msg(stepImpX, stepImpY).toHex().toStdString().c_str()));
    writeMessage(settings2Msg(stepImpX, stepImpY));
}

void SerialMessage::doneSettings1()
{
    setSettings2(memoryStepX, memoryStepY);
}

void SerialMessage::doneSettings2()
{
    emit setParamsDone();
}

void SerialMessage::setPosition(uint32_t x, uint32_t y)
{
    //emit debug(QString(positionMsg(x, y).toHex().toStdString().c_str()));
    emit debug(QString("Ustawiam pozycje %1 %2").arg(x).arg(y));
    writeMessage(positionMsg(x, y));
}

void SerialMessage::setParams(bool reverseX, bool reverseY, uint32_t maxImpX, uint32_t maxImpY, uint32_t maxStepX, uint32_t maxStepY)
{
    emit debug(QString("Ustawiam parametry urzadzenia : reverseX=%1 reverseY=%2 maxImpX=%3 maxImpY=%4 maxStepX=%5 maxStepY=%6")
               .arg(reverseX).arg(reverseY).arg(maxImpX).arg(maxImpY).arg(maxStepX).arg(maxStepY));
    setSettings1(reverseX, reverseY, maxImpX, maxImpY);

    memoryStepX = maxStepX;
    memoryStepY = maxStepY;
}

void SerialMessage::readValueFromRadio()
{
    emit debug("Zczytaj wartosc z czujnika");
    //emit debug(QString(measValuesMsg().toHex().toStdString().c_str()));
    writeMessage(measValuesMsg());
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
    return prepareMessage(MOVEHOME_REQ, NULL, 0);
}

QByteArray SerialMessage::positionMsg(uint32_t x, const uint32_t y)
{
    uint8_t tab[8];
    tab[0] = (y >> 24) & 0xff;
    tab[1] = (y >> 16) & 0xff;
    tab[2] = (y >> 8) & 0xff;
    tab[3] = y & 0xff;
    tab[4] = (x >> 24) & 0xff;
    tab[5] = (x >> 16) & 0xff;
    tab[6] = (x >> 8) & 0xff;
    tab[7] = x & 0xff;
    return prepareMessage(POSITION_REQ, tab, 8);
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

QByteArray SerialMessage::settings1Msg(bool reverseX, bool reverseY, uint32_t maxImpX, uint32_t maxImpY)
{
    uint8_t tab[10];
    tab[0] = 0x01;
    tab[1] = (reverseX ? 0x01 : 0x00) | (reverseY ? 0x02 : 0x00);
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

QByteArray SerialMessage::settings2Msg(uint32_t stepImpX, uint32_t stepImpY)
{
    uint8_t tab[9];
    tab[0] = 0x02;
    tab[1] = (stepImpX >> 24) & 0xff;
    tab[2] = (stepImpX >> 16) & 0xff;
    tab[3] = (stepImpX >> 8) & 0xff;
    tab[4] = stepImpX & 0xff;
    tab[5] = (stepImpY >> 24) & 0xff;
    tab[6] = (stepImpY >> 16) & 0xff;
    tab[7] = (stepImpY >> 8) & 0xff;
    tab[8] = stepImpY & 0xff;
    return prepareMessage(SET_PARAM_REQ, tab, 9);
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
        //qDebug("arr = %s", arr.toHex().toStdString().c_str());
        //qDebug("cmd = %d", cmd);
        //qDebug("len = %d", len);
        return false;
    }
    //emit debug("Naglowek OK");
    return true;
}

bool SerialMessage::parseCommand(const QByteArray &arr)
{
    uint8_t cmd;
    uint8_t len;
    QByteArray data;



    if (!checkHead(arr, cmd, len, data)) {
        emit debug(QString("CheckHead faild %1").arg(arr.toHex(' ').toStdString().c_str()));
        return false;
    }

    if (cmd != MEASVALUE_REP)
        emit debug(QString("Parse cmd:") + QString(arr.toHex(' ').toStdString().c_str()));

    switch (cmd) {
        case WELCOME_REP:
        {
            if (len != 15) {
                qDebug("len != 15");
                return false;
            }
            uint8_t wzorzec[15] = {'K','O','N','T','R','O','L','E','R','W','I','A','T','R', 'U'};
            for (int i = 0; i < 15; ++i) {
                if (wzorzec[i] != data[i]) {
                    //qDebug("wzorzec != data");
                    return false;
                }
            }

            emit debug("Kontroler OK");
            emit controllerOK();

            return true;
        }
        case MOVEHOME_REP:
        {
            //set home position
            //0xB0 CRC8 - req
            //0xC1 s/L/D/p/g/K CRC8 - s=start, p=start lewoprawo, L=stop lewoprawo, g=start goradol D=stop goradol K=stop all
            if (len == 1) {
                switch(data[0]) {
                    case 's':
                        emit debug("Rozpoczynam pozycjonowanie do bazy");
                        emit startingHome();
                        return true;
                    case 'p':
                        emit debug("Rozpoczynam pozycjonowanie do bazy dla osi X");
                        emit startingHomeX();
                        return true;
                    case 'g':
                        emit debug("Rozpoczynam pozycjonowanie do bazy dla osi Y");
                        emit startingHomeY();
                        return true;
                    case 'K':
                        emit debug("Zakonczylem pozycjonowanie do bazy");
                        emit doneHome();
                        return true;
                    case 'E':
                        emit debug("Blad podczas pozycjonowania do bazy");
                        emit errorHome();
                        return true;
                    default:
                        return false;
                }
            }
            if (len == 5) {
                uint32_t step;
                switch(data[0]) {
                    case 'L':
                        step = data[1] << 24 | data[2] << 16 | data[3] << 8 | data[4];
                        emit debug(QString("Zakonczylem pozycjonowanie do bazy dla osi X. Ilosc krokow = %1").arg(step));
                        emit doneHomeX();
                        return true;
                    case 'D':
                        step = data[1] << 24 | data[2] << 16 | data[3] << 8 | data[4];
                        emit debug(QString("Zakonczylem pozycjonowanie do bazy dla osi Y. Ilosc krokow = %1").arg(step));
                        emit doneHomeY();
                        return true;
                    default:
                        return false;
                }
            }
            return false;
        }

        case POSITION_REP:
        {
        //0xC1 s/L/D/p/g/K CRC8 - s=start, p=start lewoprawo, L=stop lewoprawo, g=start goradol D=stop goradol K=stop all
            if (len == 1) {
                switch(data[0]) {
                    case 's':
                        emit debug("Rozpoczynam ustawianie pozycji");
                        emit startingPosition();
                        return true;
                    case 'l':
                        emit debug("Rozpoczynam ustawianie pozycji dla osi X");
                        emit startingPositionX();
                        return true;
                    case 'd':
                        emit debug("Rozpoczynam ustawianie pozycji dla osi Y");
                        emit startingPositionY();
                        return true;
                    case 'K':
                        emit debug("Zakonczylem ustawianie pozycji");
                        emit donePosition();
                        return true;
                    default:
                        return false;
                }
            }
            if (len == 9) {
                unsigned int step;
                uint32_t posX, posY;
                switch(data[0]) {
                    case 'P':
                        step = data[1] << 24 | data[2] << 16 | data[3] << 8 | data[4];
                        posX = data[5] << 24 | data[6] << 16 | data[7] << 8 | data[8];
                        emit debug(QString("Zakonczylem pozycjonowanie osi X (kroki = %1). Pozycja w krokach").arg(step).arg(posX));
                        emit donePositionX(step);
                        return true;
                    case 'G':
                        step = data[1] << 24 | data[2] << 16 | data[3] << 8 | data[4];
                        posY = data[5] << 24 | data[6] << 16 | data[7] << 8 | data[8];
                        emit debug(QString("Zakonczylem pozycjonowanie osi Y (kroki = %1). Pozycja w krokach").arg(step).arg(posY));
                        emit donePositionY(step);
                        return true;
                    default:
                        return false;
                }
            }
            return false;
        }

        case MEASVALUE_REP:
        {
            //getCzujVal
            //0x70 CRC8 - req
            //0x8A 'O' X2 X1 Y2 Y1 W2 W1 Z2 Z1 CRC8 - ok, wartosci odczytane z radia
            //0x81 'E' CRC8 - error polaczenia z radiem
            if (len == 1 && data[0] == 'E') {
                emit debug("Blad odczytu z radia");
                emit errorReadFromRadio();
                return true;
            }
            unsigned int val1, val2, val3, val4;
            if (len == 9 && data[0] == 'O') {
                unsigned char c1 = data[1];
                unsigned char c2 = data[2];
                unsigned char c3 = data[3];
                unsigned char c4 = data[4];
                unsigned char c5 = data[5];
                unsigned char c6 = data[6];
                unsigned char c7 = data[7];
                unsigned char c8 = data[8];
                val1 = 0;
                val2 = 0;
                val3 = 0;
                val4 = 0;
                val1 = (c1 << 8) | (c2);
                val2 = (c3 << 8) | (c4);
                val3 = (c5 << 8) | (c6);
                val4 = (c7 << 8) | (c8);
                //qDebug("dane z radia s%x %x %d",c1, c2, val1);

                (void)val2;(void)val3;(void)val4;
                //emit debug(QString("Odczytana wartosc w mV = %1").arg(val1));
                emit readedFromRadio(val1);
                return true;
            }
            if (len == 12 && data[0] == 'I') {
                return true;
            }
            return false;
        }

        case SET_PARAM_REP:
        {
            if (data[0] == (char)1) {
                emit setParams1();
                return true;
            } else if (data[0] == (char)2) {
                emit setParams2();
                return true;
            }

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
    //emit debug(QString("Wysylam:")+QString(ret.toHex(' ').data()));
    return ret;
}

