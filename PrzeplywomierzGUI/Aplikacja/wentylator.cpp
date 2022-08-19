#include "wentylator.h"
#include "miernikprzeplywu.h"
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
    ui->pbUstaw->setVisible(true);
    //ui->statusWentylator->setVisible(false);

    connect(ui->pbUstaw, &QPushButton::clicked, this, &Wentylator::pbUstaw_clicked);
    connect(ui->pbZeruj, &QPushButton::clicked, this, &Wentylator::pbZeruj_clicked);
}

Wentylator::~Wentylator()
{
    delete ui;
}

void Wentylator::pbUstaw_clicked()
{
    ui->pbUstaw->setEnabled(false);
    ui->statusWentylator->clear();
    ui->lStatusWiatrak->setText(QString("Sprawdzam dane ...."));
    if (!sprawdz()) {
        ui->lStatusWiatrak->setText(QString("Dane niepoprawne"));
        addStatus("Dane nie poprawne");
        ui->pbUstaw->setEnabled(true);
        return;
    }

   addStatus("Szukam kontrolera.");
   ui->lStatusWiatrak->setText(QString("Szukam urzadzenia ...."));
   connectToDevice();

}

void Wentylator::pbZeruj_clicked()
{
    if (true /*getConnect()*/) {
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
    qDebug() << __FILE__ << __LINE__;
    if (home) {
        ui->lStatusWiatrak->setText("Ustawiem zadaną pozycję.");
        addStatus("Zaczynam ustawiac pozycje");
        setPosition(impx, impy);
    } else {
        ui->pbUstaw->setEnabled(true);
        ui->lStatusWiatrak->setText("Pozycja wentylatora ustawiona");
        addStatus("Pozycja ustawiona.");
        setClose(false);
    }
}

void Wentylator::setStop()
{
    ui->pbUstaw->setEnabled(true);
}

void Wentylator::setError()
{
    ui->pbUstaw->setEnabled(true);
    ui->lStatusWiatrak->setText("Wystapił błąd.");
    addStatus("Wystapił błąd.");
}

bool Wentylator::sprawdz()
{
    QString sx = ui->leX->text();
    QString sy = ui->leY->text();
    QString sl = ui->leL->text();

    addStatus(QString("Ustawiam pozycje wentylatora X=%1 Y=%2 L=%3").arg(sx).arg(sy).arg(sl));

    int x = 0, y = 0, l = 0;
    bool ret = true;
    int impmmx, impmmy;

    ui->errorWentylatorX->setVisible(false);
    ui->errorWentylatorY->setVisible(false);
    ui->errorWentylatorL->setVisible(false);
    if (sx.isEmpty()) {
        ui->errorWentylatorX->setVisible(true);
        ui->errorWentylatorX->setText(QString::fromUtf8("Wartość nie może być pusta."));
        addStatus(QString::fromUtf8("Wartość X nie może być pusta."));
        ret = false;
    } else {
        bool ok;
        x = sx.toInt(&ok);
        if (!ok) {
            ui->errorWentylatorX->setVisible(true);
            ui->errorWentylatorX->setText(QString::fromUtf8("Wartość musi być liczbą."));
            addStatus(QString::fromUtf8("Wartość X musi być liczba."));
            ret = false;
        } else {
            if (x < 0) {
                ui->errorWentylatorX->setVisible(true);
                ui->errorWentylatorX->setText(QString::fromUtf8("Wartość musi być liczbą wiekszą od zera."));
                addStatus(QString::fromUtf8("Wartość X musi być liczba z zakresu [0-2200]."));
                ret = false;
            } else if (x > 2200) {
                ui->errorWentylatorX->setVisible(true);
                ui->errorWentylatorX->setText(QString::fromUtf8("Wartość musi być liczbą mniejszą od 2200."));
                addStatus(QString::fromUtf8("Wartość X musi być liczba z zakresu [0-2200]."));
                ret = false;
            }
        }
    }

    if (sy.isEmpty()) {
        ui->errorWentylatorY->setVisible(true);
        ui->errorWentylatorY->setText(QString::fromUtf8("Wartość nie może być pusta."));
        addStatus(QString::fromUtf8("Wartość Y nie może być pusta."));
        ret = false;
    } else {
        bool ok;
        y = sy.toInt(&ok);
        if (!ok) {
            ui->errorWentylatorY->setVisible(true);
            ui->errorWentylatorY->setText(QString::fromUtf8("Wartość musi być liczbą."));
            addStatus(QString::fromUtf8("Wartość Y musi być liczbą."));
            ret = false;
        } else {
            if (y < 800) {
                ui->errorWentylatorY->setVisible(true);
                ui->errorWentylatorY->setText(QString::fromUtf8("Wartość musi być liczbą wiekszą od 800"));
                addStatus(QString::fromUtf8("Wartość Y musi być liczba z zakresu [800-5000]."));
                ret = false;
            } else if (y > 5000) {
                ui->errorWentylatorY->setVisible(true);
                ui->errorWentylatorY->setText(QString::fromUtf8("Wartość musi być liczbą mniejszą od 5000."));
                addStatus(QString::fromUtf8("Wartość Y musi być liczba z zakresu [800-5000]."));
                ret = false;
            }
        }
    }

    if (sl.isEmpty()) {
        ui->errorWentylatorL->setVisible(true);
        ui->errorWentylatorL->setText(QString::fromUtf8("Wartość nie może być pusta."));
        addStatus(QString::fromUtf8("Wartość L nie może być pusta."));
        ret = false;
    } else {
        bool ok;
        l = sl.toInt(&ok);
        if (!ok) {
            ui->errorWentylatorL->setVisible(true);
            ui->errorWentylatorL->setText(QString::fromUtf8("Wartość musi być liczbą."));
            addStatus(QString::fromUtf8("Wartość L musi być liczbą."));
            ret = false;
        } else {
            if (l < 0) {
                ui->errorWentylatorL->setVisible(true);
                ui->errorWentylatorL->setText(QString::fromUtf8("Wartość musi być liczbą wiekszą od zera."));
                addStatus(QString::fromUtf8("Wartość L musi być liczba z zakresu [0-200]."));
                ret = false;
            } else if (l > 200) {
                ui->errorWentylatorL->setVisible(true);
                ui->errorWentylatorL->setText(QString::fromUtf8("Wartość musi być liczbą mniejszą od 200"));
                addStatus(QString::fromUtf8("Wartość L musi być liczba z zakresu [0-200]."));
                ret = false;
            }
        }
    }

    if (ret) {
        impmmx = mech.getWentX(x, l);
        impmmy = mech.getWentY(y);
        addStatus(QString("Wyliczone wartosci x=%1 mm y=%2 mm .").arg(impmmx).arg(impmmy));

        if (impmmx < 0) {
            ui->errorWentylatorX->setText(QString("Wyliczona wartość %1 mniejsza od zera").arg(impmmx));
            ui->errorWentylatorX->setVisible(true);
            addStatus(QString("Wyliczona wartość  x=%1 mm jest mniejsza od zera.").arg(impmmx));
            ret = false;
        }

        if (impmmy < 0) {
            ui->errorWentylatorY->setText(QString("Wyliczona wartość %1 mniejsza od zera").arg(impmmy));
            ui->errorWentylatorY->setVisible(true);
            addStatus(QString("Wyliczona wartość  y=%1 mm jest mniejsza od zera.").arg(impmmy));
            ret = false;
        }

        if (impmmx > (int)mech.getMaxXmm()) {
            ui->errorWentylatorX->setText(QString("Wyliczona wartość %1 jest większa niż %2").arg(impmmx).arg(mech.getMaxXmm()));
            ui->errorWentylatorX->setVisible(true);
            addStatus(QString("Wyliczona wartość  x=%1 mm jest większa od %2").arg(impmmx).arg(mech.getMaxXmm()));
            ret = false;
        }

        if (impmmy > (int)mech.getMaxYmm()) {
            ui->errorWentylatorY->setText(QString("Wyliczona wartość %1 jest większa niż %2").arg(impmmy).arg(mech.getMaxYmm()));
            ui->errorWentylatorY->setVisible(true);
            addStatus(QString("Wyliczona wartość  y=%1 mm jest większa od %2").arg(impmmy).arg(mech.getMaxYmm()));
            ret = false;
        }
    }

    if (ret) {
        impx = mech.getImpulsyX(impmmx);
        impy = mech.getImpulsyY(impmmy);
        addStatus(QString("Wyliczone wartosci impulsów x=%1 mm y=%2 mm .").arg(impx).arg(impy));
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

void Wentylator::addStatus(const QString &log)
{
    ui->statusWentylator->append(addTime(log));
}

void Wentylator::on_pbSelect_clicked()
{
    if (miernikPrzeplywu)
        TabWidget::miernikPrzeplywu->noweDane();
}

