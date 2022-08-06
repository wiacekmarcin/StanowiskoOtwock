#include "wentylator.h"
#include "ui_wentylator.h"

Wentylator::Wentylator(QWidget *parent) :
    TabWidget(parent),
    ui(new Ui::Wentylator)
{
    ui->setupUi(this);
    ui->errorWentylatorX->setVisible(false);
    ui->errorWentylatorY->setVisible(false);
    ui->errorWentylatorL->setVisible(false);
    ui->pbZeruj->setVisible(false);
    ui->statusWentylator->setVisible(false);

    connect(ui->pbUstaw, &QPushButton::clicked, this, &Wentylator::pbUstaw_clicked);
    connect(ui->pbZeruj, &QPushButton::clicked, this, &Wentylator::pbZeruj_clicked);
}

Wentylator::~Wentylator()
{
    delete ui;
}

void Wentylator::setConnect(bool val, const QString & error)
{
    TabWidget::setConnect(val);
    ui->statusWentylator->append(addTime("Nie udało się połączyć z urządzeniem. Błąd : %1").arg(error));
    ui->pbUstaw->setEnabled(true);
    ui->lStatusWiatrak->setText("Błąd");
}

bool Wentylator::chooseMethod(const WyborMetody::ModeWork &mode, const WyborMetody::MethodInsData &, const WyborMetodyData &)
{
    return mode == WyborMetody::MODE_FUNSET;
}

void Wentylator::ustaw()
{
    if(sprawdz()) {
        ui->pbUstaw->setEnabled(false);
        debug(QString("wyliczone wartosci impulsow x=%1 y=%2").arg(impx).arg(impy));

        setPosition(impx, impy);

    }
}

void Wentylator::pbUstaw_clicked()
{
    if (!getConnect()) {
        ui->statusWentylator->clear();
        ui->statusWentylator->append(addTime("Szukam urządzenia."));
        ui->lStatusWiatrak->setText(QString("Szukam urzadzenia ...."));
        debug(QString("Wysyłam żadanie połączenia z urządzeniem"));
        connectToDevice();
    } else {
        ui->statusWentylator->append(addTime("Kalibruje urządzenie."));
        setPositionHome();
    }

}

void Wentylator::pbZeruj_clicked()
{
    if (getConnect()) {
        ui->statusWentylator->append(addTime("Kalibruje urządzenie."));
        setPositionHome();
    } else {
        connectToDevice();
        ui->statusWentylator->append(addTime("Wysyłam konfigurację."));
        ui->lStatusWiatrak->setText("Wysyłam konfiguracje do sterownika...");
    }
}

void Wentylator::positionDone(bool home)
{
    if (home) {
        ustaw();
    } else {
        ui->pbUstaw->setEnabled(true);
        ui->lStatusWiatrak->setText("Pozycja wentylatora ustawiona");
        ui->statusWentylator->append(addTime("Pozycja ustawiona."));
    }
}

bool Wentylator::sprawdz()
{
    QString sx = ui->leX->text();
    QString sy = ui->leY->text();
    QString sl = ui->leL->text();
    debug(QString("Ustawiam pozycje wentylatora X=%1 Y=%2 L=%3").arg(sx).arg(sy).arg(sl));

    int x = 0, y = 0, l = 0;
    bool ret = true;
    int impmmx, impmmy;

    ui->errorWentylatorX->setVisible(false);
    ui->errorWentylatorY->setVisible(false);
    ui->errorWentylatorL->setVisible(false);
    if (sx.isEmpty()) {
        ui->errorWentylatorX->setVisible(true);
        ui->errorWentylatorX->setText(QString::fromUtf8("Wartość nie może być pusta"));
        ret = false;
    } else {
        bool ok;
        x = sx.toInt(&ok);
        if (!ok) {
            ui->errorWentylatorX->setVisible(true);
            ui->errorWentylatorX->setText(QString::fromUtf8("Wartość musi być liczbą"));
            ret = false;
        } else {
            if (x < 0) {
                ui->errorWentylatorX->setVisible(true);
                ui->errorWentylatorX->setText(QString::fromUtf8("Wartość musi być liczbą wiekszą od zera"));
                ret = false;
            } else if (x > 2200) {
                ui->errorWentylatorX->setVisible(true);
                ui->errorWentylatorX->setText(QString::fromUtf8("Wartość musi być liczbą mniejszą 2200"));
                ret = false;
            }
        }
    }

    if (sy.isEmpty()) {
        ui->errorWentylatorY->setVisible(true);
        ui->errorWentylatorY->setText(QString::fromUtf8("Wartość nie może być pusta"));
        ret = false;
    } else {
        bool ok;
        y = sy.toInt(&ok);
        if (!ok) {
            ui->errorWentylatorY->setVisible(true);
            ui->errorWentylatorY->setText(QString::fromUtf8("Wartość musi być liczbą"));
            ret = false;
        } else {
            if (y < 800) {
                ui->errorWentylatorY->setVisible(true);
                ui->errorWentylatorY->setText(QString::fromUtf8("Wartość musi być liczbą wiekszą od 800"));
                ret = false;
            } else if (y > 5000) {
                ui->errorWentylatorY->setVisible(true);
                ui->errorWentylatorY->setText(QString::fromUtf8("Wartość musi być liczbą mniejszą od 5000"));
                ret = false;
            }
        }
    }

    if (sl.isEmpty()) {
        ui->errorWentylatorL->setVisible(true);
        ui->errorWentylatorL->setText(QString::fromUtf8("Wartość nie może być pusta"));
        ret = false;
    } else {
        bool ok;
        l = sl.toInt(&ok);
        if (!ok) {
            ui->errorWentylatorL->setVisible(true);
            ui->errorWentylatorL->setText(QString::fromUtf8("Wartość musi być liczbą"));
            ret = false;
        } else {
            if (l < 0) {
                ui->errorWentylatorL->setVisible(true);
                ui->errorWentylatorL->setText(QString::fromUtf8("Wartość musi być liczbą wiekszą od zera"));
                ret = false;
            } else if (l > 200) {
                ui->errorWentylatorL->setVisible(true);
                ui->errorWentylatorL->setText(QString::fromUtf8("Wartość musi być liczbą mniejszą od 200"));
                ret = false;
            }
        }
    }

    if (ret) {
        impmmx = mech.getWentX(x, l);
        impmmy = mech.getWentY(y);
        debug(QString("wyliczone wartosci x=%1 y=%2").arg(impmmx).arg(impmmy));

        if (impmmx < 0) {
            ui->errorWentylatorX->setText(QString("Wyliczona wartość %1 mniejsza od zera").arg(impmmx));
            ui->errorWentylatorX->setVisible(true);
            ret = false;
        }

        if (impmmy < 0) {
            ui->errorWentylatorY->setText(QString("Wyliczona wartość %1 mniejsza od zera").arg(impmmy));
            ui->errorWentylatorY->setVisible(true);
            ret = false;
        }

        if (impmmx > (int)mech.getMaxXmm()) {
            ui->errorWentylatorX->setText(QString("Wyliczona wartość %1 jest większa niż %2").arg(impmmx).arg(mech.getMaxXmm()));
            ui->errorWentylatorX->setVisible(true);
            ret = false;
        }

        if (impmmy > (int)mech.getMaxYmm()) {
            ui->errorWentylatorY->setText(QString("Wyliczona wartość %1 jest większa niż %2").arg(impmmy).arg(mech.getMaxYmm()));
            ui->errorWentylatorY->setVisible(true);
            ret = false;
        }
    }

    if (ret) {
        impx = mech.getImpulsyX(impmmx);
        impy = mech.getImpulsyY(impmmy);
    } else {
        QPalette palette = ui->errorWentylatorX->palette();
        palette.setColor(QPalette::WindowText, Qt::red);
        ui->errorWentylatorX->setPalette(palette);

        palette = ui->errorWentylatorY->palette();
        palette.setColor(QPalette::WindowText, Qt::red);
        ui->errorWentylatorY->setPalette(palette);

        palette = ui->errorWentylatorL->palette();
        palette.setColor(QPalette::WindowText, Qt::red);
        ui->errorWentylatorL->setPalette(palette);
    }
    return ret;
}
