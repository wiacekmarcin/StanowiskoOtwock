
#include "masterthread.h"

#include <QSerialPort>
#include <QTime>

MasterThread::MasterThread(QObject *parent) :
    QThread(parent)
{
}


MasterThread::~MasterThread()
{
    m_mutex.lock();
    m_quit = true;
    m_cond.wakeOne();
    m_mutex.unlock();
    wait();
}

void MasterThread::transaction(const QString &portName, int waitTimeout, const QByteArray &request)
{

    const QMutexLocker locker(&m_mutex);
    m_portName = portName;
    m_waitTimeout = waitTimeout;
    m_request = request;

    if (!isRunning())
        start();
    else
        m_cond.wakeOne();
}

void MasterThread::run()
{
    bool currentPortNameChanged = false;

    m_mutex.lock();

    QString currentPortName;
    if (currentPortName != m_portName) {
        currentPortName = m_portName;
        currentPortNameChanged = true;
    }

    int currentWaitTimeout = m_waitTimeout;
    QString currentRequest = m_request;
    m_mutex.unlock();

    QSerialPort serial;

    if (currentPortName.isEmpty()) {
        emit error(tr("No port name specified"));
        return;
    }

    while (!m_quit) {

        if (currentPortNameChanged) {
            serial.close();
            serial.setPortName(currentPortName);

            if (!serial.open(QIODevice::ReadWrite)) {
                emit error(tr("Can't open %1, error code %2")
                           .arg(m_portName).arg(serial.error()));
                return;
            }

            serial.setBaudRate(QSerialPort::Baud9600, QSerialPort::AllDirections);
            //serial->setBaudRate(QSerialPort::Baud115200, QSerialPort::AllDirections);
            serial.setDataBits(QSerialPort::Data8);
            serial.setFlowControl(QSerialPort::NoFlowControl);
            serial.setParity(QSerialPort::NoParity);
            serial.setStopBits(QSerialPort::OneStop);
        }

        // write request
        const QByteArray requestData = currentRequest.toUtf8();
        serial.write(requestData);
        if (serial.waitForBytesWritten(1000)) {

            // read response
            if (serial.waitForReadyRead(m_waitTimeout)) {
                QByteArray responseData = serial.readAll();
                while (serial.waitForReadyRead(10))
                    responseData += serial.readAll();

                //const QString response = QString::fromUtf8(responseData);

                emit this->response(responseData);

            } else {
                emit timeout(tr("Wait read response timeout %1")
                             .arg(QTime::currentTime().toString()));
            }

        } else {
            emit timeout(tr("Wait write request timeout %1")
                         .arg(QTime::currentTime().toString()));
        }

        m_mutex.lock();
        m_cond.wait(&m_mutex);
        if (currentPortName != m_portName) {
            currentPortName = m_portName;
            currentPortNameChanged = true;
        } else {
            currentPortNameChanged = false;
        }
        currentWaitTimeout = m_waitTimeout;
        currentRequest = m_request;
        m_mutex.unlock();
    }
}
