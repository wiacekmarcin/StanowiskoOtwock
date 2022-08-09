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

    const QString &getRolObrot1() const;
    void setRolObrot1(const QString &newRolObrot1);

    const QString &getRolObrot2() const;
    void setRolObrot2(const QString &newRolObrot2);

    const QString &getRolObrot3() const;
    void setRolObrot3(const QString &newRolObrot3);

    const QString &getRolObrot4() const;
    void setRolObrot4(const QString &newRolObrot4);

    const QString &getRolObrot5() const;
    void setRolObrot5(const QString &newRolObrot5);

    const QString &getRolObrot6() const;
    void setRolObrot6(const QString &newRolObrot6);

    const QString &getRolObrot7() const;
    void setRolObrot7(const QString &newRolObrot7);

    const QString &getRolObrot8() const;
    void setRolObrot8(const QString &newRolObrot8);

    const QString &getRolObrot9() const;
    void setRolObrot9(const QString &newRolObrot9);

    const QString &getRolObrot10() const;
    void setRolObrot10(const QString &newRolObrot10);

    const QString &getRolObrot11() const;
    void setRolObrot11(const QString &newRolObrot11);

    const QString &getRolObrot12() const;
    void setRolObrot12(const QString &newRolObrot12);

    const QString &getRolObrot13() const;
    void setRolObrot13(const QString &newRolObrot13);

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
    QString rolObrot1;
    QString rolObrot2;
    QString rolObrot3;
    QString rolObrot4;
    QString rolObrot5;
    QString rolObrot6;
    QString rolObrot7;
    QString rolObrot8;
    QString rolObrot9;
    QString rolObrot10;
    QString rolObrot11;
    QString rolObrot12;
    QString rolObrot13;

    QString rolOffsetX;
    QString rolOffsetY;
    QString rolkrokiperMM;
};

#endif // USTAWIENIA_H
