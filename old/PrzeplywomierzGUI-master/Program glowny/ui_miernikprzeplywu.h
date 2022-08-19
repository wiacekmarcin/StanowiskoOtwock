/********************************************************************************
** Form generated from reading UI file 'miernikprzeplywu.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MIERNIKPRZEPLYWU_H
#define UI_MIERNIKPRZEPLYWU_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MiernikPrzeplywu
{
public:
    QAction *actionNowy;
    QAction *actionZamknij;
    QAction *actionWyj_cie;
    QAction *actionZapisz_wyniki;
    QAction *actionPo_cz;
    QAction *actionPo_cz_2;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QGridLayout *gridLayout;
    QLabel *ecz1;
    QLabel *statusserial;
    QLabel *lStatus;
    QLabel *eKontroler;
    QSpacerItem *horizontalSpacer;
    QLabel *lcz1unit;
    QLabel *lcz1mv;
    QLabel *lradio;
    QLabel *eStatus;
    QLabel *eRadio;
    QLabel *lConneected;
    QTabWidget *tabWidget;
    QWidget *tab_2700;
    QVBoxLayout *verticalLaoutTab2700;
    QWidget *tab_1000p;
    QVBoxLayout *verticallayoutTab1000p;
    QWidget *tab_1000l;
    QVBoxLayout *verticallayoutTab1000l;
    QWidget *tab_fun;
    QGridLayout *gridLayout_2;
    QLabel *errorWentylatorY;
    QLabel *eStatusWiatrak;
    QLineEdit *leL;
    QSpacerItem *horizontalSpacer_12;
    QLabel *label;
    QSpacerItem *verticalSpacer_2;
    QLabel *lmmy;
    QLabel *errorWentylatorX;
    QLineEdit *leX;
    QLabel *lStatusWiatrak;
    QFrame *line_3;
    QLineEdit *leY;
    QTextEdit *statusWentylator;
    QLabel *label_2;
    QLabel *errorWentylatorL;
    QLabel *eY;
    QLabel *lmmx;
    QTextEdit *debug;
    QLabel *eX;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout;
    QPushButton *pbUstaw;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pbZeruj;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label_4;
    QLabel *label_5;
    QPushButton *pbWybierzStanowisko;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MiernikPrzeplywu)
    {
        if (MiernikPrzeplywu->objectName().isEmpty())
            MiernikPrzeplywu->setObjectName(QString::fromUtf8("MiernikPrzeplywu"));
        MiernikPrzeplywu->resize(1091, 869);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/prefix1/cnbop.ico"), QSize(), QIcon::Normal, QIcon::Off);
        MiernikPrzeplywu->setWindowIcon(icon);
        actionNowy = new QAction(MiernikPrzeplywu);
        actionNowy->setObjectName(QString::fromUtf8("actionNowy"));
        actionZamknij = new QAction(MiernikPrzeplywu);
        actionZamknij->setObjectName(QString::fromUtf8("actionZamknij"));
        actionWyj_cie = new QAction(MiernikPrzeplywu);
        actionWyj_cie->setObjectName(QString::fromUtf8("actionWyj_cie"));
        actionZapisz_wyniki = new QAction(MiernikPrzeplywu);
        actionZapisz_wyniki->setObjectName(QString::fromUtf8("actionZapisz_wyniki"));
        actionPo_cz = new QAction(MiernikPrzeplywu);
        actionPo_cz->setObjectName(QString::fromUtf8("actionPo_cz"));
        actionPo_cz_2 = new QAction(MiernikPrzeplywu);
        actionPo_cz_2->setObjectName(QString::fromUtf8("actionPo_cz_2"));
        centralwidget = new QWidget(MiernikPrzeplywu);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        frame = new QFrame(centralwidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        ecz1 = new QLabel(frame);
        ecz1->setObjectName(QString::fromUtf8("ecz1"));

        gridLayout->addWidget(ecz1, 1, 0, 2, 1);

        statusserial = new QLabel(frame);
        statusserial->setObjectName(QString::fromUtf8("statusserial"));

        gridLayout->addWidget(statusserial, 0, 2, 1, 1);

        lStatus = new QLabel(frame);
        lStatus->setObjectName(QString::fromUtf8("lStatus"));

        gridLayout->addWidget(lStatus, 3, 1, 1, 6);

        eKontroler = new QLabel(frame);
        eKontroler->setObjectName(QString::fromUtf8("eKontroler"));

        gridLayout->addWidget(eKontroler, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 3, 1, 2);

        lcz1unit = new QLabel(frame);
        lcz1unit->setObjectName(QString::fromUtf8("lcz1unit"));

        gridLayout->addWidget(lcz1unit, 2, 1, 1, 1);

        lcz1mv = new QLabel(frame);
        lcz1mv->setObjectName(QString::fromUtf8("lcz1mv"));

        gridLayout->addWidget(lcz1mv, 1, 1, 1, 1);

        lradio = new QLabel(frame);
        lradio->setObjectName(QString::fromUtf8("lradio"));

        gridLayout->addWidget(lradio, 0, 6, 1, 1);

        eStatus = new QLabel(frame);
        eStatus->setObjectName(QString::fromUtf8("eStatus"));

        gridLayout->addWidget(eStatus, 3, 0, 1, 1);

        eRadio = new QLabel(frame);
        eRadio->setObjectName(QString::fromUtf8("eRadio"));

        gridLayout->addWidget(eRadio, 0, 5, 1, 1);

        lConneected = new QLabel(frame);
        lConneected->setObjectName(QString::fromUtf8("lConneected"));

        gridLayout->addWidget(lConneected, 0, 1, 1, 1);


        verticalLayout->addWidget(frame);

        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab_2700 = new QWidget();
        tab_2700->setObjectName(QString::fromUtf8("tab_2700"));
        verticalLaoutTab2700 = new QVBoxLayout(tab_2700);
        verticalLaoutTab2700->setObjectName(QString::fromUtf8("verticalLaoutTab2700"));
        tabWidget->addTab(tab_2700, QString());
        tab_1000p = new QWidget();
        tab_1000p->setObjectName(QString::fromUtf8("tab_1000p"));
        verticallayoutTab1000p = new QVBoxLayout(tab_1000p);
        verticallayoutTab1000p->setObjectName(QString::fromUtf8("verticallayoutTab1000p"));
        tabWidget->addTab(tab_1000p, QString());
        tab_1000l = new QWidget();
        tab_1000l->setObjectName(QString::fromUtf8("tab_1000l"));
        verticallayoutTab1000l = new QVBoxLayout(tab_1000l);
        verticallayoutTab1000l->setObjectName(QString::fromUtf8("verticallayoutTab1000l"));
        tabWidget->addTab(tab_1000l, QString());
        tab_fun = new QWidget();
        tab_fun->setObjectName(QString::fromUtf8("tab_fun"));
        gridLayout_2 = new QGridLayout(tab_fun);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        errorWentylatorY = new QLabel(tab_fun);
        errorWentylatorY->setObjectName(QString::fromUtf8("errorWentylatorY"));

        gridLayout_2->addWidget(errorWentylatorY, 3, 1, 1, 1);

        eStatusWiatrak = new QLabel(tab_fun);
        eStatusWiatrak->setObjectName(QString::fromUtf8("eStatusWiatrak"));

        gridLayout_2->addWidget(eStatusWiatrak, 8, 0, 1, 1);

        leL = new QLineEdit(tab_fun);
        leL->setObjectName(QString::fromUtf8("leL"));

        gridLayout_2->addWidget(leL, 4, 1, 1, 1);

        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_12, 0, 4, 1, 1);

        label = new QLabel(tab_fun);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_2->addWidget(label, 4, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_2, 11, 0, 1, 1);

        lmmy = new QLabel(tab_fun);
        lmmy->setObjectName(QString::fromUtf8("lmmy"));

        gridLayout_2->addWidget(lmmy, 2, 2, 1, 1);

        errorWentylatorX = new QLabel(tab_fun);
        errorWentylatorX->setObjectName(QString::fromUtf8("errorWentylatorX"));

        gridLayout_2->addWidget(errorWentylatorX, 1, 1, 1, 1);

        leX = new QLineEdit(tab_fun);
        leX->setObjectName(QString::fromUtf8("leX"));

        gridLayout_2->addWidget(leX, 0, 1, 1, 1);

        lStatusWiatrak = new QLabel(tab_fun);
        lStatusWiatrak->setObjectName(QString::fromUtf8("lStatusWiatrak"));

        gridLayout_2->addWidget(lStatusWiatrak, 8, 1, 1, 1);

        line_3 = new QFrame(tab_fun);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        gridLayout_2->addWidget(line_3, 7, 0, 1, 5);

        leY = new QLineEdit(tab_fun);
        leY->setObjectName(QString::fromUtf8("leY"));

        gridLayout_2->addWidget(leY, 2, 1, 1, 1);

        statusWentylator = new QTextEdit(tab_fun);
        statusWentylator->setObjectName(QString::fromUtf8("statusWentylator"));

        gridLayout_2->addWidget(statusWentylator, 9, 0, 1, 5);

        label_2 = new QLabel(tab_fun);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_2->addWidget(label_2, 4, 2, 1, 1);

        errorWentylatorL = new QLabel(tab_fun);
        errorWentylatorL->setObjectName(QString::fromUtf8("errorWentylatorL"));

        gridLayout_2->addWidget(errorWentylatorL, 5, 1, 1, 1);

        eY = new QLabel(tab_fun);
        eY->setObjectName(QString::fromUtf8("eY"));

        gridLayout_2->addWidget(eY, 2, 0, 1, 1);

        lmmx = new QLabel(tab_fun);
        lmmx->setObjectName(QString::fromUtf8("lmmx"));

        gridLayout_2->addWidget(lmmx, 0, 2, 1, 1);

        debug = new QTextEdit(tab_fun);
        debug->setObjectName(QString::fromUtf8("debug"));

        gridLayout_2->addWidget(debug, 10, 0, 1, 5);

        eX = new QLabel(tab_fun);
        eX->setObjectName(QString::fromUtf8("eX"));

        gridLayout_2->addWidget(eX, 0, 0, 1, 1);

        label_3 = new QLabel(tab_fun);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_2->addWidget(label_3, 0, 3, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pbUstaw = new QPushButton(tab_fun);
        pbUstaw->setObjectName(QString::fromUtf8("pbUstaw"));

        horizontalLayout->addWidget(pbUstaw);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        pbZeruj = new QPushButton(tab_fun);
        pbZeruj->setObjectName(QString::fromUtf8("pbZeruj"));

        horizontalLayout->addWidget(pbZeruj);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);


        gridLayout_2->addLayout(horizontalLayout, 6, 1, 1, 1);

        label_4 = new QLabel(tab_fun);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_2->addWidget(label_4, 2, 3, 1, 1);

        label_5 = new QLabel(tab_fun);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_2->addWidget(label_5, 4, 3, 1, 1);

        pbWybierzStanowisko = new QPushButton(tab_fun);
        pbWybierzStanowisko->setObjectName(QString::fromUtf8("pbWybierzStanowisko"));

        gridLayout_2->addWidget(pbWybierzStanowisko, 6, 4, 1, 1);

        tabWidget->addTab(tab_fun, QString());

        verticalLayout->addWidget(tabWidget);

        MiernikPrzeplywu->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MiernikPrzeplywu);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MiernikPrzeplywu->setStatusBar(statusbar);
        QWidget::setTabOrder(tabWidget, leX);
        QWidget::setTabOrder(leX, leY);
        QWidget::setTabOrder(leY, leL);
        QWidget::setTabOrder(leL, pbUstaw);
        QWidget::setTabOrder(pbUstaw, statusWentylator);
        QWidget::setTabOrder(statusWentylator, debug);

        retranslateUi(MiernikPrzeplywu);

        tabWidget->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(MiernikPrzeplywu);
    } // setupUi

    void retranslateUi(QMainWindow *MiernikPrzeplywu)
    {
        MiernikPrzeplywu->setWindowTitle(QCoreApplication::translate("MiernikPrzeplywu", "MiernikPrzeplywu", nullptr));
        actionNowy->setText(QCoreApplication::translate("MiernikPrzeplywu", "&Nowy", nullptr));
        actionZamknij->setText(QCoreApplication::translate("MiernikPrzeplywu", "&Zamknij", nullptr));
        actionWyj_cie->setText(QCoreApplication::translate("MiernikPrzeplywu", "Wyj\305\233cie", nullptr));
        actionZapisz_wyniki->setText(QCoreApplication::translate("MiernikPrzeplywu", "&Zapisz wyniki", nullptr));
        actionPo_cz->setText(QCoreApplication::translate("MiernikPrzeplywu", "Po\305\202\304\205cz", nullptr));
        actionPo_cz_2->setText(QCoreApplication::translate("MiernikPrzeplywu", "&Po\305\202\304\205cz", nullptr));
        ecz1->setText(QCoreApplication::translate("MiernikPrzeplywu", "Czujnik", nullptr));
        statusserial->setText(QString());
        lStatus->setText(QCoreApplication::translate("MiernikPrzeplywu", "--", nullptr));
        eKontroler->setText(QCoreApplication::translate("MiernikPrzeplywu", "Konroler silnik\303\263w :", nullptr));
        lcz1unit->setText(QCoreApplication::translate("MiernikPrzeplywu", "--", nullptr));
        lcz1mv->setText(QCoreApplication::translate("MiernikPrzeplywu", "--", nullptr));
        lradio->setText(QCoreApplication::translate("MiernikPrzeplywu", "Nie po\305\202\304\205czono", nullptr));
        eStatus->setText(QCoreApplication::translate("MiernikPrzeplywu", "Status pracy", nullptr));
        eRadio->setText(QCoreApplication::translate("MiernikPrzeplywu", "Czujnik radiowy :", nullptr));
        lConneected->setText(QCoreApplication::translate("MiernikPrzeplywu", "Nie wykryto", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2700), QCoreApplication::translate("MiernikPrzeplywu", "Stanowisko 2700x3000", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_1000p), QCoreApplication::translate("MiernikPrzeplywu", "Stanowisko 1000x2000 prawe", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_1000l), QCoreApplication::translate("MiernikPrzeplywu", "Stanowisko 1000x2000 lewe", nullptr));
        errorWentylatorY->setText(QString());
        eStatusWiatrak->setText(QCoreApplication::translate("MiernikPrzeplywu", "Status :", nullptr));
        label->setText(QCoreApplication::translate("MiernikPrzeplywu", "Pozycja L", nullptr));
        lmmy->setText(QCoreApplication::translate("MiernikPrzeplywu", "mm", nullptr));
        errorWentylatorX->setText(QString());
        lStatusWiatrak->setText(QString());
        label_2->setText(QCoreApplication::translate("MiernikPrzeplywu", "mm", nullptr));
        errorWentylatorL->setText(QString());
        eY->setText(QCoreApplication::translate("MiernikPrzeplywu", "Pozycja Y", nullptr));
        lmmx->setText(QCoreApplication::translate("MiernikPrzeplywu", "mm", nullptr));
        eX->setText(QCoreApplication::translate("MiernikPrzeplywu", "Pozycja X", nullptr));
        label_3->setText(QCoreApplication::translate("MiernikPrzeplywu", "Dopuszczalna warto\305\233\304\207 w zakresie od 0 do 2200", nullptr));
        pbUstaw->setText(QCoreApplication::translate("MiernikPrzeplywu", "Ustaw", nullptr));
        pbZeruj->setText(QCoreApplication::translate("MiernikPrzeplywu", "Zeruj", nullptr));
        label_4->setText(QCoreApplication::translate("MiernikPrzeplywu", "Dopuszczalna warto\305\233\304\207 w zakresie od 800 do 5000", nullptr));
        label_5->setText(QCoreApplication::translate("MiernikPrzeplywu", "Dopuszczlalna warto\305\233\304\207 w zakresie od 0 do 200", nullptr));
        pbWybierzStanowisko->setText(QCoreApplication::translate("MiernikPrzeplywu", "Wybierz stanowisko", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_fun), QCoreApplication::translate("MiernikPrzeplywu", "Ustawienia wentylatora", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MiernikPrzeplywu: public Ui_MiernikPrzeplywu {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MIERNIKPRZEPLYWU_H
