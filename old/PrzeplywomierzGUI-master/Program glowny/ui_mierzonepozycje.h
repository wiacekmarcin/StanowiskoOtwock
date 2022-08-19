/********************************************************************************
** Form generated from reading UI file 'mierzonepozycje.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MIERZONEPOZYCJE_H
#define UI_MIERZONEPOZYCJE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MierzonePozycje
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pbNoweDane;
    QPushButton *pbStart;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pbZapisz;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLabel *status;
    QSpacerItem *horizontalSpacer;
    QTableWidget *table;
    QTextEdit *localdebug;
    QTextEdit *debug;

    void setupUi(QWidget *MierzonePozycje)
    {
        if (MierzonePozycje->objectName().isEmpty())
            MierzonePozycje->setObjectName(QString::fromUtf8("MierzonePozycje"));
        MierzonePozycje->resize(640, 480);
        verticalLayout = new QVBoxLayout(MierzonePozycje);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        widget = new QWidget(MierzonePozycje);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pbNoweDane = new QPushButton(widget);
        pbNoweDane->setObjectName(QString::fromUtf8("pbNoweDane"));

        horizontalLayout->addWidget(pbNoweDane);

        pbStart = new QPushButton(widget);
        pbStart->setObjectName(QString::fromUtf8("pbStart"));

        horizontalLayout->addWidget(pbStart);

        horizontalSpacer_2 = new QSpacerItem(293, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        pbZapisz = new QPushButton(widget);
        pbZapisz->setObjectName(QString::fromUtf8("pbZapisz"));

        horizontalLayout->addWidget(pbZapisz);


        verticalLayout->addWidget(widget);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(MierzonePozycje);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_2->addWidget(label);

        status = new QLabel(MierzonePozycje);
        status->setObjectName(QString::fromUtf8("status"));

        horizontalLayout_2->addWidget(status);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_2);

        table = new QTableWidget(MierzonePozycje);
        table->setObjectName(QString::fromUtf8("table"));

        verticalLayout->addWidget(table);

        localdebug = new QTextEdit(MierzonePozycje);
        localdebug->setObjectName(QString::fromUtf8("localdebug"));

        verticalLayout->addWidget(localdebug);

        debug = new QTextEdit(MierzonePozycje);
        debug->setObjectName(QString::fromUtf8("debug"));

        verticalLayout->addWidget(debug);


        retranslateUi(MierzonePozycje);

        QMetaObject::connectSlotsByName(MierzonePozycje);
    } // setupUi

    void retranslateUi(QWidget *MierzonePozycje)
    {
        MierzonePozycje->setWindowTitle(QCoreApplication::translate("MierzonePozycje", "Form", nullptr));
        pbNoweDane->setText(QCoreApplication::translate("MierzonePozycje", "Nowe dane", nullptr));
        pbStart->setText(QCoreApplication::translate("MierzonePozycje", "Start", nullptr));
        pbZapisz->setText(QCoreApplication::translate("MierzonePozycje", "Zapisz Dane", nullptr));
        label->setText(QCoreApplication::translate("MierzonePozycje", "Status:", nullptr));
        status->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MierzonePozycje: public Ui_MierzonePozycje {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MIERZONEPOZYCJE_H
