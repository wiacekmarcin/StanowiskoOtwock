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



    switch(mode) {
        case MODE_2700:
            ui->rb2700->setChecked(true);
            ui->rb1000p->setChecked(false);
            ui->rb1000l->setChecked(false);
            ui->rbFunSet->setChecked(false);
            ui->rbRoleta->setChecked(false);
            visibleOther(true);
            visibleRoleta(false);
        break;
        case MODE_1000P:
            ui->rb2700->setChecked(false);
            ui->rb1000p->setChecked(true);
            ui->rb1000l->setChecked(false);
            ui->rbFunSet->setChecked(false);
            ui->rbRoleta->setChecked(false);
            visibleOther(true);
            visibleRoleta(false);
        break;
        case MODE_1000L:
            ui->rb2700->setChecked(false);
            ui->rb1000p->setChecked(false);
            ui->rb1000l->setChecked(true);
            ui->rbFunSet->setChecked(false);
            visibleOther(true);
            visibleRoleta(false);
        break;
        case MODE_FUNSET:
            visibleOther(false);
            visibleRoleta(false);
        break;
        case MODE_SERVICE:
            visibleOther(false);
            visibleRoleta(false);
        break;
        case MODE_ROLETA:
            ui->rb2700->setChecked(false);
            ui->rb1000p->setChecked(false);
            ui->rb1000l->setChecked(false);
            ui->rbFunSet->setChecked(false);
            ui->rbRoleta->setChecked(true);
            visibleOther(false);
            visibleRoleta(true);
        break;
        case MODE_ROLETAPLIK:
            ui->rb2700->setChecked(false);
            ui->rb1000p->setChecked(false);
            ui->rb1000l->setChecked(false);
            ui->rbFunSet->setChecked(false);
            ui->rbRoleta->setChecked(true);
            visibleOther(false);
            visibleRoleta(true);
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
    ui->frHalfManual->setDisabled(checked);
    ui->frManual->setDisabled(checked);
    if (checked) {
        wbMetoda = METHOD_FILE;
    } else {
        return;
    }


}

void WyborMetody::on_rbmanual_toggled(bool checked)
{
    ui->frManual->setDisabled(!checked);
    ui->frHalfManual->setDisabled(checked);
    ui->frChoiceFile->setDisabled(checked);
    if (checked) {
        short ok = 0;
        if (isValidNumber(ui->numberManual)) {
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
    ui->frManual->setDisabled(checked);
    ui->frChoiceFile->setDisabled(checked);
    if (checked) {
        short ok = 0;
        if (isValidNumber(ui->numberAuto)) {
            numberWidth = ui->numberAuto->text().toUInt();
            ++ok;
        }
        if (isValidNumber(ui->numberAuto_2)) {
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

bool WyborMetody::isValidNumber(QLineEdit *number)
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

bool WyborMetody::isValidNumberRolety(QLineEdit *number)
{
    QString sn = number->text();
    bool ok;
    int isn = sn.toInt(&ok);
    if (!ok) {
        return false;
    }
    if (isn <= 0 || isn > maxRolet) {
        return false;
    }
    return true;
}

bool WyborMetody::isValidPostojRolety(QLineEdit *number)
{
    QString sn = number->text();
    bool ok;
    int isn = sn.toInt(&ok);
    if (!ok) {
        return false;
    }
    if (isn <= 0 || isn > maxPostoj) {
        return false;
    }
    return true;
}

void WyborMetody::visibleRoleta(bool visible)
{
    ui->rbRoletaDane->setVisible(visible);
    ui->rbRoletaPlik->setVisible(visible);
    ui->frRoleta->setVisible(visible);
    ui->frRoletaPlik->setVisible(visible);
    resize(minimumSizeHint());
}

void WyborMetody::visibleOther(bool visible)
{
    ui->rbfile->setVisible(visible);
    ui->rbhalfmanual->setVisible(visible);
    ui->rbmanual->setVisible(visible);
    ui->frChoiceFile->setVisible(visible);
    ui->frHalfManual->setVisible(visible);
    ui->frManual->setVisible(visible);
    resize(minimumSizeHint());
}

QString WyborMetody::getFileName2() const
{
    return fileName2;
}

unsigned int WyborMetody::getPartRolet() const
{
    return partRolet;
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
    return isValidNumber(ui->numberAuto) &&
           isValidNumber(ui->numberAuto_2) &&
           isValidTime(ui->timeAuto);
}

bool WyborMetody::isValidManualParameters()
{
    return isValidNumber(ui->numberManual) && isValidTime(ui->timeManualDefault);
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
        visibleOther(true);
        visibleRoleta(false);
        wbMode = MODE_2700;
    }
}

void WyborMetody::on_rb1000l_toggled(bool checked)
{
    if (checked) {
        visibleOther(true);
        visibleRoleta(false);
        wbMode = MODE_1000L;
    }
}

void WyborMetody::on_rb1000p_toggled(bool checked)
{
    if (checked) {
        visibleOther(true);
        visibleRoleta(false);
        wbMode = MODE_1000P;
    }
}

void WyborMetody::on_rbFunSet_toggled(bool checked)
{
    if (checked) {
        visibleOther(false);
        visibleRoleta(false);
        wbMode = MODE_FUNSET;
        setEnabledContinue(true);
    }
}

void WyborMetody::on_rbRoleta_toggled(bool checked)
{
    if (checked) {
        wbMode = MODE_ROLETA;
        visibleOther(false);
        visibleRoleta(true);
        setEnabledContinue(false);
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
    if (startWindow)
        QApplication::quit();
}

void WyborMetody::reject()
{
    QMessageBox::StandardButton resBtn = QMessageBox::Yes;
    resBtn = QMessageBox::question( this, "Przeplywomierz",
                                        tr("Czy jesteś pewny?\n"),
                                        QMessageBox::No | QMessageBox::Yes,
                                        QMessageBox::Yes);

    if (resBtn == QMessageBox::Yes) {
        QDialog::reject();
    }
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
    if (isValidTime(ui->timeAuto)){
        setEnabledContinue(false);
        return;
    }
    timeStopAuto = ui->timeAuto->text().toUInt();
    setEnabledContinue(isValidAutoParameters());
}

void WyborMetody::on_numberAuto_editingFinished()
{
    if (!isValidNumber(ui->numberAuto)) {
        setEnabledContinue(false);
        return;
    }
    numberHeight = ui->numberAuto->text().toUInt();
    setEnabledContinue(isValidAutoParameters());
}

void WyborMetody::on_numberAuto_2_editingFinished()
{
    if (!isValidNumber(ui->numberAuto_2)) {
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
    if (!isValidNumber(ui->numberAuto)) {
        setEnabledContinue(false);
        return;
    }
    numberHeight = ui->numberAuto->text().toUInt();
    setEnabledContinue(isValidAutoParameters());
}

void WyborMetody::on_numberAuto_2_textChanged(const QString &)
{
    if (!isValidNumber(ui->numberAuto_2)) {
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
    if (!isValidNumber(ui->numberManual)) {
        setEnabledContinue(false);
        return;
    }
    numberPozMan = ui->numberManual->text().toUInt();
    setEnabledContinue(isValidManualParameters());
}



void WyborMetody::on_lineEdit_textChanged(const QString &arg1)
{
    if (!isValidNumberRolety(ui->lineEdit)) {
        setEnabledContinue(false);
        return;
    }
    partRolet = arg1.toUInt();
    setEnabledContinue(true);
}

void WyborMetody::on_postojEtap_textChanged(const QString &arg1)
{
    if (!isValidPostojRolety(ui->lineEdit)) {
        setEnabledContinue(false);
        return;
    }
    stableTimeCzas = arg1.toUInt();
    setEnabledContinue(true);
}

void WyborMetody::on_rbRoletaDane_toggled(bool checked)
{
    ui->rbRoletaDane->setChecked(checked);
    ui->rbRoletaPlik->setChecked(!checked);
    ui->frRoleta->setDisabled(!checked);
    ui->frRoletaPlik->setDisabled(checked);
    wbMode = MODE_ROLETA;
}

void WyborMetody::on_rbRoletaPlik_toggled(bool checked)
{
    ui->rbRoletaDane->setChecked(!checked);
    ui->rbRoletaPlik->setChecked(checked);
    ui->frRoleta->setDisabled(checked);
    ui->frRoletaPlik->setDisabled(!checked);
    wbMode = MODE_ROLETAPLIK;
}

void WyborMetody::on_pbChooseFile_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, QCoreApplication::translate("WyborMetody", "Wybierz plik z danymi", nullptr),
                                                QStandardPaths::displayName(QStandardPaths::DocumentsLocation));
    if (file.isEmpty()) {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled( false );
        return;
    }
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled( true );
    fileName2 = file;
    ui->wybranyPlikRolety->setText(file);
}
