#ifndef SERIALMESSAGE_H
#define SERIALMESSAGE_H

#include <QObject>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QTimer>

/***
 * NOP_REP    0x00
 * NOP_REQ    0x0f
 * 
 *      CMD+LEN D1     D2      D3      D4      D5      D6      D7      D8      D9      D10     D11     D12     D13     D14     D15    CRC8
 *
 * WELCOME_REQ   - sprawdzenie zy odowiedni kontroler
 *      0x10    0x70
 *
 * WELCOME_REP   
 *      0x2F    0x4B    0x4F    0x4E    0x54    0x52    0x4F    0x4C    0x45    0x52    0x57    0x49    0x41    0x54    0x52    0x32    0xDC
 *              K       O       M       T       R       O       L       E       R       W       I       A       T       R       2   
 *     
 * SET_PARAM_REQ  - ustawienie parametrow (maxymalna ilosc impulsow i korkow na osiach X i Y, kierunek obrotow silnikow i max krokow rolety)
 *      0x3E    0x01    REVB    MAXIX4  MAXIX3  MAXIX2  MAXIX1  MAXIY4  MAXSY3  MAXIY2  MAXIY1  RHSPD2  RHSPD1  RPSPD2  RPSPD1  CRC
 *      0x3F    0x02    MAXSX4  MAXSX3  MAXSX2  MAXSX1  MAXSY4  MAXSY3  MAXSY2  MAXSY1  MAXSR4  MAXSR3  MAXSR2  MAXSR1  MINSR1  MINSR2  CRC 
 * 
 * SET_PARAM_REP
 *      0x41    0x01    0x49       
 *      0x42    0x02    0x40
 *         
 * POSITION_REQ - zadanie ustawienia silnika P 
 *      0x59    0x50    IMPY4   IMPY3   IMPY2   IMPY1   IMPX4   IMPX3    IMPX2  IMPX1   CRC
 *              P  
 *      0x55    0x52    STEPR4  STEPR3  STEPR2  STEPR1  CRC
 *              R
 * 
 * POSITION_REP
 *      0x61    0x73    0xBE
 *              s   
 *      0x61    0x64    0xDB
 *              d           
 *      0x69    0x47    STEPY4   STEPY3   STEPY2   STEPY1   POSIY4  POSIY3  POSIY2  POSIY1  CRC
 *              G
 *      0x61    0x6C    0xE3
 *              l
 *      0x69    0x50    STEPX4   STEPX3   STEPX2   STEPX1   POSIX4  POSIX3  POSIX2  POSIX1  CRC
 *              P    
 *      0x61    0x4B    0x16
 *              K
 * 
 *      0x61    0x72    0xB9
 *              r
 *      0x69    0x52    STEPR4   STEPR3   STEPR2   STEPR1   POSSR4  POSSR3  POSSR2  POSSR1  CRC
 *              R
 *
 *      0x62    0x45    0x50    0xBE
 *              E       P
 *      0x62    0x45    0x52    0xB0
 *              E       R
 *
  * MOVEHOME_REQ zerowanie czujnika (P) , rolety(R), oraz koncowe zerowanie czujnika (p) i rolety (r) - bez komunikatow
 *       0x71   0x50    0x00 
 *              P
 *       0x71   0x52    0x0E
 *              R
 *       0x71   0x70    0xE0
 *              p
 *       0x71   0x72    0xEE
 *              r
 * 
 * MOVEHOME_REP
 *      0x81    0x73    0xFD - start
 *              s 
 *      0x81    0x6C    0xA0 - start X
 *              l
 *      0x85    0x50    STEPX4   STEPX3   STEPX2   STEPX1   CRC - end X
 *              P 
 *      0x81    0x64    0x98 - start Y
 *              d
 *      0x85    0x47    STEPY4   STEPY3   STEPY2   STEPY1   CRC - end Y
 *              G  
 *      0x81    0x4B    0x55 - end
 *              K
 *
 *      0x81    0x72    0xFA - roleta start
 *              r
 *      0x85    0x52    STEPR4  STEPR3  STEPR2  STEPR1  CRC - end R
 *              R
 * 
 *      0x82    0x45    0x50    0x70
 *              E       P
 *      0x82    0x45    0x52    0x7E
 *              E       R
 *
 * MEASVALUE_REQ
 *      0x90    0xF9
 *
 * MEASVALUE_REP
 *      0xA9    0x4F    VAL1_16 VAL1_8  VAL2_16 VAL2_8  VAL3_16 VAL3_8  VAL4_16 VAL4_8  CRC
 *              O
 *      0xA1    0x45    ERR     CRC
 *              E         
 *
 * MEASUNIT_REQ
 * MEASUNIT_REP
 *
 * RESET_STER_REQ
 *      0xC0    0x4E
 * RESET_STER_REP
 *      0xD0             
 * ERROR_REP 
 *      0xF?   TXT1 ... TXT15   CRC     
 */

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
        RESET_STER_REQ = 13,
        RESET_STER_REP = 14,
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
     * @param minStepR - ilość kroktów od wykrycia bazy do złożenia rolety
     * @param speedRolHome - predkosc odpuszczania rolety
     * @param speedRolPos - predkosc podnoszenia rolety
     * @return bajty reprezentujące wiadomość
     */
    static QByteArray settings1Msg(bool reverseX, bool reverseY, bool reverseR, uint32_t maxImpX, uint32_t maxImpY,  
                uint16_t speedRolHome, uint16_t speedRolPos);
    static QByteArray settings2Msg(uint32_t maxStepX, uint32_t maxStepY, uint32_t maxStepR, uint16_t minStepR);

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

    static QByteArray resetSterownik();
    

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

    unsigned int m_speedRolHome;
    unsigned int m_speedRolPos;
    unsigned int m_minStepR;

};

#endif // SERIALMESSAGE_H

