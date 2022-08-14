#ifndef USTAWIENIA_H
#define USTAWIENIA_H

#include <QObject>
#include <QString>
#include <QSettings>

Q_DECLARE_METATYPE(long double)

class Ustawienia : public QObject
{
    Q_OBJECT

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

    QString getWentOffsetX() const;
    void setWentOffsetX(const QString &value);

    QString getWentOffsetY() const;
    void setWentOffsetY(const QString &value);

    const QString &getRolDlugosc() const;
    void setRolDlugosc(const QString &newRolDlugosc);

    const QString &getRolStepObrot() const;
    void setRolStepObrot(const QString &newRolImpObrot);

    const QString &getRolObrot(unsigned short obr) const;
    void setRolObrot(unsigned short obr, const QString &newRolObrot1);

    QString getRolOffsetX() const;
    void setRolOffsetX(const QString &value);

    QString getRolOffsetY() const;
    void setRolOffsetY(const QString &value);

    const QString &getRolkrokiperMM() const;
    void setRolkrokiperMM(const QString &newRolkrokiperMM);

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
    QString wentOffsetX;
    QString wentOffsetY;

    QString rolDlugosc;
    QString rolStepObrot;
    QString rolObrot[30];

    QString rolOffsetX;
    QString rolOffsetY;
    QString rolkrokiperMM;
};

#endif // USTAWIENIA_H
