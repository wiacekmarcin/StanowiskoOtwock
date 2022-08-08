#include "wybormetody.h"
#include "ui_wybormetody.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <QDebug>

WyborMetody::WyborMetody(QWidget *parent, ModeWork mode, MethodInsData ins) :
    QDialog(parent),
    ui(new Ui::WyborMetody),
    wbInsData(ins),
    wbMode(mode)

{
    data.timeStopManual = 0;
    data.timeStopAuto = 0;
    data.numberPozMan = 0;
    data.numberWidth = 0;
    data.numberHeight = 0;
    data.etapNrRoleta = 0;
    data.stableTimeRoleta = 0;
    data.timeStopRoleta = 0;
    data.fileName  = "";
    data.fileName2 = "";
    data.offsetX = 0;
    data.offsetY = 0;

    ui->setupUi(this);

    init();
}

WyborMetody::~WyborMetody()
{
    delete ui;
}

void WyborMetody::init()
{
    //qDebug() << __FILE__ << __LINE__ << __PRETTY_FUNCTION__ << wbInsData;
    on_rbfile_toggled(false);
    on_rbmanual_toggled(false);
    on_rbhalfmanual_toggled(false);
    setEnabledContinue(false);

    if (wbMode == MODE_NONE) {
        ui->rb2700->setChecked(true);
        ui->rbfile->setChecked(true);
        return;
    }

    //ui->gbMethod->setEnabled(false);

    switch(wbMode) {
        case MODE_2700:
            ui->rb2700->setChecked(true);
            ui->rb1000p->setChecked(false);
            ui->rb1000l->setChecked(false);
            ui->rbFunSet->setChecked(false);
            ui->rbRoleta->setChecked(false);
            visibleOther(true);
            visibleRoleta(false);
            ui->gbMethod->setVisible(true);
            initMethodPosition();
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
            initMethodPosition();
        break;
        case MODE_1000L:
            ui->rb2700->setChecked(false);
            ui->rb1000p->setChecked(false);
            ui->rb1000l->setChecked(true);
            ui->rbFunSet->setChecked(false);
            visibleOther(true);
            visibleRoleta(false);
            ui->gbMethod->setVisible(true);
            initMethodPosition();
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
    default:
        break;
    }
    setEnabledContinue(false);
}

void WyborMetody::initMethodPosition()
{
    //qDebug() << __FILE__ << __LINE__ << __PRETTY_FUNCTION__ << wbInsData;
    switch(wbInsData)
    {
    case METHOD_FILE:
        ui->rbmanual->setChecked(false);
        ui->rbhalfmanual->setChecked(false);
        ui->rbfile->setChecked(true);
        on_rbfile_toggled(true);
        break;
    case METHOD_MANUAL:
        ui->rbhalfmanual->setChecked(false);
        ui->rbfile->setChecked(false);
        ui->rbmanual->setChecked(true);
        on_rbmanual_toggled(true);
        break;
    case METHOD_SQUERE:
        ui->rbmanual->setChecked(false);
        ui->rbfile->setChecked(false);
        ui->rbhalfmanual->setChecked(true);
        on_rbhalfmanual_toggled(true);
        break;
    default:
        break;
    }
}


void WyborMetody::on_rbfile_toggled(bool checked)
{
    ui->frChoiceFile->setDisabled(!checked);
    ui->frHalfManual->setDisabled(checked);
    ui->frManual->setDisabled(checked);
    if (checked) {
        wbInsData = METHOD_FILE;
        setEnabledContinue(!getFileName().isEmpty());
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
        if (isValidNumber(ui->numberManual, 1000)) {
            data.numberPozMan = ui->numberManual->text().toUInt();
            ++ok;
        }
        if (isValidTime(ui->timeManualDefault)) {
            data.timeStopManual = ui->timeManualDefault->text().toUInt();
            ++ok;
        }
        setEnabledContinue(ok == 2);
        wbInsData = METHOD_MANUAL;
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
        if (isValidNumber(ui->numberAuto, 20)) {
            data.numberWidth = ui->numberAuto->text().toUInt();
            ++ok;
        }
        if (isValidNumber(ui->numberAuto_2, 20)) {
            data.numberHeight = ui->numberAuto_2->text().toUInt();
            ++ok;
        }
        if (isValidTime(ui->timeAuto)) {
            data.timeStopAuto = ui->timeAuto->text().toUInt();
            ++ok;
        }
        setEnabledContinue(ok == 3);
        wbInsData = METHOD_SQUERE;
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
    data.fileName = file;
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

bool WyborMetody::isValidNumberRolety(QLineEdit *number)
{
    QString sn = number->text();
    bool ok;
    int isn = sn.toInt(&ok);
    if (!ok) {
        return false;
    }
    if (isn < minRolet || isn > maxRolet) {
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

void WyborMetody::setData(const WyborMetodyData &newData)
{
    data = newData;
}

void WyborMetody::setWbMode(ModeWork newWbMode)
{
    wbMode = newWbMode;
}

void WyborMetody::setWbMetoda(MethodInsData newWbMetoda)
{
    wbInsData = newWbMetoda;
}

const WyborMetodyData &WyborMetody::getData() const
{
    return data;
}

unsigned int WyborMetody::getTimeStopRoleta() const
{
    return data.timeStopRoleta;
}

QString WyborMetody::getFileName2() const
{
    return data.fileName2;
}

unsigned int WyborMetody::getPartRolet() const
{
    return data.etapNrRoleta;
}

unsigned int WyborMetody::getStableTimeCzas() const
{
    return data.stableTimeRoleta;
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
           isValidNumber(ui->numberAuto, 80) &&
           isValidNumber(ui->numberAuto_2, 80) &&
           isValidTime(ui->timeAuto);
}

bool WyborMetody::isValidManualParameters()
{
    return isValidPlaszczynaRB() && isValidNumber(ui->numberManual, 1000) && isValidTime(ui->timeManualDefault);
}


WyborMetody::MethodInsData WyborMetody::getWbMetoda() const
{
    return wbInsData;
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
    data.timeStopManual = ui->timeManualDefault->text().toUInt();
    setEnabledContinue(isValidManualParameters());
}

void WyborMetody::reject()
{
    //qDebug() << __FILE__ << __LINE__ << __FUNCTION__;
    QMessageBox::StandardButton resBtn = QMessageBox::Yes;
    resBtn = QMessageBox::question( this, "Przeplywomierz",
                                        tr("Czy jesteś pewny?\n"),
                                        QMessageBox::No | QMessageBox::Yes,
                                        QMessageBox::Yes);

    if (resBtn == QMessageBox::Yes) {
        QCoreApplication::exit(0);
    }
}

unsigned int WyborMetody::getNumberHeight() const
{
    return data.numberHeight;
}

unsigned int WyborMetody::getNumberWidth() const
{
    return data.numberWidth;
}

unsigned int WyborMetody::getNumberPozMan() const
{
    return data.numberPozMan;
}

unsigned int WyborMetody::getTimeStopAuto() const
{
    return data.timeStopAuto;
}

unsigned int WyborMetody::getTimeStopManual() const
{
    return data.timeStopManual;
}

QString WyborMetody::getFileName() const
{
    return data.fileName;
}

void WyborMetody::on_timeAuto_editingFinished()
{
    if (!isValidTime(ui->timeAuto)){
        setEnabledContinue(false);
        return;
    }
    data.timeStopAuto = ui->timeAuto->text().toUInt();
    setEnabledContinue(isValidAutoParameters());
}

void WyborMetody::on_numberAuto_editingFinished()
{
    if (!isValidNumber(ui->numberAuto, 80)) {
        setEnabledContinue(false);
        return;
    }
    data.numberHeight = ui->numberAuto->text().toUInt();
    setEnabledContinue(isValidAutoParameters());
}

void WyborMetody::on_numberAuto_2_editingFinished()
{
    if (!isValidNumber(ui->numberAuto_2, 80)) {
        setEnabledContinue(false);
        return;
    }
    data.numberWidth = ui->numberAuto_2->text().toUInt();
    setEnabledContinue(isValidAutoParameters());
}


void WyborMetody::on_timeAuto_textChanged(const QString &)
{
    if (!isValidTime(ui->timeAuto)) {
        setEnabledContinue(false);
        return;
    }
    data.timeStopAuto = ui->timeAuto->text().toUInt();
    setEnabledContinue(isValidAutoParameters());
}

void WyborMetody::on_numberAuto_textChanged(const QString &)
{
    if (!isValidNumber(ui->numberAuto, 80)) {
        setEnabledContinue(false);
        return;
    }
    data.numberHeight = ui->numberAuto->text().toUInt();
    setEnabledContinue(isValidAutoParameters());
}

void WyborMetody::on_numberAuto_2_textChanged(const QString &)
{
    if (!isValidNumber(ui->numberAuto_2, 80)) {
        setEnabledContinue(false);
        return;
    }
    data.numberWidth = ui->numberAuto_2->text().toUInt();
    setEnabledContinue(isValidAutoParameters());;
}

void WyborMetody::on_timeManualDefault_textChanged(const QString &)
{
    if (!isValidTime(ui->timeManualDefault)) {
        setEnabledContinue(false);
        return;
    }
    data.timeStopManual = ui->timeManualDefault->text().toUInt();
    setEnabledContinue(isValidManualParameters());
}

void WyborMetody::on_numberManual_textChanged(const QString &)
{
    if (!isValidNumber(ui->numberManual, 1000)) {
        setEnabledContinue(false);
        return;
    }
    data.numberPozMan = ui->numberManual->text().toUInt();
    setEnabledContinue(isValidManualParameters());
}

void WyborMetody::on_rbRoletaDane_toggled(bool checked)
{
    ui->rbRoletaDane->setChecked(checked);
    ui->rbRoletaPlik->setChecked(!checked);
    ui->frRoleta->setDisabled(!checked);
    ui->frRoletaPlik->setDisabled(checked);
    wbMode = MODE_ROLETA;
    wbInsData = METHOD_MANUAL;
}

void WyborMetody::on_rbRoletaPlik_toggled(bool checked)
{
    ui->rbRoletaDane->setChecked(!checked);
    ui->rbRoletaPlik->setChecked(checked);
    ui->frRoleta->setDisabled(checked);
    ui->frRoletaPlik->setDisabled(!checked);
    wbMode = MODE_ROLETA;
    wbInsData = METHOD_FILE;
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
    data.fileName2 = file;
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
    if (ok && val >= 0) {
        ui->normaOffsetX->setText(ui->normaOffsetXEdit->text());
        data.offsetX = val;
    }

    val = ui->normaOffsetYEdit->text().toInt(&ok);
    if (ok && val >= 0) {
        ui->normaOffsetY->setText(ui->normaOffsetYEdit->text());
        data.offsetY = val;
    }
}


void WyborMetody::on_normaEtapNumber_textChanged(const QString &arg1)
{
    if (!isValidNumberRolety(ui->normaEtapNumber)) {
        setEnabledContinue(false);
        return;
    }
    data.etapNrRoleta = arg1.toUInt();
    setEnabledContinue(isValidRoletaRB() && isValidPostojRolety(ui->normaStabTime) && isValidIloscRolety(ui->NormaIloscProbek));
}


void WyborMetody::on_normaStabTime_textChanged(const QString &arg1)
{
    if (!isValidPostojRolety(ui->normaStabTime)) {
        setEnabledContinue(false);
        return;
    }
    data.stableTimeRoleta = arg1.toUInt();
    setEnabledContinue(isValidRoletaRB() && isValidNumberRolety(ui->normaEtapNumber) && isValidIloscRolety(ui->NormaIloscProbek));
}


void WyborMetody::on_NormaIloscProbek_textChanged(const QString &arg1)
{
    if (!isValidIloscRolety(ui->NormaIloscProbek)) {
        setEnabledContinue(false);
        return;
    }
    data.timeStopRoleta = arg1.toUInt();
    setEnabledContinue(isValidRoletaRB() && isValidNumberRolety(ui->normaEtapNumber) && isValidPostojRolety(ui->normaStabTime));
}

void WyborMetody::on_rbRoletaDane_clicked()
{

}





