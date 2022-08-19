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

    virtual void positionDone(bool home);
    virtual void setStop();
    virtual void setError();

protected:
    bool sprawdz();
    void addStatus(const QString & log);
protected slots:
    void pbUstaw_clicked();
    void pbZeruj_clicked();
private:
    Ui::Wentylator *ui;
    uint32_t impx, impy;

};

#endif // WENTYLATOR_H
