void lcdsetup();

typedef enum _RadioStatus {
    RADIO_OK,
    RADIO_CHECKING,
    RADIO_ERROR,
    RADIO_NOPRESENT,
    RADIO_SENDING,
    RADIO_SENDOK,
    WAITING_DATA
} RadioStatus;

void drawBitmap(void);
void drawSpeed(float value);
void drawmVolts(uint16_t value);
void drawPercent(uint8_t perc);
void drawStatus(RadioStatus st);
void drawNOD1115();
void clearDisplay();
void changeUnit(bool mV);