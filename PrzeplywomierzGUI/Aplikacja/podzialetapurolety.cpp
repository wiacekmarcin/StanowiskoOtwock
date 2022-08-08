#include "podzialetapurolety.h"
#include "ui_podzialetapurolety.h"

#include <QLabel>
#include <QSpinBox>
#include <QDebug>

PodzialEtapuRolety::PodzialEtapuRolety(QWidget *parent, unsigned int nrEtapow_, unsigned int iloscProbek_,
                                       unsigned int czasStablizacji_, unsigned int maxR_, unsigned int width_,
                                       unsigned int offsetX_, unsigned int offsetY_) :
    QDialog(parent),
    ui(new Ui::PodzialEtapuRolety),
    nrEtapow(nrEtapow_),
    iloscProbek(iloscProbek_),
    czasStablizacji(czasStablizacji_),
    maxR(maxR_)
{
    ui->setupUi(this);

    for (unsigned int e = 0; e < nrEtapow; ++e) {
        jedenWidget(e);
    }

    qDebug() << __FILE__ << __LINE__ << offsetX_ << offsetY_;
    pozycje.setInit(nrEtapow_, maxR_, width_, iloscProbek_, czasStablizacji_);
    pozycje.setOffset(offsetX_, offsetY_);
}



PodzialEtapuRolety::~PodzialEtapuRolety()
{
    delete ui;
}

void PodzialEtapuRolety::jedenWidget(unsigned int etap)
{
    QWidget * w = new QWidget(ui->widget);
    w->setObjectName(QString::fromUtf8("widget_%1").arg(etap));
    QHBoxLayout * horizontalLayout = new QHBoxLayout(w);
    w->setLayout(horizontalLayout);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout_%1").arg(etap));
    QLabel * label = new QLabel(w);
    label->setObjectName(QString::fromUtf8("label_%1").arg(etap));
    label->setText(QString("Etap nr %1").arg(etap+1));
    horizontalLayout->addWidget(label);

    QSpinBox * Xpart = new QSpinBox(w);
    Xpart->setObjectName(QString::fromUtf8("Xpart_%1").arg(etap));
    Xpart->setMinimum(5);
    Xpart->setMaximum(10);
    Xpart->setValue(5);
    xspinbox[etap] = Xpart;
    horizontalLayout->addWidget(Xpart);

    QSpinBox * Ypart = new QSpinBox(w);
    Ypart->setObjectName(QString::fromUtf8("Ypart_%1").arg(etap));
    Ypart->setMinimum(5);
    Ypart->setMaximum(10);
    Ypart->setValue(5);
    yspinbox[etap] = Ypart;
    horizontalLayout->addWidget(Ypart);

    ui->gridLayout->addWidget(w, etap / 4, etap % 4, 1, 1);
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

