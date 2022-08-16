#include "podzialetapurolety.h"
#include "ui_podzialetapurolety.h"

#include <QLabel>
#include <QSpinBox>
#include <QDebug>

PodzialEtapuRolety::PodzialEtapuRolety(QWidget *parent, bool reverse, unsigned int nrEtapow_, unsigned int iloscProbek_,
                                       unsigned int czasStablizacji_, unsigned int maxR_, unsigned int width_,
                                       unsigned int offsetX_, unsigned int offsetY_) :
    QDialog(parent),
    ui(new Ui::PodzialEtapuRolety),
    reverseX(reverse),
    nrEtapow(nrEtapow_),
    iloscProbek(iloscProbek_),
    czasStablizacji(czasStablizacji_),
    maxR(maxR_),
    pozycje(reverse, nrEtapow, maxR, iloscProbek, czasStablizacji, width_, offsetX_, offsetY_)
{
    ui->setupUi(this);
    setWindowTitle("Przepływomierz - wybór norm dla każdego etapu.");

    for (unsigned int e = 0; e < 15; ++e) {
        jedenWidget(e, nrEtapow);
    }

    qDebug() << __FILE__ << __LINE__ << offsetX_ << offsetY_ << reverse;
    pozycje.setInit(nrEtapow_, maxR_, width_, iloscProbek_, czasStablizacji_);
    pozycje.setOffset(reverse, offsetX_, offsetY_);
}



PodzialEtapuRolety::~PodzialEtapuRolety()
{
    delete ui;
}

void PodzialEtapuRolety::jedenWidget(unsigned int etap, unsigned int maxEtap)
{
    int row = etap >= 8 ? 2*(etap-8) + 4 : 2*etap + 4;

    QFrame * line1L = new QFrame(this);
    line1L->setObjectName(QString::fromUtf8("line1L")+QString::number(etap));
    line1L->setFrameShape(QFrame::VLine);
    line1L->setFrameShadow(QFrame::Sunken);
    ui->gridLayout->addWidget(line1L, row, etap >= 8 ? 8 : 0, 1, 1);

    QLabel * label = new QLabel(this);
    label->setObjectName(QString::fromUtf8("labelEtap_%1").arg(etap));
    label->setText(QString("%1").arg(etap+1));
    ui->gridLayout->addWidget(label, row, etap >= 8 ? 9 : 1, 1, 1);

    QFrame * line2L = new QFrame(this);
    line2L->setObjectName(QString::fromUtf8("line2L")+QString::number(etap));
    line2L->setFrameShape(QFrame::VLine);
    line2L->setFrameShadow(QFrame::Sunken);
    ui->gridLayout->addWidget(line2L, row, etap >= 8 ? 10 : 2, 1, 1);

    QSpinBox * Xpart = new QSpinBox(this);
    Xpart->setObjectName(QString::fromUtf8("Xpart_%1").arg(etap));
    Xpart->setMinimum(5);
    Xpart->setMaximum(10);
    Xpart->setValue(5);
    xspinbox[etap] = Xpart;
    ui->gridLayout->addWidget(Xpart, row, etap >= 8 ? 11 : 3, 1, 1);
    Xpart->setEnabled(etap < maxEtap);

    QFrame * line3L = new QFrame(this);
    line3L->setObjectName(QString::fromUtf8("line2L")+QString::number(etap));
    line3L->setFrameShape(QFrame::VLine);
    line3L->setFrameShadow(QFrame::Sunken);
    ui->gridLayout->addWidget(line3L, row, etap >= 8 ? 12 : 4, 1, 1);

    QSpinBox * Ypart = new QSpinBox(this);
    Ypart->setObjectName(QString::fromUtf8("Ypart_%1").arg(etap));
    Ypart->setMinimum(5);
    Ypart->setMaximum(10);
    Ypart->setValue(5);
    yspinbox[etap] = Ypart;
    ui->gridLayout->addWidget(Ypart, row, etap >= 8 ? 13 : 5, 1, 1);
    Ypart->setEnabled(etap < maxEtap);

    QFrame * line4L = new QFrame(this);
    line4L->setObjectName(QString::fromUtf8("line2L")+QString::number(etap));
    line4L->setFrameShape(QFrame::VLine);
    line4L->setFrameShadow(QFrame::Sunken);
    ui->gridLayout->addWidget(line4L, row, etap >= 8 ? 14 : 6, 1, 1);

    if (etap < 8) {
        QFrame * line1H = new QFrame(this);
        line1H->setObjectName(QString::fromUtf8("lineH")+QString::number(etap+3));
        line1H->setFrameShape(QFrame::HLine);
        line1H->setFrameShadow(QFrame::Sunken);
        ui->gridLayout->addWidget(line1H, row+1, 0, 1, etap == 7 ? 7 : 15);
    }
}

void PodzialEtapuRolety::on_buttonBox_accepted()
{
    for (unsigned int i =0; i< nrEtapow; i++) {
        pozycje.appendRoletaEtap(i);
        pozycje.appendPoints(i,xspinbox[i]->value(),yspinbox[i]->value());
    }
}

const PozycjeRol &PodzialEtapuRolety::getLista() const
{
    return pozycje;
}

