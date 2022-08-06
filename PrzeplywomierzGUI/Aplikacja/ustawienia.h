#ifndef USTAWIENIA_H
#define USTAWIENIA_H

#include <QString>
#include <QSettings>

Q_DECLARE_METATYPE(long double)

class Ustawienia
{
public:
    Ustawienia();

    void read();

    void write();



    QString getRatioCzujnik1() const;
    void setRatioCzujnik1(QString value);

    QString getUnitCzujnik1() const;
    void setUnitCzujnik1(const QString &value);


    QString getImpulsyXperMM() const;
    void setImpulsyXperMM(const QString &value);

    QString getImpulsyYperMM() const;
    void setImpulsyYperMM(const QString &value);

    QString getKrokiXperMM() const;
    void setKrokiXperMM(const QString &value);

    QString getKrokiYperMM() const;
    void setKrokiYperMM(const QString &value);


    QString getKatnachylenia() const;
    void setKatnachylenia(const QString &value);

    QString getOffsetX() const;
    void setOffsetX(const QString &value);

    QString getOffsetY() const;
    void setOffsetY(const QString &value);

private:
    QSettings settings;

    static char appnazwa[];
    static char firmnazwa[];

    QString ratioCzujnik1;


    QString unitCzujnik1;

    QString impulsyXperMM;
    QString impulsyYperMM;
    QString krokiXperMM;
    QString krokiYperMM;

    QString katnachylenia;
    QString offsetX;
    QString offsetY;

};

#endif // USTAWIENIA_H
