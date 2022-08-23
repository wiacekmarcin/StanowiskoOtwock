#include <Arduino.h>
#include "../../kontroler_lib/crc8.h"
#define MAXLENPROTO 17

// |      HEAD       | n1 | n2 | .... | n15 | CRC8
// | CMD 4b | LEN 4b |
// LEN(MSG) = 1 (HEAD) + LEN(DATA) + 1 (CRC8)
 /* NOP_REP    0x00
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


class MessageSerialBase {
public:
    MessageSerialBase();

    typedef enum _work {
        NOP,
        POS_START,
        POS_X,
        POS_Y,
        POS_DONE,
        RETURN_HOME,
        RETURN_DONE,
        ROL_START,
        ROL_HOME,
        ROL_DONE,
    } Work;

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
        RESET_STER_REQ = 13,
        RESET_STER_REP = 14,
        ERROR_REP = 15,
    } CMD;


    void sendError(const char * errorStr);
    void init();
    void sendMessage(uint8_t, uint8_t*,uint8_t) {}
};
    
