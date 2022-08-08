#ifndef WENTYLATOR_H
#define WENTYLATOR_H

#include "tabwidget.h"

namespace Ui {
class Wentylator;
}

class Wentylator : public TabWidget
{
    Q_OBJECT

public:
    explicit Wentylator(QWidget *parent = nullptr);
    ~Wentylator();
    void setConnect(bool val, const QString &error);
    bool chooseMethod(const WyborMetody::ModeWork & work, const WyborMetody::MethodInsData & data, const WyborMetodyData &values);

    virtual void positionDone(bool home);
    virtual void setStop();
protected:
    void ustaw();
    bool sprawdz();
protected slots:
    void pbUstaw_clicked();
    void pbZeruj_clicked();
private:
    Ui::Wentylator *ui;
    uint32_t impx, impy;

};

#endif // WENTYLATOR_H
