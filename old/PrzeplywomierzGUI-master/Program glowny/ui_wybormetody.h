/********************************************************************************
** Form generated from reading UI file 'wybormetody.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WYBORMETODY_H
#define UI_WYBORMETODY_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_WyborMetody
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *gbMode;
    QHBoxLayout *horizontalLayout;
    QRadioButton *rb2700;
    QRadioButton *rb1000p;
    QRadioButton *rb1000l;
    QRadioButton *rbFunSet;
    QGroupBox *gbMethod;
    QFormLayout *formLayoutDataMethod;
    QRadioButton *rbfile;
    QFrame *frChoiceFile;
    QGridLayout *gridLayoutDataFile;
    QPushButton *pushButton;
    QLabel *selectedFile;
    QSpacerItem *horizontalSpacer;
    QLabel *iChoiceFile;
    QLabel *eChosenFile;
    QLabel *lChooseFileName;
    QRadioButton *rbmanual;
    QFrame *frManual;
    QVBoxLayout *verticalLayout_3;
    QLabel *iManual;
    QFormLayout *formLayout;
    QLabel *eCzasManual;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *timeManualDefault;
    QLabel *label_minmaxTimeman;
    QFormLayout *formLayout_3;
    QLabel *eNumberManual;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *numberManual;
    QLabel *label_minmaxPosman;
    QRadioButton *rbhalfmanual;
    QFrame *frHalfManual;
    QVBoxLayout *verticalLayout_2;
    QLabel *iHalfmanual;
    QFormLayout *formLayout_4;
    QLabel *eCzasHalf;
    QHBoxLayout *horizontalLayout_4;
    QLineEdit *timeAuto;
    QLabel *label;
    QFormLayout *formLayout_2;
    QLabel *eNumberSquere;
    QHBoxLayout *horizontalLayout_7;
    QLineEdit *numberAuto_2;
    QLabel *label_2;
    QFormLayout *formLayout_5;
    QLabel *eNumberSquere_2;
    QHBoxLayout *horizontalLayout_6;
    QLineEdit *numberAuto;
    QLabel *label_3;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *WyborMetody)
    {
        if (WyborMetody->objectName().isEmpty())
            WyborMetody->setObjectName(QString::fromUtf8("WyborMetody"));
        WyborMetody->resize(983, 780);
        WyborMetody->setMinimumSize(QSize(980, 780));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/prefix1/cnbop.ico"), QSize(), QIcon::Normal, QIcon::Off);
        WyborMetody->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(WyborMetody);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gbMode = new QGroupBox(WyborMetody);
        gbMode->setObjectName(QString::fromUtf8("gbMode"));
        horizontalLayout = new QHBoxLayout(gbMode);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        rb2700 = new QRadioButton(gbMode);
        rb2700->setObjectName(QString::fromUtf8("rb2700"));

        horizontalLayout->addWidget(rb2700);

        rb1000p = new QRadioButton(gbMode);
        rb1000p->setObjectName(QString::fromUtf8("rb1000p"));

        horizontalLayout->addWidget(rb1000p);

        rb1000l = new QRadioButton(gbMode);
        rb1000l->setObjectName(QString::fromUtf8("rb1000l"));

        horizontalLayout->addWidget(rb1000l);

        rbFunSet = new QRadioButton(gbMode);
        rbFunSet->setObjectName(QString::fromUtf8("rbFunSet"));

        horizontalLayout->addWidget(rbFunSet);


        verticalLayout->addWidget(gbMode);

        gbMethod = new QGroupBox(WyborMetody);
        gbMethod->setObjectName(QString::fromUtf8("gbMethod"));
        gbMethod->setFlat(false);
        formLayoutDataMethod = new QFormLayout(gbMethod);
        formLayoutDataMethod->setObjectName(QString::fromUtf8("formLayoutDataMethod"));
        rbfile = new QRadioButton(gbMethod);
        rbfile->setObjectName(QString::fromUtf8("rbfile"));

        formLayoutDataMethod->setWidget(0, QFormLayout::LabelRole, rbfile);

        frChoiceFile = new QFrame(gbMethod);
        frChoiceFile->setObjectName(QString::fromUtf8("frChoiceFile"));
        frChoiceFile->setFrameShape(QFrame::StyledPanel);
        frChoiceFile->setFrameShadow(QFrame::Raised);
        gridLayoutDataFile = new QGridLayout(frChoiceFile);
        gridLayoutDataFile->setObjectName(QString::fromUtf8("gridLayoutDataFile"));
        pushButton = new QPushButton(frChoiceFile);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayoutDataFile->addWidget(pushButton, 1, 2, 1, 1);

        selectedFile = new QLabel(frChoiceFile);
        selectedFile->setObjectName(QString::fromUtf8("selectedFile"));

        gridLayoutDataFile->addWidget(selectedFile, 4, 1, 1, 2);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayoutDataFile->addItem(horizontalSpacer, 1, 1, 1, 1);

        iChoiceFile = new QLabel(frChoiceFile);
        iChoiceFile->setObjectName(QString::fromUtf8("iChoiceFile"));
        iChoiceFile->setTextFormat(Qt::RichText);
        iChoiceFile->setScaledContents(true);
        iChoiceFile->setAlignment(Qt::AlignJustify|Qt::AlignVCenter);
        iChoiceFile->setWordWrap(true);

        gridLayoutDataFile->addWidget(iChoiceFile, 0, 0, 1, 3);

        eChosenFile = new QLabel(frChoiceFile);
        eChosenFile->setObjectName(QString::fromUtf8("eChosenFile"));
        eChosenFile->setWordWrap(true);

        gridLayoutDataFile->addWidget(eChosenFile, 3, 0, 1, 1);

        lChooseFileName = new QLabel(frChoiceFile);
        lChooseFileName->setObjectName(QString::fromUtf8("lChooseFileName"));

        gridLayoutDataFile->addWidget(lChooseFileName, 3, 1, 1, 2);


        formLayoutDataMethod->setWidget(0, QFormLayout::FieldRole, frChoiceFile);

        rbmanual = new QRadioButton(gbMethod);
        rbmanual->setObjectName(QString::fromUtf8("rbmanual"));

        formLayoutDataMethod->setWidget(1, QFormLayout::LabelRole, rbmanual);

        frManual = new QFrame(gbMethod);
        frManual->setObjectName(QString::fromUtf8("frManual"));
        frManual->setFrameShape(QFrame::StyledPanel);
        frManual->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(frManual);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        iManual = new QLabel(frManual);
        iManual->setObjectName(QString::fromUtf8("iManual"));
        iManual->setAlignment(Qt::AlignJustify|Qt::AlignVCenter);
        iManual->setWordWrap(true);

        verticalLayout_3->addWidget(iManual);

        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setHorizontalSpacing(0);
        formLayout->setVerticalSpacing(0);
        eCzasManual = new QLabel(frManual);
        eCzasManual->setObjectName(QString::fromUtf8("eCzasManual"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(eCzasManual->sizePolicy().hasHeightForWidth());
        eCzasManual->setSizePolicy(sizePolicy);
        eCzasManual->setMinimumSize(QSize(230, 0));

        formLayout->setWidget(0, QFormLayout::LabelRole, eCzasManual);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        timeManualDefault = new QLineEdit(frManual);
        timeManualDefault->setObjectName(QString::fromUtf8("timeManualDefault"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(timeManualDefault->sizePolicy().hasHeightForWidth());
        timeManualDefault->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(timeManualDefault);

        label_minmaxTimeman = new QLabel(frManual);
        label_minmaxTimeman->setObjectName(QString::fromUtf8("label_minmaxTimeman"));
        sizePolicy1.setHeightForWidth(label_minmaxTimeman->sizePolicy().hasHeightForWidth());
        label_minmaxTimeman->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(label_minmaxTimeman);


        formLayout->setLayout(0, QFormLayout::FieldRole, horizontalLayout_2);


        verticalLayout_3->addLayout(formLayout);

        formLayout_3 = new QFormLayout();
        formLayout_3->setObjectName(QString::fromUtf8("formLayout_3"));
        formLayout_3->setHorizontalSpacing(0);
        formLayout_3->setVerticalSpacing(0);
        eNumberManual = new QLabel(frManual);
        eNumberManual->setObjectName(QString::fromUtf8("eNumberManual"));
        sizePolicy.setHeightForWidth(eNumberManual->sizePolicy().hasHeightForWidth());
        eNumberManual->setSizePolicy(sizePolicy);
        eNumberManual->setMinimumSize(QSize(230, 0));

        formLayout_3->setWidget(0, QFormLayout::LabelRole, eNumberManual);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        numberManual = new QLineEdit(frManual);
        numberManual->setObjectName(QString::fromUtf8("numberManual"));
        sizePolicy1.setHeightForWidth(numberManual->sizePolicy().hasHeightForWidth());
        numberManual->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(numberManual);

        label_minmaxPosman = new QLabel(frManual);
        label_minmaxPosman->setObjectName(QString::fromUtf8("label_minmaxPosman"));
        sizePolicy1.setHeightForWidth(label_minmaxPosman->sizePolicy().hasHeightForWidth());
        label_minmaxPosman->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(label_minmaxPosman);


        formLayout_3->setLayout(0, QFormLayout::FieldRole, horizontalLayout_3);


        verticalLayout_3->addLayout(formLayout_3);


        formLayoutDataMethod->setWidget(1, QFormLayout::FieldRole, frManual);

        rbhalfmanual = new QRadioButton(gbMethod);
        rbhalfmanual->setObjectName(QString::fromUtf8("rbhalfmanual"));

        formLayoutDataMethod->setWidget(2, QFormLayout::LabelRole, rbhalfmanual);

        frHalfManual = new QFrame(gbMethod);
        frHalfManual->setObjectName(QString::fromUtf8("frHalfManual"));
        frHalfManual->setFrameShape(QFrame::StyledPanel);
        frHalfManual->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frHalfManual);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        iHalfmanual = new QLabel(frHalfManual);
        iHalfmanual->setObjectName(QString::fromUtf8("iHalfmanual"));
        iHalfmanual->setAlignment(Qt::AlignJustify|Qt::AlignVCenter);
        iHalfmanual->setWordWrap(true);

        verticalLayout_2->addWidget(iHalfmanual);

        formLayout_4 = new QFormLayout();
        formLayout_4->setObjectName(QString::fromUtf8("formLayout_4"));
        eCzasHalf = new QLabel(frHalfManual);
        eCzasHalf->setObjectName(QString::fromUtf8("eCzasHalf"));
        sizePolicy.setHeightForWidth(eCzasHalf->sizePolicy().hasHeightForWidth());
        eCzasHalf->setSizePolicy(sizePolicy);
        eCzasHalf->setMinimumSize(QSize(220, 0));

        formLayout_4->setWidget(0, QFormLayout::LabelRole, eCzasHalf);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        timeAuto = new QLineEdit(frHalfManual);
        timeAuto->setObjectName(QString::fromUtf8("timeAuto"));
        sizePolicy1.setHeightForWidth(timeAuto->sizePolicy().hasHeightForWidth());
        timeAuto->setSizePolicy(sizePolicy1);

        horizontalLayout_4->addWidget(timeAuto);

        label = new QLabel(frHalfManual);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_4->addWidget(label);


        formLayout_4->setLayout(0, QFormLayout::FieldRole, horizontalLayout_4);


        verticalLayout_2->addLayout(formLayout_4);

        formLayout_2 = new QFormLayout();
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        eNumberSquere = new QLabel(frHalfManual);
        eNumberSquere->setObjectName(QString::fromUtf8("eNumberSquere"));
        sizePolicy.setHeightForWidth(eNumberSquere->sizePolicy().hasHeightForWidth());
        eNumberSquere->setSizePolicy(sizePolicy);
        eNumberSquere->setMinimumSize(QSize(220, 0));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, eNumberSquere);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        numberAuto_2 = new QLineEdit(frHalfManual);
        numberAuto_2->setObjectName(QString::fromUtf8("numberAuto_2"));
        sizePolicy1.setHeightForWidth(numberAuto_2->sizePolicy().hasHeightForWidth());
        numberAuto_2->setSizePolicy(sizePolicy1);

        horizontalLayout_7->addWidget(numberAuto_2);

        label_2 = new QLabel(frHalfManual);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_7->addWidget(label_2);


        formLayout_2->setLayout(0, QFormLayout::FieldRole, horizontalLayout_7);


        verticalLayout_2->addLayout(formLayout_2);

        formLayout_5 = new QFormLayout();
        formLayout_5->setObjectName(QString::fromUtf8("formLayout_5"));
        eNumberSquere_2 = new QLabel(frHalfManual);
        eNumberSquere_2->setObjectName(QString::fromUtf8("eNumberSquere_2"));
        sizePolicy.setHeightForWidth(eNumberSquere_2->sizePolicy().hasHeightForWidth());
        eNumberSquere_2->setSizePolicy(sizePolicy);
        eNumberSquere_2->setMinimumSize(QSize(220, 0));

        formLayout_5->setWidget(0, QFormLayout::LabelRole, eNumberSquere_2);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        numberAuto = new QLineEdit(frHalfManual);
        numberAuto->setObjectName(QString::fromUtf8("numberAuto"));
        sizePolicy1.setHeightForWidth(numberAuto->sizePolicy().hasHeightForWidth());
        numberAuto->setSizePolicy(sizePolicy1);

        horizontalLayout_6->addWidget(numberAuto);

        label_3 = new QLabel(frHalfManual);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_6->addWidget(label_3);


        formLayout_5->setLayout(0, QFormLayout::FieldRole, horizontalLayout_6);


        verticalLayout_2->addLayout(formLayout_5);


        formLayoutDataMethod->setWidget(2, QFormLayout::FieldRole, frHalfManual);


        verticalLayout->addWidget(gbMethod);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        buttonBox = new QDialogButtonBox(WyborMetody);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(WyborMetody);
        QObject::connect(buttonBox, SIGNAL(accepted()), WyborMetody, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), WyborMetody, SLOT(reject()));

        QMetaObject::connectSlotsByName(WyborMetody);
    } // setupUi

    void retranslateUi(QDialog *WyborMetody)
    {
        WyborMetody->setWindowTitle(QCoreApplication::translate("WyborMetody", "Pomiar przep\305\202ywno\305\233ci - Wyb\303\263r metody wprowadzania danych", nullptr));
        gbMode->setTitle(QCoreApplication::translate("WyborMetody", "Wybierz tryb pracy", nullptr));
        rb2700->setText(QCoreApplication::translate("WyborMetody", "Stanowisko\n"
" &2700x3000 [mm]", nullptr));
        rb1000p->setText(QCoreApplication::translate("WyborMetody", "Stanowisko\n"
"&1000x2000 [mm]\n"
"prawe", nullptr));
        rb1000l->setText(QCoreApplication::translate("WyborMetody", "Stanow&isko\n"
"1000x2000 [mm]\n"
"lewe", nullptr));
        rbFunSet->setText(QCoreApplication::translate("WyborMetody", "Tryb ustawiania \n"
"wentylatora", nullptr));
        gbMethod->setTitle(QCoreApplication::translate("WyborMetody", "Wybierz metod\304\231 wprowadznia danych", nullptr));
        rbfile->setText(QCoreApplication::translate("WyborMetody", "Dane z pliku", nullptr));
        pushButton->setText(QCoreApplication::translate("WyborMetody", "Wybierz plik", nullptr));
        selectedFile->setText(QString());
        iChoiceFile->setText(QCoreApplication::translate("WyborMetody", "W wybranym pliku dane od siebie powinny by\304\207 odzielone od siebie przecinkiem, a kolejne pozycje zaczyna\304\207 si\304\231 od nowej linii. Na pierwszym miejscu jest szeroko\305\233\304\207 w milimetrach, na drugim wysoko\305\233\304\207 milimetrach, a na trzecim miejscu czas w sekundach.", nullptr));
        eChosenFile->setText(QCoreApplication::translate("WyborMetody", "Wybrany plik:", nullptr));
        lChooseFileName->setText(QString());
        rbmanual->setText(QCoreApplication::translate("WyborMetody", "Dane &wprowadzane\n"
"r\304\231cznie", nullptr));
        iManual->setText(QCoreApplication::translate("WyborMetody", "W kolejnym kroku nale\305\274y wype\305\202ni\304\207 tabel\304\231, gdzie w pierwszej kolumnie nale\305\274y poda\304\207 szeroko\305\233\304\207 w mm, w drugiej wysoko\305\233\304\207 w mm, a w trzeciej czas w sekundach. Czas okre\305\233la ile czasu na danej pozycji b\304\231dzie ustawiona karetka z czujnikiem.", nullptr));
        eCzasManual->setText(QCoreApplication::translate("WyborMetody", "Domy\305\233lny czas postoju karetki [s]:", nullptr));
        label_minmaxTimeman->setText(QCoreApplication::translate("WyborMetody", "Warto\305\233\304\207 od 1 do 3600", nullptr));
        eNumberManual->setText(QCoreApplication::translate("WyborMetody", "Ilo\305\233\304\207 pozycji :", nullptr));
        label_minmaxPosman->setText(QCoreApplication::translate("WyborMetody", "Warto\305\233\304\207 od 1 do 1000", nullptr));
        rbhalfmanual->setText(QCoreApplication::translate("WyborMetody", "Dane wybierane \n"
"z p\305\202aszczyzny", nullptr));
        iHalfmanual->setText(QCoreApplication::translate("WyborMetody", "W kolejnym kroku nale\305\274y wybra\304\207 pozycje, klikaj\304\205c na odpowiedni kwadrat z na oknie, wtedy zostanie dodane do danych \305\233rodek tego kwadratu przeliczony na warto\305\233ci w milimetrach. Czas postoju dla ka\305\274dej wybranej pozycji b\304\231dzie taki sam. Czas ten nale\305\274y wybra\304\207 tutaj. Kolejny parametr to na ile kwadrat\303\263w podzieli\304\207 jeden bok p\305\202aszczyzny operacyjnej", nullptr));
        eCzasHalf->setText(QCoreApplication::translate("WyborMetody", "Domy\305\233lny czas postoju karetki [s]: ", nullptr));
        label->setText(QCoreApplication::translate("WyborMetody", "Warto\305\233\304\207 od 1 do 3500", nullptr));
        eNumberSquere->setText(QCoreApplication::translate("WyborMetody", "Ilo\305\233\304\207 kwadrat\303\263w na osi X :", nullptr));
        label_2->setText(QCoreApplication::translate("WyborMetody", "Warto\305\233c od 1 do 80", nullptr));
        eNumberSquere_2->setText(QCoreApplication::translate("WyborMetody", "Ilo\305\233\304\207 kwadrat\303\263w na osi Y:", nullptr));
        label_3->setText(QCoreApplication::translate("WyborMetody", "Warto\305\233\304\207 od 1 do 80", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WyborMetody: public Ui_WyborMetody {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WYBORMETODY_H
