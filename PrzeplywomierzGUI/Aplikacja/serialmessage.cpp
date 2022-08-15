#if 0
#include "serialmessage.h"
#include "crc8.h"
#include "rs232.h"

#include <QSerialPortInfo>
#include <QSerialPort>
#include <QTextStream>
#include <QCoreApplication>
#include <QThread>
#include <QDebug>


bool SerialMessage::parseCommand(const QByteArray &arr)
{
    uint8_t cmd;
    uint8_t len;
    QByteArray data;

    emit debug(QString("Parsuje komende: [") + QString(arr.toHex()) + QString("]"));

    if (!checkHead(arr, cmd, len, data)) {
        emit debug(QString("Nie poprawny komunikat"));
        return false;
    }

    switch (cmd) {
        case WELCOME_REP:
        {
            if (len != 15) {
                //qDebug("len != 15");
                return false;
            }
            uint8_t wzorzec[15] = {'K','O','N','T','R','O','L','E','R','W','I','A','T','R', '2'};
            for (int i = 0; i < 15; ++i) {
                if (wzorzec[i] != data[i]) {
                    //qDebug("wzorzec != data");
                    connSerial = false;
                    emit deviceName("-");
                    m_serialPort.close();
                    return false;
                }
            }

            emit debug("welcome msg");
            emit controllerOK();
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
                        emit debug("Rozpoczynam pozycjonowanie bazowe");
                        emit homeStatus(START_XY);
                        return true;
                    case 'l':
                        emit debug("Rozpoczynam pozycjonowanie bazowe osi X");
                        emit homeStatus(START_X);
                        return true;
                    case 'd':
                        emit debug("Rozpoczynam pozycjonowanie bazowe osi Y");
                        emit homeStatus(START_Y);
                        return true;
                    case 'K':
                        emit debug("Pozycjonowa bazowe zakończone");
                        emit homeStatus(END_XY);
                        return true;
                    case 'r':
                        emit debug("Rozpoczynam pozycjonowanie rolety");
                        emit homeStatus(START_R);
                        return true;
                    default:
                        return false;
                }
            } else if (len == 5) {
                switch(data[0]) {
                case 'P':
                    moveStepX = getNumber(data.mid(1, 4));
                    emit debug(QString("Pozycjonowanie osi X zakończone. Kroki = %1").arg(moveStepX));
                    emit homeStatus(END_X);
                    return true;
                case 'G':
                    moveStepY = getNumber(data.mid(1, 4));
                    emit debug(QString("Pozycjonowanie osi Y zakończone. Kroki = %1").arg(moveStepY));
                    emit homeStatus(END_Y);
                    return true;
                case 'R':
                    moveStepR = getNumber(data.mid(1, 4));
                    emit debug(QString("Pozycjonowanie rolety zakończone. Kroki = %1").arg(moveStepR));
                    emit homeStatus(END_R);
                    return true;
                default:
                    return false;
                }
            } else if (len == 2) {
                if (data[0] == 'E' && data[1] == 'P') {
                    emit debug("Błąd pozycjonowania bazowego");
                    emit errorHome();
                    emit homeStatus(ERROR_XY);
                    return true;
                } else if (data[0] == 'E' && data[1] == 'R') {
                    emit debug("Błąd pozycjonowania bazowego rolety");
                    emit errorHomeRoleta();
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
                    emit debug("Rozpoczynam ustawianie puntku XY");
                    emit positionStatus(START_XY);
                    return true;
                case 'l':
                    emit debug("Rozpoczynam ustawianie punktu na osi X");
                    emit positionStatus(START_X);
                    return true;
                case 'd':
                    emit debug("Rozpoczynam ustawianie punktu na osi Y");
                    emit positionStatus(START_Y);
                    return true;
                case 'K':
                    emit debug("Ustawianie punktu na obu osiach zakończone");
                    emit positionStatus(END_XY);
                    return true;
                case 'r':
                    emit debug("Ustawienia rolety zakończone");
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
                    emit debug(QString("Ustawianie na osi X zakończone. Kroki = %1. Pozycja = %2 (%3 %)").arg(moveStepX).arg(posImpX).arg(100*posImpX/memorymaxImpX));

                    emit positionStatus(END_X);
                    return true;
                case 'G':
                    moveStepY = getNumber(data.mid(1, 4));
                    posImpY = getNumber(data.mid(5, 4));
                    emit debug(QString("Ustawianie na osi Y zakończone. Kroki = %1. Pozycja = %2 (%3 %").arg(moveStepY).arg(posImpY).arg(100*posImpY/memorymaxImpY));
                    emit positionStatus(END_Y);
                    return true;
                case 'R':
                    moveStepR = getNumber(data.mid(1, 4));
                    posStepR = getNumber(data.mid(5, 4));
                    emit debug(QString("Ustawianie rolety zakończone. Kroki = %1. Pozycja = %2 (%3 %").arg(moveStepR).arg(posStepR).arg(100*posStepR/memoryStepR));
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
            if (len == 2 && data[0] == 'E') {
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

        

        default:
            emit debug("Nieznana komenda");
            return false;
    }

    return false;
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




void SerialMessage::readRadio()
{
#ifdef SYMULATOR
    qSleep(500);
    emit readFromRadio(10);
    return;
#endif
    emit debug(QString("Wysyłam żądanie odczytu radia [") + QString(measValuesMsg().toHex()) + QString("]"));
    writeMessage(measValuesMsg());
}







#endif

#include "serialmessage.h"
#include "crc8.h"


#include <QDebug>

SerialMessage::SerialMessage()
{

}

SerialMessage::~SerialMessage()
{

}

QByteArray SerialMessage::welcomeMsg()
{
    return prepareMessage(WELCOME_REQ, NULL, 0);
}

QByteArray SerialMessage::setPositionHome(bool roleta)
{
    uint8_t tab[1] = { roleta ? (uint8_t)'R' : (uint8_t)'P' };
    return prepareMessage(MOVEHOME_REQ, tab, 1);
}

QByteArray SerialMessage::setPosition(uint32_t x, const uint32_t y)
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

QByteArray SerialMessage::setRoleta(uint32_t r)
{
    uint8_t tab[5];
    tab[0] = 'R';
    tab[1] = (r >> 24) & 0xff;
    tab[2] = (r >> 16) & 0xff;
    tab[3] = (r >> 8) & 0xff;
    tab[4] = r & 0xff;
    return prepareMessage(POSITION_REQ, tab, 5);
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


bool SerialMessage::checkHead(const QByteArray &arr, uint8_t & cmd, uint8_t & len,  QByteArray & data)
{
    m_errorText = "";
    m_errorBool = false;
    if (arr.length() == 0) {
        m_errorText = "Pusta komenda";
        m_errorBool = true;
        return false;
    }

    cmd = (arr[0] >> 4) & 0x0f;
    len = arr[0] & 0x0f;

    if (cmd == NOP_REP && len == 0xf)
        return true;
    CRC8 crc;
    data = QByteArray(len, Qt::Uninitialized);

    int i = 0;
    for (i = 0; i < arr.length() - 1 && i <= len; i++) {
        crc.add(arr.at(i));
        if (i) {
            data[i-1] = arr.at(i);
        }
    }

    if (i-1 != len) {
        m_errorText = QString("Nie poprawna dlugosc rozkazu %1 != %2").arg(i-1).arg(len);
        m_errorBool = true;
        return false;
    }
    if (i >= arr.size()) {
        m_errorText = QString("Za dlugi rozkaz %1 > 15").arg(i);
        m_errorBool = true;
        return false;
    }
    unsigned short msgcrc = arr.at(i) & 0xff;

    if (crc.getCRC() != msgcrc) {
        m_errorText = QString("Nie zgodne crc");
        m_errorBool = true;
        return false;
    }

    return true;
}



bool SerialMessage::parseCommand(const QByteArray &arr)
{
    uint8_t cmd;
    uint8_t len;
    QByteArray data;
    m_errorBool = false;
    m_parseReply = INVALID_REPLY;
    if (!checkHead(arr, cmd, len, data)) {
        return false;
    }

    switch (cmd) {
        case NOP_REP:
            return true;

        case WELCOME_REP:
        {
            if (len != 15) {
                m_errorText = QString("Nie poprawna dlugosc wiadomosci powitalnej %d").arg(len);
                m_errorBool = true;
                return false;
            }
            uint8_t wzorzec[15] = {'K','O','N','T','R','O','L','E','R','W','I','A','T','R', '2'};
            for (int i = 0; i < 15; ++i) {
                if (wzorzec[i] != data[i]) {
                    m_errorText = QString("Nie poprawna wzorzec odpdowiedzi");
                    m_errorBool = true;
                    return false;
                }
            }

            m_parseReply = WELCOME_REPLY;
            return true;
        }

        case SET_PARAM_REP:
        {
            if (data[0] == (char)1) {
                m_parseReply = SETPARAMS1_REPLY;
                return true;
            }
            if (data[0] == (char)2) {
                m_parseReply = SETPARAMS2_REPLY;
                return true;
            }
            m_errorText = QString("Nie poprawna odpowiedz dla ustawienia parametru");
            m_errorBool = true;
            
            return false;
        }
        
        case MOVEHOME_REP:
        {    
            if (len == 1) {
                return reply1Byte(MOVEHOME_REPLY, data[0]);
            } else if (len == 5) {
                switch(data[0]) {
                case 'P':
                    moveStepX = getNumber(data.mid(1, 4));
                    m_parseReply = MOVEHOME_REPLY;
                    posWork = END_X;
                    return true;
                case 'G':
                    moveStepY = getNumber(data.mid(1, 4));
                    m_parseReply = MOVEHOME_REPLY;
                    posWork = END_Y;
                    return true;
                case 'R':
                    moveStepR = getNumber(data.mid(1, 4));
                    m_parseReply = MOVEHOME_REPLY;
                    posWork = END_R;
                    return true;
                default:
                    m_errorText = QString("Nie poprawna odpowiedz na pozycjonowanie bazowe %1").arg((short)data[0], 1, 16);
                    m_errorBool = true;    
                    return false;
                }
            } else if (len == 2) {
                return reply2Byte(MOVEHOME_REPLY, data[0], data[1]);
            }
            m_errorText = QString("Nie poprawna dligosc odpowiedzi na pozycjonowanie bazowe len = %1").arg(len);
            m_errorBool = true;    
                    
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
                return reply1Byte(POSITION_REPLY, data[0]);
            } else if (len == 9) {
                switch(data[0]) {
                    case 'P':
                        moveStepX = getNumber(data.mid(1, 4));
                        posImpX = getNumber(data.mid(5, 4));
                        m_parseReply = POSITION_REPLY;
                        posWork = END_X;
                        return true;
                    case 'G':
                        moveStepY = getNumber(data.mid(1, 4));
                        posImpY = getNumber(data.mid(5, 4));
                        m_parseReply = POSITION_REPLY;
                        posWork = END_Y;
                        return true;
                    case 'R':
                        moveStepR = getNumber(data.mid(1, 4));
                        posStepR = getNumber(data.mid(5, 4));
                        m_parseReply = POSITION_REPLY;
                        posWork = END_R;
                        return true;
                    default:
                        m_errorText = QString("Nie poprawna odpowiedzi na pozycjonowanie  = %1").arg((short)data[0], 0, 16);
                        m_errorBool = true; 
                        return false;
                }
            }
            m_errorText = QString("Nie poprawna dligosc odpowiedzi na pozycjonowanie len = %1").arg(len);
            m_errorBool = true;    
            return false;
        }
            
        case MEASVALUE_REP:
        {
            //getCzujVal
            //0xa0 CRC8 - req
            //0xbA 'O' X2 X1 Y2 Y1 W2 W1 Z2 Z1 CRC8 - ok, wartosci odczytane z radia
            //0xb1 'E' CRC8 - error polaczenia z radiem
            if (len == 2 && data[0] == 'E') {
                readRadioOK = false;
                m_parseReply = RADIOREAD_REPLY;
                return true;
            } else if (len == 9 && data[0] == 'O') {
                radioVal1 = 0;
                radioVal2 = 0;
                radioVal3 = 0;
                radioVal4 = 0;
                radioVal1 = data[1] << 8 | data[2];
                radioVal2 = data[3] << 8 | data[4];
                radioVal3 = data[5] << 8 | data[6];
                radioVal4 = data[7] << 8 | data[8];
                readRadioOK = true;
                m_parseReply = RADIOREAD_REPLY;
                return true;
            }
            m_errorText = QString("Nieprawidlowe dane komendy przy odczycie z radia");;
            m_errorBool = true;    
            return false;
        }

        
        default:
            m_errorText = QString("Nieznana komenda");
            m_errorBool = true;
            return false;
    }

    return false;
}

bool SerialMessage::reply1Byte(ParseReply rep, uint8_t b)
{
    switch(b) {
    case 's':
        m_parseReply = rep;
        posWork = START_XY;
        return true;
    case 'l':
        m_parseReply = rep;
        posWork = START_X;
        return true;
    case 'd':
        m_parseReply = rep;
        posWork = START_Y;
        return true;
    case 'K':
        m_parseReply = rep;
        posWork = END_XY;
        return true;
    case 'r':
        m_parseReply = rep;
        posWork = START_R;
        return true;
    default:
        m_errorText = QString("Nie poprawna odpowiedz na pozycjonowanie (bazowe) %1").arg(b, 1, 16);
        m_errorBool = true;
        return false;
    }
}

bool SerialMessage::reply2Byte(ParseReply rep, uint8_t b1, uint8_t b2)
{

    if (b1 == 'E' && b2 == 'P') {
        m_parseReply = rep;
        posWork = ERROR_XY;
        return true;
    } else if (b1 == 'E' && b2 == 'R') {
        m_parseReply = rep;
        posWork = ERROR_R;
        return true;
    } else {
        m_errorText = QString("Nie poprawna odpowiedz na pozycjonowanie (bazowe) %1 %2").arg(b1, 1, 16).arg(b2, 1, 16);
        m_errorBool = true;    
        return false;                
    }
}

void SerialMessage::setInvalidReply()
{
    m_parseReply = INVALID_REPLY;
}

void SerialMessage::setInProgressReply()
{
    m_parseReply = INPROGRESS_REPLY;
}

void SerialMessage::setTimeoutReply(bool write)
{
    m_parseReply = write ? TIMEOUT_WRITE_REPLY : TIMEOUT_READ_REPLY;
}

bool SerialMessage::isInvalidReply()
{
    return m_parseReply == INVALID_REPLY;
}

bool SerialMessage::isInProgressReply()
{
    return m_parseReply == INPROGRESS_REPLY;
}

bool SerialMessage::isTimeoutWriteReply()
{
    return m_parseReply == TIMEOUT_WRITE_REPLY;
}

bool SerialMessage::isTimeoutReadReply()
{
    return m_parseReply == TIMEOUT_READ_REPLY;
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
    ret.append(arr);
    return ret;
}

SerialMessage::ParseReply SerialMessage::getParseReply() const
{
    return m_parseReply;
}

void SerialMessage::readRadioValues(int & val1, int & val2, int & val3, int & val4)
{
    val1 = radioVal1;
    val2 = radioVal2;
    val3 = radioVal4;
    val4 = radioVal4;
}

uint32_t SerialMessage::getNumber(const QByteArray &data)
{
    return ((data[0] & 0xff) << 24) +  ((data[1] & 0xff) << 16) + ((data[2] & 0xff) << 8) + (data[3] & 0xff);
}

