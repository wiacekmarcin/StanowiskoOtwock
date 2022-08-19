#include "wybormetody.h"
#include "ui_wybormetody.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>

WyborMetody::WyborMetody(QWidget *parent, ModeWork mode) :
    QDialog(parent),
    ui(new Ui::WyborMetody),
    wbMetoda(METHOD_NONE),
    timeStopManual(0),
    timeStopAuto(0),
    numberPozMan(0),
    numberWidth(0),
    numberHeight(0)
{
    ui->setupUi(this);
    on_rbfile_toggled(true);
    on_rbmanual_toggled(false);
    on_rbhalfmanual_toggled(false);
    setEnabledContinue(false);
    ui->gbMethod->setEnabled(false);
    ui->rb2700->setChecked(true);
    ui->rbfile->setChecked(true);
    switch(mode) {
        case MODE_2700:
            ui->rb2700->setChecked(true);
            ui->rb1000p->setChecked(false);
            ui->rb1000l->setChecked(false);
            ui->rbFunSet->setChecked(false);

        break;
        case MODE_1000P:
            ui->rb2700->setChecked(false);
            ui->rb1000p->setChecked(true);
            ui->rb1000l->setChecked(false);
            ui->rbFunSet->setChecked(false);

        break;
        case MODE_1000L:
            ui->rb2700->setChecked(false);
            ui->rb1000p->setChecked(false);
            ui->rb1000l->setChecked(true);
            ui->rbFunSet->setChecked(false);

        break;
        case MODE_FUNSET:
            ui->rb2700->setChecked(false);
            ui->rb1000p->setChecked(false);
            ui->rb1000l->setChecked(false);
            ui->rbFunSet->setChecked(true);

        break;
        case MODE_SERVICE:
            ui->rb2700->setChecked(false);
            ui->rb1000p->setChecked(false);
            ui->rb1000l->setChecked(false);
            ui->rbFunSet->setChecked(false);

        break;
    default:
        break;
    }
    startWindow = parent == nullptr;
}

WyborMetody::~WyborMetody()
{
    delete ui;
}

void WyborMetody::on_rbfile_toggled(bool checked)
{
    ui->frChoiceFile->setDisabled(!checked);
    if (checked) {
        wbMetoda = METHOD_FILE;
    } else {
        return;
    }


}

void WyborMetody::on_rbmanual_toggled(bool checked)
{
    ui->frManual->setDisabled(!checked);
    if (checked) {
        short ok = 0;
        if (isValidNumber(ui->numberManual, 1000)) {
            numberPozMan = ui->numberManual->text().toUInt();
            ++ok;
        }
        if (isValidTime(ui->timeManualDefault)) {
            timeStopManual = ui->timeManualDefault->text().toUInt();
            ++ok;
        }
        setEnabledContinue(ok == 2);
        wbMetoda = METHOD_MANUAL;
    } else {
        return;
    }

}

void WyborMetody::on_rbhalfmanual_toggled(bool checked)
{
    ui->frHalfManual->setDisabled(!checked);
    if (checked) {
        short ok = 0;
        if (isValidNumber(ui->numberAuto, 20)) {
            numberWidth = ui->numberAuto->text().toUInt();
            ++ok;
        }
        if (isValidNumber(ui->numberAuto_2, 20)) {
            numberHeight = ui->numberAuto_2->text().toUInt();
            ++ok;
        }
        if (isValidTime(ui->timeAuto)) {
            timeStopAuto = ui->timeAuto->text().toUInt();
            ++ok;
        }
        setEnabledContinue(ok == 3);
        wbMetoda = METHOD_SQUERE;
    } else {
        return;
    }
}

void WyborMetody::on_pushButton_clicked()
{
    chooseFileName();

}

void WyborMetody::on_pushButton_pressed()
{
    chooseFileName();
}

void WyborMetody::chooseFileName()
{
    QString file = QFileDialog::getOpenFileName(this, QCoreApplication::translate("WyborMetody", "Wybierz plik z danymi", nullptr),
                                                QStandardPaths::displayName(QStandardPaths::DocumentsLocation));
    if (file.isEmpty()) {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled( false );
        return;
    }
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled( true );
    fileName = file;
    ui->lChooseFileName->setText(file);
}

void WyborMetody::setEnabledContinue(bool enabled)
{
    //TODO
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled( enabled );
}

bool WyborMetody::isValidNumber(QLineEdit *number, int maxPosition)
{
    QString sn = number->text();
    bool ok;
    int isn = sn.toInt(&ok);
    if (!ok) {
        return false;
    }
    if (isn <= 0 || isn > maxPosition) {
        return false;
    }
    return true;
}

bool WyborMetody::isValidTime(QLineEdit *time)
{
    QString st = time->text();
    bool ok;
    int ist = st.toInt(&ok);
    if (!ok) {
        return false;
    }
    if (ist < minTime || ist > maxTime) {
        return false;
    }
    return true;
}

bool WyborMetody::isValidAutoParameters()
{
    return isValidNumber(ui->numberAuto, 80) &&
           isValidNumber(ui->numberAuto_2, 80) &&
           isValidTime(ui->timeAuto);
}

bool WyborMetody::isValidManualParameters()
{
    return isValidNumber(ui->numberManual, 1000) && isValidTime(ui->timeManualDefault);
}


WyborMetody::MethodInsData WyborMetody::getWbMetoda() const
{
    return wbMetoda;
}

WyborMetody::ModeWork WyborMetody::getWbMode() const
{
    return wbMode;
}

void WyborMetody::on_rb2700_toggled(bool checked)
{
    if (checked) {
        ui->gbMethod->setEnabled(true);
        wbMode = MODE_2700;
    }
}

void WyborMetody::on_rb1000l_toggled(bool checked)
{
    if (checked) {
        ui->gbMethod->setEnabled(true);
        wbMode = MODE_1000L;
    }
}

void WyborMetody::on_rb1000p_toggled(bool checked)
{
    if (checked) {
        ui->gbMethod->setEnabled(true);
        wbMode = MODE_1000P;
    }
}

void WyborMetody::on_rbFunSet_toggled(bool checked)
{
    if (checked) {
        ui->gbMethod->setEnabled(false);
        wbMode = MODE_FUNSET;
        setEnabledContinue(true);
    }
}

void WyborMetody::on_timeManualDefault_editingFinished()
{
    if (!isValidTime(ui->timeManualDefault)) {
        setEnabledContinue(false);
        return;
    }
    timeStopManual = ui->timeManualDefault->text().toUInt();
    setEnabledContinue(isValidManualParameters());
}

void WyborMetody::on_buttonBox_rejected()
{
    QMessageBox::StandardButton resBtn = QMessageBox::Yes;
    resBtn = QMessageBox::question( this, "Przeplywomierz",
                                        tr("Czy jesteś pewny?\n"),
                                        QMessageBox::No | QMessageBox::Yes,
                                        QMessageBox::Yes);

    if (resBtn == QMessageBox::Yes) {
        QCoreApplication::exit(0);
    } else {
        //QDialog::accept();
    }
}

void WyborMetody::reject()
{
    on_buttonBox_rejected();

}

unsigned int WyborMetody::getNumberHeight() const
{
    return numberHeight;
}

unsigned int WyborMetody::getNumberWidth() const
{
    return numberWidth;
}

unsigned int WyborMetody::getNumberPozMan() const
{
    return numberPozMan;
}

unsigned int WyborMetody::getTimeStopAuto() const
{
    return timeStopAuto;
}

unsigned int WyborMetody::getTimeStopManual() const
{
    return timeStopManual;
}

QString WyborMetody::getFileName() const
{
    return fileName;
}

void WyborMetody::on_timeAuto_editingFinished()
{
    if (!isValidTime(ui->timeAuto)){
        setEnabledContinue(false);
        return;
    }
    timeStopAuto = ui->timeAuto->text().toUInt();
    setEnabledContinue(isValidAutoParameters());
}

void WyborMetody::on_numberAuto_editingFinished()
{
    if (!isValidNumber(ui->numberAuto, 80)) {
        setEnabledContinue(false);
        return;
    }
    numberHeight = ui->numberAuto->text().toUInt();
    setEnabledContinue(isValidAutoParameters());
}

void WyborMetody::on_numberAuto_2_editingFinished()
{
    if (!isValidNumber(ui->numberAuto_2, 80)) {
        setEnabledContinue(false);
        return;
    }
    numberWidth = ui->numberAuto_2->text().toUInt();
    setEnabledContinue(isValidAutoParameters());
}


void WyborMetody::on_timeAuto_textChanged(const QString &)
{
    if (!isValidTime(ui->timeAuto)) {
        setEnabledContinue(false);
        return;
    }
    timeStopAuto = ui->timeAuto->text().toUInt();
    setEnabledContinue(isValidAutoParameters());
}

void WyborMetody::on_numberAuto_textChanged(const QString &)
{
    if (!isValidNumber(ui->numberAuto, 80)) {
        setEnabledContinue(false);
        return;
    }
    numberHeight = ui->numberAuto->text().toUInt();
    setEnabledContinue(isValidAutoParameters());
}

void WyborMetody::on_numberAuto_2_textChanged(const QString &)
{
    if (!isValidNumber(ui->numberAuto_2, 80)) {
        setEnabledContinue(false);
        return;
    }
    numberWidth = ui->numberAuto_2->text().toUInt();
    setEnabledContinue(isValidAutoParameters());;
}

void WyborMetody::on_timeManualDefault_textChanged(const QString &)
{
    if (!isValidTime(ui->timeManualDefault)) {
        setEnabledContinue(false);
        return;
    }
    timeStopManual = ui->timeManualDefault->text().toUInt();
    setEnabledContinue(isValidManualParameters());
}

void WyborMetody::on_numberManual_textChanged(const QString &)
{
    if (!isValidNumber(ui->numberManual, 1000)) {
        setEnabledContinue(false);
        return;
    }
    numberPozMan = ui->numberManual->text().toUInt();
    setEnabledContinue(isValidManualParameters());
}
