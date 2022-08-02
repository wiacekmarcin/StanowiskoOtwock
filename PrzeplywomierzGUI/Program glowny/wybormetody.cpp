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
            ui->gbMethod->setVisible(true);
        break;
        case MODE_1000P:
            ui->rb2700->setChecked(false);
            ui->rb1000p->setChecked(true);
            ui->rb1000l->setChecked(false);
            ui->rbFunSet->setChecked(false);
            ui->rbRoleta->setChecked(false);
            visibleOther(true);
            visibleRoleta(false);
            ui->gbMethod->setVisible(true);
        break;
        case MODE_1000L:
            ui->rb2700->setChecked(false);
            ui->rb1000p->setChecked(false);
            ui->rb1000l->setChecked(true);
            ui->rbFunSet->setChecked(false);
            visibleOther(true);
            visibleRoleta(false);
            ui->gbMethod->setVisible(true);
        break;
        case MODE_FUNSET:
            visibleOther(false);
            visibleRoleta(false);
            ui->gbMethod->setVisible(false);
            ui->gbMethod->setDisabled(true);
        break;
        case MODE_SERVICE:
            visibleOther(false);
            visibleRoleta(false);
            ui->gbMethod->setVisible(false);
        break;
        case MODE_ROLETA:
            ui->rb2700->setChecked(false);
            ui->rb1000p->setChecked(false);
            ui->rb1000l->setChecked(false);
            ui->rbFunSet->setChecked(false);
            ui->rbRoleta->setChecked(true);
            visibleOther(false);
            visibleRoleta(true);
            ui->gbMethod->setVisible(true);
        break;
        case MODE_ROLETAPLIK:
            ui->rb2700->setChecked(false);
            ui->rb1000p->setChecked(false);
            ui->rb1000l->setChecked(false);
            ui->rbFunSet->setChecked(false);
            ui->rbRoleta->setChecked(true);
            visibleOther(false);
            visibleRoleta(true);
            ui->gbMethod->setVisible(true);
    break;

    default:
        break;
    }
    startWindow = parent == nullptr;
    setEnabledContinue(false);
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

bool WyborMetody::isValidIloscRolety(QLineEdit *number)
{
    QString sn = number->text();
    bool ok;
    int isn = sn.toInt(&ok);
    if (!ok) {
        return false;
    }
    if (isn <= 0 || isn > maxTime) {
        return false;
    }
    return true;
}


void WyborMetody::visibleRoleta(bool visible)
{
    ui->frRoletaInfo->setVisible(visible);
    if (visible) {
        ui->pbNormaOffsetSave->setEnabled(false);
        ui->pbNormaOffsetChange->setEnabled(true);
        ui->normaOffsetXEdit->setVisible(false);
        ui->normaOffsetX->setVisible(true);
        ui->normaOffsetYEdit->setVisible(false);
        ui->normaOffsetY->setVisible(true);
    }
    ui->rbRoletaDane->setVisible(visible);
    ui->rbRoletaPlik->setVisible(visible);
    ui->frRoleta->setVisible(visible);
    ui->frRoletaPlik->setVisible(visible);
}

void WyborMetody::visibleOther(bool visible)
{
    ui->rbfile->setVisible(visible);
    ui->rbhalfmanual->setVisible(visible);
    ui->rbmanual->setVisible(visible);
    ui->frChoiceFile->setVisible(visible);
    ui->frHalfManual->setVisible(visible);
    ui->frManual->setVisible(visible);
}

bool WyborMetody::isValidRoletaRB()
{
    return ui->rbRoletaDane->isChecked() || ui->rbRoletaPlik->isChecked();
}

bool WyborMetody::isValidPlaszczynaRB()
{
    return ui->rbfile->isChecked() || ui->rbmanual->isChecked() || ui->rbhalfmanual->isChecked();
}

unsigned int WyborMetody::getTimeStopRoleta() const
{
    return timeStopRoleta;
}

QString WyborMetody::getFileName2() const
{
    return fileName2;
}

unsigned int WyborMetody::getPartRolet() const
{
    return etapNrRoleta;
}

unsigned int WyborMetody::getStableTimeCzas() const
{
    return stableTimeRoleta;
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
    return isValidPlaszczynaRB() &&
           isValidNumber(ui->numberAuto) &&
           isValidNumber(ui->numberAuto_2) &&
           isValidTime(ui->timeAuto);
}

bool WyborMetody::isValidManualParameters()
{
    return isValidPlaszczynaRB() && isValidNumber(ui->numberManual) && isValidTime(ui->timeManualDefault);
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
        setEnabledContinue(isValidNumberRolety(ui->normaEtapNumber) && isValidPostojRolety(ui->normaStabTime) && isValidIloscRolety(ui->NormaIloscProbek));
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

void WyborMetody::on_pbNormaOffsetChange_clicked()
{
    ui->pbNormaOffsetChange->setEnabled(false);
    ui->pbNormaOffsetSave->setEnabled(true);

    ui->normaOffsetXEdit->setVisible(true);
    ui->normaOffsetYEdit->setVisible(true);

    ui->normaOffsetXEdit->setText(ui->normaOffsetX->text());
    ui->normaOffsetYEdit->setText(ui->normaOffsetY->text());
}

void WyborMetody::on_pbNormaOffsetSave_clicked()
{
    ui->pbNormaOffsetChange->setEnabled(true);
    ui->pbNormaOffsetSave->setEnabled(false);

    ui->normaOffsetXEdit->setVisible(false);
    ui->normaOffsetYEdit->setVisible(false);

    bool ok;
    int val = ui->normaOffsetXEdit->text().toInt(&ok);
    if (ok && val >= 0)
        ui->normaOffsetX->setText(ui->normaOffsetXEdit->text());

    val = ui->normaOffsetYEdit->text().toInt(&ok);
    if (ok && val >= 0)
        ui->normaOffsetY->setText(ui->normaOffsetYEdit->text());
}


void WyborMetody::on_normaEtapNumber_textChanged(const QString &arg1)
{
    if (!isValidNumberRolety(ui->normaEtapNumber)) {
        setEnabledContinue(false);
        return;
    }
    etapNrRoleta = arg1.toUInt();
    setEnabledContinue(isValidRoletaRB() && isValidPostojRolety(ui->normaStabTime) && isValidIloscRolety(ui->NormaIloscProbek));
}


void WyborMetody::on_normaStabTime_textChanged(const QString &arg1)
{
    if (!isValidPostojRolety(ui->normaStabTime)) {
        setEnabledContinue(false);
        return;
    }
    stableTimeRoleta = arg1.toUInt();
    setEnabledContinue(isValidRoletaRB() && isValidNumberRolety(ui->normaEtapNumber) && isValidIloscRolety(ui->NormaIloscProbek));
}


void WyborMetody::on_NormaIloscProbek_textChanged(const QString &arg1)
{
    if (!isValidIloscRolety(ui->NormaIloscProbek)) {
        setEnabledContinue(false);
        return;
    }
    timeStopRoleta = arg1.toUInt();
    setEnabledContinue(isValidRoletaRB() && isValidNumberRolety(ui->normaEtapNumber) && isValidPostojRolety(ui->normaStabTime));
}



