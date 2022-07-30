#include "miernikprzeplywu.h"
#include "wybormetody.h"
#include <QApplication>
#include <QDebug>

#include <QCoreApplication>
#include <QSerialPortInfo>
#include <QTextStream>
#include <QTranslator>
#include <QLibraryInfo>

#include "ustawienia.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WyborMetody m;
    Ustawienia u;
    u.read();

    QTranslator qtTranslator;
    if (qtTranslator.load(QLocale::system(),
                "qt", "_",
                QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
    {
        ///qDebug() << "qtTranslator ok";
        a.installTranslator(&qtTranslator);
    }

    QTranslator qtBaseTranslator;
    if (qtBaseTranslator.load("qtbase_" + QLocale::system().name(),
                QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
    {
        //qDebug() << "qtBaseTranslator ok";
        a.installTranslator(&qtBaseTranslator);
    }


    int r = m.exec();
    //qDebug() << r;
    if (r == 1) {
        MiernikPrzeplywu w(NULL, m.getWbMode(), m.getWbMetoda(), m.getFileName(),
                            m.getNumberPozMan(), m.getTimeStopManual(),
                           m.getNumberWidth(), m.getNumberHeight(), m.getTimeStopAuto() /*,
                           m.getPartRolet(), m.getStableTimeCzas()*/);
        w.setUstawienia(&u);
        w.show();
        return a.exec();
    }
}
