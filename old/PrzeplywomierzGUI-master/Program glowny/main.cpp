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
    int ar = 0;
    do {

        WyborMetody m;
        Ustawienia u;
        u.read();


        QTranslator qtTranslator;
        if (qtTranslator.load(QLocale::system(),
                    "qt", "_",
                    QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        {
            //\\QDebug() << "qtTranslator ok";
            a.installTranslator(&qtTranslator);
        }

        QTranslator qtBaseTranslator;
        if (qtBaseTranslator.load("qtbase_" + QLocale::system().name(),
                    QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        {
            //\\QDebug() << "qtBaseTranslator ok";
            a.installTranslator(&qtBaseTranslator);
        }


        int r = m.exec();
        //\\QDebug() << r;
        if (r == 1) {
            MiernikPrzeplywu w(NULL, m.getWbMode(), m.getWbMetoda(), m.getFileName(),
                                m.getNumberPozMan(), m.getTimeStopManual(),
                               m.getNumberWidth(), m.getNumberHeight(), m.getTimeStopAuto());
            w.setUstawienia(&u);
            w.show();
            ar = a.exec();
            qDebug("ar=%d", ar);
        } else {
            ar = 0;
        }
    } while (ar != 0);
    return 0;
}
