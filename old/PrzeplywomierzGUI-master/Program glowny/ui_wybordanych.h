/********************************************************************************
** Form generated from reading UI file 'wybordanych.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WYBORDANYCH_H
#define UI_WYBORDANYCH_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QGridLayout *gridLayout;
    QRadioButton *rbhalfmanual;
    QDialogButtonBox *buttonBox;
    QFrame *frChooseFile;
    QLabel *label;
    QRadioButton *rbfile;
    QRadioButton *rbmanual;
    QFrame *frHalfManual;
    QFrame *frManual;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(480, 640);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/prefix1/cnbop.ico"), QSize(), QIcon::Normal, QIcon::Off);
        Dialog->setWindowIcon(icon);
        gridLayout = new QGridLayout(Dialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        rbhalfmanual = new QRadioButton(Dialog);
        rbhalfmanual->setObjectName(QString::fromUtf8("rbhalfmanual"));

        gridLayout->addWidget(rbhalfmanual, 5, 0, 1, 1);

        buttonBox = new QDialogButtonBox(Dialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 7, 0, 1, 2);

        frChooseFile = new QFrame(Dialog);
        frChooseFile->setObjectName(QString::fromUtf8("frChooseFile"));
        frChooseFile->setFrameShape(QFrame::StyledPanel);
        frChooseFile->setFrameShadow(QFrame::Raised);

        gridLayout->addWidget(frChooseFile, 1, 1, 2, 2);

        label = new QLabel(Dialog);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 2);

        rbfile = new QRadioButton(Dialog);
        rbfile->setObjectName(QString::fromUtf8("rbfile"));

        gridLayout->addWidget(rbfile, 1, 0, 1, 1);

        rbmanual = new QRadioButton(Dialog);
        rbmanual->setObjectName(QString::fromUtf8("rbmanual"));

        gridLayout->addWidget(rbmanual, 3, 0, 1, 1);

        frHalfManual = new QFrame(Dialog);
        frHalfManual->setObjectName(QString::fromUtf8("frHalfManual"));
        frHalfManual->setFrameShape(QFrame::StyledPanel);
        frHalfManual->setFrameShadow(QFrame::Raised);

        gridLayout->addWidget(frHalfManual, 3, 1, 2, 2);

        frManual = new QFrame(Dialog);
        frManual->setObjectName(QString::fromUtf8("frManual"));
        frManual->setFrameShape(QFrame::StyledPanel);
        frManual->setFrameShadow(QFrame::Raised);

        gridLayout->addWidget(frManual, 5, 1, 2, 2);


        retranslateUi(Dialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), Dialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Dialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "Dialog", nullptr));
        rbhalfmanual->setText(QCoreApplication::translate("Dialog", "Dane wyb&ierane \n"
"z p\305\202aszczyzny", nullptr));
        label->setText(QCoreApplication::translate("Dialog", "Wybierz metod\304\205 wprowadzania danch:", nullptr));
        rbfile->setText(QCoreApplication::translate("Dialog", "Dane z pliku", nullptr));
        rbmanual->setText(QCoreApplication::translate("Dialog", "Dane &wprowadzane\n"
"r\304\231cznie", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WYBORDANYCH_H
