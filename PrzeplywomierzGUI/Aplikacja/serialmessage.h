#ifndef SERIALMESSAGE_H
#define SERIALMESSAGE_H

#include <QObject>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QTimer>

struct SerialMessageValues {
    uint32_t moveStepR;
    uint32_t moveStepX;
    uint32_t moveStepY;
    uint32_t posImpX;
    uint32_t posImpY;
    uint32_t posStepR;
};

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
    const SerialMessageValues& getValues() const;

protected:

    bool checkHead(const QByteArray &arr, uint8_t & cmd, uint8_t & len, QByteArray & data);
    static QByteArray prepareMessage(uint8_t cmd, uint8_t tab[], uint8_t len);
    static uint32_t getNumber(const QByteArray &data);

    bool reply1Byte(ParseReply rep, uint8_t b);
    bool reply2Byte(ParseReply rep, uint8_t b1, uint8_t b2);

private:
    QString m_errorText;
    bool m_errorBool;

    SerialMessageValues v;

    ParseReply m_parseReply;
    StatusWork posWork;
    bool readRadioOK;

    unsigned int radioVal1;
    unsigned int radioVal2;
    unsigned int radioVal3;
    unsigned int radioVal4;
};

#endif // SERIALMESSAGE_H

