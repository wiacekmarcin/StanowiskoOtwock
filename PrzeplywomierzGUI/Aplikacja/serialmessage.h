#ifndef SERIALMESSAGE_H
#define SERIALMESSAGE_H

#include <QObject>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QTimer>

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

protected:
    bool openDevice(const QSerialPortInfo & port);

    QByteArray welcomeMsg();
    QByteArray homePositionMsg();
    QByteArray positionMsg(uint32_t x, const uint32_t y);
    QByteArray homeRoletaMsg();
    QByteArray roletaMsg(uint32_t r);
    QByteArray measValuesMsg();
    QByteArray measUnitMsg(short index, const float &ratio, QString &unit);
    bool checkHead(const QByteArray &arr, uint8_t & cmd, uint8_t & len, QByteArray & data);
    bool parseCommand(const QByteArray &arr);
    QByteArray prepareMessage(uint8_t cmd, uint8_t tab[], uint8_t len);
    void writeMessage(const QByteArray &writeData);
    QByteArray settings1Msg(bool reverseX, bool reverseY, bool reverseR, uint32_t maxImpX, uint32_t maxImpY);
    QByteArray settings2Msg(uint32_t maxStepX, uint32_t maxStepY, uint32_t maxStepR);

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

};

#endif // SERIALMESSAGE_H
