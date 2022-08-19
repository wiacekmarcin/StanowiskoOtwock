/********************************************************************************
** Form generated from reading UI file 'recznedodpozycji.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RECZNEDODPOZYCJI_H
#define UI_RECZNEDODPOZYCJI_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ReczneDodPozycji
{
public:
    QVBoxLayout *verticalLayout;
    QTableWidget *tbWybrane;
    QHBoxLayout *horizontalLayout;
    QPushButton *pbSprawdz;
    QSpacerItem *horizontalSpacer;
    QPushButton *pbOk;
    QPushButton *pbAnuluj;

    void setupUi(QDialog *ReczneDodPozycji)
    {
        if (ReczneDodPozycji->objectName().isEmpty())
            ReczneDodPozycji->setObjectName(QString::fromUtf8("ReczneDodPozycji"));
        ReczneDodPozycji->resize(640, 480);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/prefix1/cnbop.ico"), QSize(), QIcon::Normal, QIcon::Off);
        ReczneDodPozycji->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(ReczneDodPozycji);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tbWybrane = new QTableWidget(ReczneDodPozycji);
        tbWybrane->setObjectName(QString::fromUtf8("tbWybrane"));

        verticalLayout->addWidget(tbWybrane);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pbSprawdz = new QPushButton(ReczneDodPozycji);
        pbSprawdz->setObjectName(QString::fromUtf8("pbSprawdz"));

        horizontalLayout->addWidget(pbSprawdz);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pbOk = new QPushButton(ReczneDodPozycji);
        pbOk->setObjectName(QString::fromUtf8("pbOk"));

        horizontalLayout->addWidget(pbOk);

        pbAnuluj = new QPushButton(ReczneDodPozycji);
        pbAnuluj->setObjectName(QString::fromUtf8("pbAnuluj"));

        horizontalLayout->addWidget(pbAnuluj);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(ReczneDodPozycji);

        QMetaObject::connectSlotsByName(ReczneDodPozycji);
    } // setupUi

    void retranslateUi(QDialog *ReczneDodPozycji)
    {
        ReczneDodPozycji->setWindowTitle(QCoreApplication::translate("ReczneDodPozycji", "Dodaj list\304\231 pozycji", nullptr));
        pbSprawdz->setText(QCoreApplication::translate("ReczneDodPozycji", "Sprawd\305\272 dane", nullptr));
        pbOk->setText(QCoreApplication::translate("ReczneDodPozycji", "OK", nullptr));
        pbAnuluj->setText(QCoreApplication::translate("ReczneDodPozycji", "Anuluj", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ReczneDodPozycji: public Ui_ReczneDodPozycji {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RECZNEDODPOZYCJI_H
