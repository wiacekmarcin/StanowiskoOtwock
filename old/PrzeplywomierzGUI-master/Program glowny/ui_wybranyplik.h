/********************************************************************************
** Form generated from reading UI file 'wybranyplik.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WYBRANYPLIK_H
#define UI_WYBRANYPLIK_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_WybranyPlik
{
public:
    QFormLayout *formLayout;
    QLabel *efilename;
    QLabel *lfilename;
    QLabel *label;
    QPlainTextEdit *leError;
    QDialogButtonBox *buttonBox;
    QLabel *lfindRecords;
    QTableWidget *tbWybrane;

    void setupUi(QDialog *WybranyPlik)
    {
        if (WybranyPlik->objectName().isEmpty())
            WybranyPlik->setObjectName(QString::fromUtf8("WybranyPlik"));
        WybranyPlik->resize(640, 480);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/prefix1/cnbop.ico"), QSize(), QIcon::Normal, QIcon::Off);
        WybranyPlik->setWindowIcon(icon);
        formLayout = new QFormLayout(WybranyPlik);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        efilename = new QLabel(WybranyPlik);
        efilename->setObjectName(QString::fromUtf8("efilename"));

        formLayout->setWidget(0, QFormLayout::LabelRole, efilename);

        lfilename = new QLabel(WybranyPlik);
        lfilename->setObjectName(QString::fromUtf8("lfilename"));

        formLayout->setWidget(0, QFormLayout::FieldRole, lfilename);

        label = new QLabel(WybranyPlik);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label);

        leError = new QPlainTextEdit(WybranyPlik);
        leError->setObjectName(QString::fromUtf8("leError"));

        formLayout->setWidget(2, QFormLayout::FieldRole, leError);

        buttonBox = new QDialogButtonBox(WybranyPlik);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        formLayout->setWidget(3, QFormLayout::SpanningRole, buttonBox);

        lfindRecords = new QLabel(WybranyPlik);
        lfindRecords->setObjectName(QString::fromUtf8("lfindRecords"));

        formLayout->setWidget(1, QFormLayout::LabelRole, lfindRecords);

        tbWybrane = new QTableWidget(WybranyPlik);
        tbWybrane->setObjectName(QString::fromUtf8("tbWybrane"));

        formLayout->setWidget(1, QFormLayout::FieldRole, tbWybrane);

        QWidget::setTabOrder(tbWybrane, leError);

        retranslateUi(WybranyPlik);
        QObject::connect(buttonBox, SIGNAL(accepted()), WybranyPlik, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), WybranyPlik, SLOT(reject()));

        QMetaObject::connectSlotsByName(WybranyPlik);
    } // setupUi

    void retranslateUi(QDialog *WybranyPlik)
    {
        WybranyPlik->setWindowTitle(QCoreApplication::translate("WybranyPlik", "Dane z pliku", nullptr));
        efilename->setText(QCoreApplication::translate("WybranyPlik", "Plik:", nullptr));
        lfilename->setText(QCoreApplication::translate("WybranyPlik", "--", nullptr));
        label->setText(QCoreApplication::translate("WybranyPlik", "B\305\202\304\231dne linie", nullptr));
        lfindRecords->setText(QCoreApplication::translate("WybranyPlik", "Znalezione\n"
"rekordy", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WybranyPlik: public Ui_WybranyPlik {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WYBRANYPLIK_H
