/********************************************************************************
** Form generated from reading UI file 'wyborkwadratow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WYBORKWADRATOW_H
#define UI_WYBORKWADRATOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_WyborKwadratow
{
public:
    QGridLayout *gridLayout;
    QFrame *frWyborKw;
    QGridLayout *grWyborKw;
    QTableWidget *tbWybrane;
    QLabel *label;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *pbBack;
    QPushButton *pbClear;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *WyborKwadratow)
    {
        if (WyborKwadratow->objectName().isEmpty())
            WyborKwadratow->setObjectName(QString::fromUtf8("WyborKwadratow"));
        WyborKwadratow->resize(797, 315);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/prefix1/cnbop.ico"), QSize(), QIcon::Normal, QIcon::Off);
        WyborKwadratow->setWindowIcon(icon);
        gridLayout = new QGridLayout(WyborKwadratow);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        frWyborKw = new QFrame(WyborKwadratow);
        frWyborKw->setObjectName(QString::fromUtf8("frWyborKw"));
        frWyborKw->setFrameShape(QFrame::StyledPanel);
        frWyborKw->setFrameShadow(QFrame::Raised);
        grWyborKw = new QGridLayout(frWyborKw);
        grWyborKw->setObjectName(QString::fromUtf8("grWyborKw"));

        gridLayout->addWidget(frWyborKw, 1, 0, 4, 3);

        tbWybrane = new QTableWidget(WyborKwadratow);
        tbWybrane->setObjectName(QString::fromUtf8("tbWybrane"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tbWybrane->sizePolicy().hasHeightForWidth());
        tbWybrane->setSizePolicy(sizePolicy);

        gridLayout->addWidget(tbWybrane, 1, 3, 3, 1);

        label = new QLabel(WyborKwadratow);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 3);

        label_2 = new QLabel(WyborKwadratow);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 0, 3, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pbBack = new QPushButton(WyborKwadratow);
        pbBack->setObjectName(QString::fromUtf8("pbBack"));

        horizontalLayout->addWidget(pbBack);

        pbClear = new QPushButton(WyborKwadratow);
        pbClear->setObjectName(QString::fromUtf8("pbClear"));

        horizontalLayout->addWidget(pbClear);


        gridLayout->addLayout(horizontalLayout, 4, 3, 1, 1);

        buttonBox = new QDialogButtonBox(WyborKwadratow);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 5, 0, 1, 2);


        retranslateUi(WyborKwadratow);
        QObject::connect(buttonBox, SIGNAL(accepted()), WyborKwadratow, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), WyborKwadratow, SLOT(reject()));

        QMetaObject::connectSlotsByName(WyborKwadratow);
    } // setupUi

    void retranslateUi(QDialog *WyborKwadratow)
    {
        WyborKwadratow->setWindowTitle(QCoreApplication::translate("WyborKwadratow", "Wyb\303\263r pozycji", nullptr));
        label->setText(QCoreApplication::translate("WyborKwadratow", "Wybierz pozycje", nullptr));
        label_2->setText(QCoreApplication::translate("WyborKwadratow", "Wybrane pozycje", nullptr));
        pbBack->setText(QCoreApplication::translate("WyborKwadratow", "cofnij", nullptr));
        pbClear->setText(QCoreApplication::translate("WyborKwadratow", "wyczy\305\233\304\207", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WyborKwadratow: public Ui_WyborKwadratow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WYBORKWADRATOW_H
