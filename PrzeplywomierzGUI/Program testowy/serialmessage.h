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
        WELCOME_REQ = 1,
        WELCOME_REP = 2,
        SET_PARAM_REQ = 3,
        SET_PARAM_REP = 4,
        POSITION_REQ = 5,
        POSITION_REP = 6,
        MEASVALUE_REQ = 7,
        MEASVALUE_REP = 8,
        MEASUNIT_REQ = 9,
        MEASUNIT_REP = 10,
        MOVEHOME_REQ = 11,
        MOVEHOME_REP = 12,

    } CMD;

signals:
    void successOpenDevice(bool);
    void deviceName(QString);

    void controllerOK();
    void startingPosition();
    void startingPositionX();
    void donePositionX();
    void startingPositionY();
    void donePositionY();
    void donePosition();
    void errorReadFromRadio();
    void readFromRadio(int val);

    void startingHome();
    void startingHomeX();
    void doneHomeX();
    void startingHomeY();
    void doneHomeY();
    void doneHome();
    void errorHome();

    void setParams1();
    void setParams2();

    void setParamsDone();

    void errorSerial(QString);
    //void timeoutSerial(QString, bool, QString);

    void debug(QString);

public slots:
    void handleReadyRead();
    void serialError(const QSerialPort::SerialPortError & error);

    void checkController();
    void connectToSerial();
    void setPositionHome();
    void setSettings1(bool reverseX, bool reverseY, uint32_t maxImpX, uint32_t maxImpY);
    void setSettings2(uint32_t stepImpX, uint32_t stepImpY);
    void doneSettings1();
    void doneSettings2();
    void setPosition(uint32_t x, uint32_t y);

    void setParams(bool reverseX, bool reverseY, uint32_t maxImpX, uint32_t maxImpY, uint32_t maxStepX, uint32_t maxStepY);

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
    QByteArray measValuesMsg();
    QByteArray measUnitMsg(short index, const float &ratio, QString &unit);
    bool checkHead(const QByteArray &arr, uint8_t & cmd, uint8_t & len, QByteArray & data);
    bool parseCommand(const QByteArray &arr);
    QByteArray prepareMessage(uint8_t cmd, uint8_t tab[], uint8_t len);
    void writeMessage(const QByteArray &writeData);
    QByteArray settings1Msg(bool reverseX, bool reverseY, uint32_t maxImpX, uint32_t maxImpY);
    QByteArray settings2Msg(uint32_t maxImpX, uint32_t maxImpY);


private:
    //MasterThread mt;
    QSerialPort m_serialPort;
    QString portName;
    bool connSerial;

    QList<QByteArray> commands;
    QByteArray cmd;
    unsigned short lenCmd;

    int32_t memoryStepX;
    int32_t memoryStepY;

};

#endif // SERIALMESSAGE_H
