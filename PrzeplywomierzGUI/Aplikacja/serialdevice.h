#ifndef SERIALDEVICE_H
#define SERIALDEVICE_H

#include <QThread>
#include <QMutex>
#include <QThread>
#include <QWaitCondition>

#include <QByteArray>

#include "serialmessage.h"
#include "ustawienia.h"

#define DEBUG(X) emit debug(QString("%1:%2 %3").arg(__FILE__).arg(__LINE__).arg(X))

class SerialDevice;
class QThread;
/**
 * @brief The SerialWorker class
 * Klasa wątku w którym chodzi sterowanie dozownikime
 * Normalnie wątek jest zawieszony do czasu podania i
 * intrukcji za pomocą funkcji command.
 * @param actTask - aktualne wykonywane zadanie, na IDLE - wątek "wisi"
 * @param mutex - ustawianie zadania chronione mutexem
 * @param newTask - zmienna informujaca, że jest nowe zadanie
 * @param sd - pointer to device
 */
class SerialWorker : public QThread
{
    Q_OBJECT
public:
    /**
     * Zadania,
     * IDLE -nic nierób, CONNECT - staraj się połączyć z urządzeniem, CONFIGURE - skonfiguruj urządzenie,
     * RESET - resetuj sterownik silników, SET_PARAMS - ustaw parametry sterownika silników,
     * SET_POSITION - ustaw konkretną pozycję dozownika, SET_HOME - ustaw dozownik w pozycji bazowej,
     * SET_ROLETA - ustaw rolete, SET_ROLETA_HOME - zjedz roleta w dol
     */
    typedef enum _task {
        IDLE,
        CONNECT,
        CONFIGURE,
        SET_PARAMS,
        SET_POSITION,
        SET_HOME,
        SET_ROLETA,
        SET_ROLETA_HOME,
        READ_RADIO,
        
    } Task;

    explicit SerialWorker(SerialDevice * device);
    ~SerialWorker();

    /**
     * @brief command
     * Ustawienie nowego zadania
     * @param task - nowe zadanie IDLE, CONNECT, CONFIGURE, RESET, SET_PARAMS, SET_POSITION, SET_HOME, SET_CYCLE,
     * SET_STEPS, SET_ECHO2
     */
    void command(Task task);

    /**
     * @brief setStop
     * Zatrzymanie pracy workera
     */
    void setStop();


    Task getActTask();

signals:
    void debug(QString);

protected:
    /**
     * @brief run
     * Fukcja workera
     */
    void run();


private:
    Task actTask;
    QMutex mutex;
    QWaitCondition newTask;
    bool runWorker;
    SerialDevice * sd;
};


/**
 * @brief The SerialDevice class
 * Klasa urzadzenia szeregowego, jest odpowiedzialna za sterowanie dozownikiem
 * @param m_portName - nazwa portu szeregowego
 * @param m_portNr - numer portu szeregowego na windowsie np 7 (czyli COM7)
 * @param m_connected - czy urządzenie jest w systemie
 * @param m_configured - czy urządzenie jest skonfigurowane
 * @param m_reverse - konfiguracja obrotów silnika
 * @param m_maxImp - maksymalna ilość impulsów
 * @param m_maxStep - maksymalna ilosc krokow (R - rolety)
 * @param m_worker - obiekt który realizuje komunikację w wątku
 */
class SerialDevice : public QObject
{
    Q_OBJECT
public:
    explicit SerialDevice(QObject *parent = nullptr);
    ~SerialDevice();

    /**
     * @brief setThreads
     * Ustawia wątek do obsługi procesu
     * @param trh
     */
    void setThread(QThread * trh);

    /**
     * @brief setStop
     * Zatrzymanie pracy workera
     */
    void setStop();


    /************ Funkcje sterujące *************************************/
    /**
     * @brief closeDevice zamyka urzadzenia
     * @param afterBase - czy spozycjonowaniu na zero
     */
    void closeDevice(bool afterBase);

    /**
     * @brief connectToDevice ustanawia polaczenie z urzadzeniem
     */
    void connectToDevice();

    /**
    * @brief setParams zapamietuje parametry, ktore zostana wpisane do kontrolera w momencie otwarcia
    * @param reverseX czy silnik X chodzi w odwrotnym kierunku
    * @param reverseY czy silnik Y chodzi w odwrotnym kierunku
    * @param reverseR czy silnik R chodzi w odwrotnym kierunku
    * @param maxImpX maksymalna ilosc impulsow dla silnika X
    * @param maxImpY maksymalna ilosc impulsow dla silnika Y
    * @param maxStepX maksymalna ilosc krokow dla silnika X
    * @param maxStepY maksymalna ilosc krokow dla silnika Y
    * @param maxStepR maksymalna ilosc krokow dla silnika R
    */
    void setParams(bool reverseX, bool reverseY, bool reverseR,
                   uint32_t maxImpX, uint32_t maxImpY,
                   uint32_t maxStepX, uint32_t maxStepY,
                   uint32_t maxStepR);


    /**
     * @brief setPositionHome - ustawia pozycje bazową dla dozownika
    */
    void setPositionHome();

    /**
     * @brief setSteps - ustawia ilość kroków dla przesunięcia
     * @param x - ilosc przesuniecia w impulsach os X
     * @param y - ilosc przesuniecia w impulsach os Y
     */
    void setPosition(uint32_t x, uint32_t y);

    /**
     * @brief setPositionHome - ustawia pozycje bazową dla dozownika
    */
    void setRoletaHome();

    /**
     * @brief setSteps - ustawia ilość kroków dla przesunięcia
     * @param r - ilosc przesuniecia w impulsach os X
     */
    void setRoleta(uint32_t r);

    /**
     * @brief readRadio
     */
    void readRadio();

    void insertParams();

signals:
    /**
     * @brief error wystąpił bład
     * @param s info o błędzie
     */
    void error(const QString &s);

    /**
     * @brief debug Informacje debugowe
     * @param d
     */
    void debug(const QString &d);

    /**
     * @brief setParamsDone - ustawianie parametrów zakończone
     * @param success - true powodzenie, false niepowodzenie
     */
    void setParamsDone(bool success);

    /**
     * @brief kontrolerConfigured - znaleziono sterownik i skonfigurowano
     * @param conn - czy połączone
     * @param errConn - czy wystąpily błedy podczas otwierania portu
     * @param conf - skonfigurowano
     * @param confErr - czy wystąpiły błedy podczas konfiguracji
     * @param sett - skonfigurowano
     * @param settErr - czy wystąpiły błedy podczas konfiguracji

     */
    void kontrolerConfigured(bool conn, bool errConn, bool conf, bool confErr, bool sett, bool settErr);

    /**
     * @brief zwraca nazwe urzadzenia
     * @param name nazwa urzadzenia
     */
    void deviceName(const QString & name);

    /**
     * @brief setPositionDone - ustawianie pozycji zakończone
     * @param home - czy powrot do bazy true, czy ustawianie pozycji
     * @param success - czy poprawny komunikat
     * @param work - aktualna wykonywana praca
     */
    void setPositionDone(bool home, bool success, int work);

    /**
     * @brief Funkcja ktora odczytuje z radia podane wartosci 
     * @param success czy udalo sie odczytac z radia
     * @param val1 - wartosc dla kanalu pierwszego
     * @param val2 - wartosc dla kanalu drugiego
     * @param val3 - wartosc dla kanalu trzeciego
     * @param val4 - wartosc dla kanalu czwartego
     */
    void readFromRadio(bool success, int val1, int val2, int val3, int val4);

protected:
    /********************************** INNE FUNKCJE *************************/
    /**
     * @brief closeDevice zamyka urzadzenia
     */
    void closeDevice();
    /*********************  JOBY ******************************************/
protected:
    friend class SerialWorker;
    /**
     * @brief connectToSerialJob - wyszukiwanie i otworzenie sterownika
     */
    void connectToSerialJob();

    /**
     * @brief configureDeviceJob - konfiguracja sterownika dozowników
     * @return
     */
    bool configureDeviceJob();

    /**
     * @brief setParamsJob - ustawia paremetry sterownika silnika
     */
    void setParamsJob();

    /**
     * @brief setHomeJob - ustawia pozycję bazową dla wybranego dozownika
     */
    void setHomeJob();

    /**
     * @brief setPosJob - ustawia żądana pozycję
     */
    void setPosJob();

    /**
     * @brief setHomeJob - ustawia pozycję bazową dla wybranego dozownika
     */
    void setRoletaHomeJob();

    /**
     * @brief setPosJob - ustawia żądana pozycję
     */
    void setRoletaJob();

    /**
     * @brief readRadio - pobiera dane z radia
     */
    void readRadioJob();

    /**
     * @brief openDevice - otwiera urzadzenie
     * @return
     */
    bool openDevice();

    /**
     * @brief write - zapisz
     * @param req - dane do zapisu
     * @param currentWaitWriteTimeout
     * @param currentReadWaitTimeout
     * @return
     */
    SerialMessage write(const QByteArray & req, int currentWaitWriteTimeout, int currentReadWaitTimeout);

    /**
     * @brief parseMessage
     * @param reply
     * @return
     */
    SerialMessage parseMessage(const QByteArray & reply);

    /**
     * @brief Wspolna funkcja dla setPos i setHome
     * @param home - czy powrot do bazy
     */
    void setPosJobLocal(bool home);

    /**
     * @brief Wspolna funkcja dla setPos i setHome
     * @param home - czy powrot do bazy
     */
    void setRoletaJobLocal(bool home);
private:
    /* Ustawienia obiektu */
    QString m_portName;
    int m_portNr;
    bool m_connected;
    bool m_configured;
    SerialWorker m_worker;
    bool waitForClose;

    /* Ustawianie silnikow */
    bool m_reverseX, m_reverseY, m_reverseR;
    uint32_t m_maxImpX, m_maxImpY, m_maxStepX, m_maxStepY, m_maxStepR;

    /* Zmienne pomocnicze do komunikacji z sterownikiem */
    uint32_t m_impX, m_impY, m_stepR;
};



#endif // SERIALDEVICE_H
