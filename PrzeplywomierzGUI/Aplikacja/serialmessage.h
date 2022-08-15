#ifndef SERIALMESSAGE_H
#define SERIALMESSAGE_H

#include <QObject>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QTimer>

#if 0
class SerialMessage : public QObject
{

    Q_OBJECT
public:

    explicit SerialMessage(QObject *parent = nullptr);

    ~SerialMessage();
    void closeDevice();

    typedef enum _cmd {
        NOP_MSG = 0,
        WELCOME_REQ = 1,
        WELCOME_REP = 2,
        SET_PARAM_REQ = 3,
        SET_PARAM_REP = 4,
        POSITION_REQ = 5,
        POSITION_REP = 6,
        MOVEHOME_REQ = 7,
        MOVEHOME_REP = 8,
        MEASVALUE_REQ = 9,
        MEASVALUE_REP = 10,
        MEASUNIT_REQ = 11,
        MEASUNIT_REP = 12,
        ERROR_REP = 15,

    } CMD;

    typedef enum _status {
        START_XY,
        START_X,
        END_X,
        START_Y,
        END_Y,
        END_XY,
        START_R,
        END_R,
        ERROR_XY,
        ERROR_R
    } StatusWork;

    int32_t getMoveStepR() const;
    int32_t getMoveStepX() const;
    int32_t getMoveStepY() const;
    int32_t getPosImpX() const;
    int32_t getPosImpY() const;
    int32_t getPosStepR() const;

    void setMechParams(bool reverseX, bool reverseY, bool reverseR,
                                  uint32_t maxImpX, uint32_t maxImpY,
                                  uint32_t maxStepX, uint32_t maxStepY,
                                  uint32_t maxStepR);


signals:
    void successOpenDevice(bool);
    void deviceName(QString);

    void controllerOK();

    void positionStatus(SerialMessage::StatusWork work);
    void homeStatus(SerialMessage::StatusWork work);

    void errorHome();
    void errorHomeRoleta();

    void errorReadFromRadio();
    void readFromRadio(int val);


    void setParamsDone();

    void errorSerial(QString);
    //void timeoutSerial(QString, bool, QString);

    void debug(QString);

public slots:
    void handleReadyRead();
    void serialError(const QSerialPort::SerialPortError & error);

    void connectToSerial();
    void setPositionHome();
    void setPosition(uint32_t x, uint32_t y);

    void setRoletaHome();
    void setRoleta(uint32_t x);

    void setParams(bool reverseX, bool reverseY, bool reverseR,
                   uint32_t maxImpX, uint32_t maxImpY,
                   uint32_t maxStepX, uint32_t maxStepY,
                   uint32_t maxStepR);


    void readRadio();

protected:
    void setSettings1(bool reverseX, bool reverseY, bool reverseR, uint32_t maxImpX, uint32_t maxImpY);
    void setSettings2(uint32_t stepMaxX, uint32_t stepMaxY, uint32_t stepMaxR);

    void response(const QByteArray &s);
    //void errorRead(const QString &s);
    //void timeoutRead(const QString &s);
    //void errorWrite(const QString &s);
    //void timeoutWrite(const QString &s);
    //void findPort(const QString &s);

public:
    bool openDevice(const QSerialPortInfo & port);

    static QByteArray welcomeMsg();
    QByteArray homePositionMsg();
    QByteArray positionMsg(uint32_t x, const uint32_t y);
    QByteArray homeRoletaMsg();
    QByteArray roletaMsg(uint32_t r);
    QByteArray measValuesMsg();
    QByteArray measUnitMsg(short index, const float &ratio, QString &unit);
    bool checkHead(const QByteArray &arr, uint8_t & cmd, uint8_t & len, QByteArray & data);
    bool parseCommand(const QByteArray &arr);
    static QByteArray prepareMessage(uint8_t cmd, uint8_t tab[], uint8_t len);

    void writeMessage(const QByteArray &writeData, int currentWaitWriteTimeout=360000, int currentReadWaitTimeout=360000);


    static QByteArray settings1Msg(bool reverseX, bool reverseY, bool reverseR, uint32_t maxImpX, uint32_t maxImpY);
    static QByteArray settings2Msg(uint32_t maxStepX, uint32_t maxStepY, uint32_t maxStepR);

    uint32_t getNumber(const QByteArray & data);

private:
    //MasterThread mt;
    QSerialPort m_serialPort;
    QString portName;
    bool connSerial;

    QList<QByteArray> commands;
    QByteArray cmd;
    unsigned short lenCmd;

    bool memoryreverseX;
    bool memoryreverseY;
    bool memoryreverseR;
    uint32_t memorymaxImpX;
    uint32_t memorymaxImpY;

    uint32_t memoryStepX;
    uint32_t memoryStepY;
    uint32_t memoryStepR;

    uint32_t moveStepR;
    uint32_t moveStepX;
    uint32_t moveStepY;

    uint32_t posImpX;
    uint32_t posImpY;
    uint32_t posStepR;

    short actPosC2C;


    QString m_portName;
    int m_portNr;
    bool m_connected;
    bool m_configured;

};
#endif


/**
 * @brief The SerialMessage class
 * Klasa reprezuntująca wiadomość wysyłaną do sterownika dozowników
 * NOP_REQ i NOP_REP - pusta wiadomość, WELCOME_REQ i WELCOME_REP -
 * wiadomość powitalna, SET_PARAM_REQ i SET_PARAM_REP - ustawianie
 * parametrów, POSITION_REQ i POSITION_REP - żądanie ustawienia
 * pozycji, ECHO_REQ i ECHO_REP - wiadomość echo, MOVEHOME_REQ i
 * MOVEHOME_REP - ustawianie pozycji bazowej, RESET_REQ i RESET_REP
 * - reset sterownika silnika, ERROR_REP - bład
 *
 * @param m_errorText - info o błedzie
 * @param m_errorBool - czy był bład parsowania
 * @param m_steps - ilość kroków
 * @param m_homePosition - status pozycji bazowych dla wszystkich dozowników
 * @param m_parseReply - odpowiedż po sparsowaniu
 */
class SerialMessage
{

public:

    explicit SerialMessage();

    ~SerialMessage();

    /**
     * Typy wiadomości
     */
    typedef enum _cmd {
        NOP_REP = 0,
        WELCOME_REQ = 1,
        WELCOME_REP = 2,
        SET_PARAM_REQ = 3,
        SET_PARAM_REP = 4,
        POSITION_REQ = 5,
        POSITION_REP = 6,
        MOVEHOME_REQ = 7,
        MOVEHOME_REP = 8,
        MEASVALUE_REQ = 9,
        MEASVALUE_REP = 10,
        MEASUNIT_REQ = 11,
        MEASUNIT_REP = 12,
        ERROR_REP = 15,

    } CMD;

    typedef enum _status {
        START_XY,
        START_X,
        END_X,
        START_Y,
        END_Y,
        END_XY,
        START_R,
        END_R,
        ERROR_XY,
        ERROR_R
    } StatusWork;



    /**
     * Możliwe statusy odpowiedzi
     */
    typedef enum _action {
        INVALID_REPLY = -4,
        TIMEOUT_WRITE_REPLY = -3,
        TIMEOUT_READ_REPLY = -2,
        INPROGRESS_REPLY = 0,
        WELCOME_REPLY = 1,
        MOVEHOME_REPLY,
        POSITION_REPLY,
        RESET_REPLY,
        SETPARAMS1_REPLY,
        SETPARAMS2_REPLY,
        RADIOREAD_REPLY,
    } ParseReply;

    /**
     * @brief setSettingsMsg - wiadomość ustawiająca parametry
     * @param reverseX - kierunek silnika 1
     * @param reverseY - kierunek silnika 2
     * @param reverseR - kierunek silnika 3
     * @param maxImpX  - maksymalna ilosc impulsow
     * @param maxImpY  - maksymalna ilosc impulsow
     * @param maxStepX - maksymalna ilość krokow
     * @param maxStepY - maksymalna ilosc krokow
     * @param maxStepR - maksymalna ilosc krokow
     * @return bajty reprezentujące wiadomość
     */
    static QByteArray settings1Msg(bool reverseX, bool reverseY, bool reverseR, uint32_t maxImpX, uint32_t maxImpY);
    static QByteArray settings2Msg(uint32_t maxStepX, uint32_t maxStepY, uint32_t maxStepR);

    /**
     * @brief welcomeMsg - wiadomość powitalna
     * @return bajty reprezentujące wiadomość
     */
    static QByteArray welcomeMsg();

    /**
     * @brief setPositionHome - wiadomość do ustawienia pozycji bazowej
     * @param roleta - false czujnik, true - roleta
     * @return bajty reprezentujące wiadomość
     */
    static QByteArray setPositionHome(bool roleta=false);
    static QByteArray setRoletaHome() { return setPositionHome(true); };

    /**
     * @brief setPosition - wiadomość do ustawienia konkretnej pozycji
     * @param impX - ilość impulsow
     * @param impY - ilość impulsow
     * @return bajty reprezentujące wiadomość
     */
    static QByteArray setPosition(uint32_t x, uint32_t y);

    /**
     * @brief setRoleta - wiadomość do ustawienia konkretnej pozycji rolety
     * @param r - ilość krokow
     * @return bajty reprezentujące wiadomość
     */
    static QByteArray setRoleta(uint32_t r);
    /*************************************************************/
    
    /** 
     * @brief measValueMsg - wiadomosc do pobrania wartosci z radia 
     * @return bajty reprezentujące wiadomość
     */
    static QByteArray measValuesMsg();
    
    /**
     * @brief measUnitMsg - konfiguruje wyswietalny tekst na czujniku
     * 
     * @param index - index od 0 do 4
     * @param ratio - wspolczyniik wolty na jednostke
     * @param unit - jednostka wyswietlana
     * @return QByteArray 
     */
    static QByteArray measUnitMsg(short index, const float &ratio, QString &unit);

    /**
     * @brief getParseReply zwraca typ sparsowanej wiadomości
     * @return
     */
    ParseReply getParseReply() const;

    /**
     * @brief parseCommand - parsuje komendę
     * @param arr - bajty do sparsowania
     * @return - wynik czy poprawna instrukcja
     */
    bool parseCommand(const QByteArray &arr);

    /**
     * @brief setInvalidReply - ustawia status niepoprawna wiadomosc
     * @brief setInProgressReply - ustawia wiadomosc w trakcie realizacji
     * @brief setTimeoutReply - ustawia wiadomosc w stanie timeout
     * @param write - write/read Timeout
     **/
    void setInvalidReply();
    void setInProgressReply();
    void setTimeoutReply(bool write);
    
    /**
     * @brief isInvalidReply
     * @brief isInProgressReply
     * @brief isTimeoutWriteReply;
     * @brief isTimeoutReadReply;
     */
    bool isInvalidReply();
    bool isInProgressReply();
    bool isTimeoutWriteReply();
    bool isTimeoutReadReply();


    StatusWork getPosWork() { return posWork; }

    void readRadioValues(int & val1, int & val2, int & val3, int & val4);
protected:

    bool checkHead(const QByteArray &arr, uint8_t & cmd, uint8_t & len, QByteArray & data);
    static QByteArray prepareMessage(uint8_t cmd, uint8_t tab[], uint8_t len);
    static uint32_t getNumber(const QByteArray &data);

    bool reply1Byte(ParseReply rep, uint8_t b);
    bool reply2Byte(ParseReply rep, uint8_t b1, uint8_t b2);

private:
    QString m_errorText;
    bool m_errorBool;

    uint32_t moveStepR;
    uint32_t moveStepX;
    uint32_t moveStepY;

    uint32_t posImpX;
    uint32_t posImpY;
    uint32_t posStepR;

    ParseReply m_parseReply;

    StatusWork posWork;

    bool readRadioOK;
    unsigned int radioVal1;
    unsigned int radioVal2;
    unsigned int radioVal3;
    unsigned int radioVal4;
};

#endif // SERIALMESSAGE_H

