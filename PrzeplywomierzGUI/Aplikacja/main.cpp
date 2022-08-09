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
    //int ar = 0;
    //WyborMetody::ModeWork mode = WyborMetody::MODE_NONE;
    //WyborMetody::MethodInsData method = WyborMetody::METHOD_NONE;

    //do {
        MiernikPrzeplywu w;
        w.show();
        //ar = w.exec();
        //qDebug() << "exec" << ar;
        //mode = w.getModeWork();
        //method = w.getMethodIns();
    //} while (ar != 0);
    //return ar;

    //w.exec();
    return a.exec();
/*
    do {

        //WyborMetody m(nullptr, mode, method);
        Ustawienia u;
        u.read();


        QTranslator qtTranslator;
        if (qtTranslator.load(QLocale::system(),
                    "qt", "_",
                    QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        {
            //qDebug() << "qtTranslator ok";
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
        qDebug() << "1 r=" << r;
        if (r == 1) {
            qDebug() << "Start app\n";
            MiernikPrzeplywu w(NULL, m.getWbMode(), m.getWbMetoda(), m.getData());
        	w.setUstawienia(&u);
        	w.show();
            ar = w.exec();
            mode = w.getModeWork();
            method = w.getMethodIns();
            qDebug() << "1 ar=" << ar << "\n";
        } else {
            ar = 0;
        }
    } while (ar != 0);
    return ar;
*/
}
