#include "wybormetody.h"
#include "ui_wybormetody.h"
#include "pozycjonowanieoffsetunormy.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <QDebug>

WyborMetody::WyborMetody(QWidget *parent, ModeWork mode, MethodInsData dataIns,
                         Ustawienia & u, SerialDevice & sdev ) :
    QDialog(parent),
    ui(new Ui::WyborMetody),
    wbInsData(dataIns),
    wbMode(mode),
    ust(u),
    sd(sdev)
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
    data.offsetXL = 0;
    data.offsetYL = 0;
    data.offsetXP = 0;
    data.offsetYP = 0;

    ui->setupUi(this);

    init();
    showMaximized();

    QString oxp = ust.getRolOffsetX_P();
    QString oyp = ust.getRolOffsetY_P();
    QString oxl = ust.getRolOffsetX_L();
    QString oyl = ust.getRolOffsetY_L();

    //qDebig() << "oxp" << oxp << "oyp" << oyp << "oxl" << oxl << "oyl" << oyl;

    ////qDebig() << __FILE__ << __LINE__ <<  ox << oy;
    ui->normaOffsetXL->setText(oxl);
    ui->normaOffsetXEditL->setText(oxl);
    ui->normaOffsetXP->setText(oxp);
    ui->normaOffsetXEditP->setText(oxp);
    ui->normaOffsetYL->setText(oyl);
    ui->normaOffsetYEditL->setText(oyl);
    ui->normaOffsetYP->setText(oyp);
    ui->normaOffsetYEditP->setText(oyp);
    data.offsetXL = oxl.toUInt();
    data.offsetYL = oyl.toUInt();
    data.offsetXP = oxp.toUInt();
    data.offsetYP = oyp.toUInt();
    ui->pbNormaCancel->setEnabled(false);

    ui->rb2700->setText(QString("Stanowisko\n%1x%2 [mm]").arg(ust.getStacOsXNazwa(),ust.getStacOsYNazwa()));
    ui->rb1000p->setText(QString("Stanowisko\n%1x%2 [mm]\nprawe").arg(ust.getOknoOsXNazwa(), ust.getOknoOsYNazwa()));
    ui->rb1000l->setText(QString("Stanowisko\n%1x%2 [mm]\nlewe").arg(ust.getOknoOsXNazwa(), ust.getOknoOsYNazwa()));
    ui->rbRoletaP->setText(QString("Stanowisko z roletą\n%1x%2 [mm]\nprawe").arg(ust.getRolOsXNazwa(), ust.getRolOsYNazwa()));
    ui->rbRoletaL->setText(QString("Stanowisko z roletą\n%1x%2 [mm]\nlewe").arg(ust.getRolOsXNazwa(), ust.getRolOsYNazwa()));
}

WyborMetody::~WyborMetody()
{
    delete ui;
}

void WyborMetody::init()
{
    //qDebig() << __FILE__ << __LINE__ << wbMode << wbInsData;
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
            ui->rbRoletaP->setChecked(false);
            ui->rbRoletaL->setChecked(false);
            visibleOther(true);
            visibleRoleta(false, false);
            //ui->gbMethod->setVisible(true);
            initMethodPosition();
        break;
        case MODE_1000P:
            ui->rb2700->setChecked(false);
            ui->rb1000p->setChecked(true);
            ui->rb1000l->setChecked(false);
            ui->rbFunSet->setChecked(false);
            ui->rbRoletaP->setChecked(false);
            ui->rbRoletaL->setChecked(false);
            visibleOther(true);
            visibleRoleta(false, false);
            //ui->gbMethod->setVisible(true);
            initMethodPosition();
        break;
        case MODE_1000L:
            ui->rb2700->setChecked(false);
            ui->rb1000p->setChecked(false);
            ui->rb1000l->setChecked(true);
            ui->rbFunSet->setChecked(false);
            ui->rbRoletaP->setChecked(false);
            ui->rbRoletaL->setChecked(false);
            visibleOther(true);
            visibleRoleta(false, false);
            //ui->gbMethod->setVisible(true);
            initMethodPosition();
        break;
        case MODE_FUNSET:
            visibleOther(false);
            visibleRoleta(false, false);
            ui->rbFunSet->setChecked(true);
            //ui->gbMethod->setVisible(false);
            //ui->gbMethod->setDisabled(true);
        break;
        case MODE_SERVICE:
            visibleOther(false);
            visibleRoleta(false, false);
            //ui->gbMethod->setVisible(false);
        break;
        case MODE_ROLETAP:
            ui->rb2700->setChecked(false);
            ui->rb1000p->setChecked(false);
            ui->rb1000l->setChecked(false);
            ui->rbFunSet->setChecked(false);
            ui->rbRoletaP->setChecked(true);
            ui->rbRoletaL->setChecked(false);
            visibleOther(false);
            visibleRoleta(true, false);
            //ui->gbMethod->setVisible(true);
        break;
        case MODE_ROLETAL:
            ui->rb2700->setChecked(false);
            ui->rb1000p->setChecked(false);
            ui->rb1000l->setChecked(false);
            ui->rbFunSet->setChecked(false);
            ui->rbRoletaP->setChecked(false);
            ui->rbRoletaL->setChecked(true);
            visibleOther(false);
            visibleRoleta(true, true);
            //ui->gbMethod->setVisible(true);
    break;
    default:
        break;
    }
    setEnabledContinue(false);
}

void WyborMetody::initMethodPosition()
{
    ////qDebig() << __FILE__ << __LINE__ << __PRETTY_FUNCTION__ << wbInsData;
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
    if (checked) {
        ui->frManual->setDisabled(true);
        ui->frHalfManual->setDisabled(true);
        ui->frChoiceFile->setDisabled(false);
        ui->rbmanual->setChecked(false);
        ui->rbhalfmanual->setChecked(false);
        ui->rbfile->setChecked(true);
        wbInsData = METHOD_FILE;
        setEnabledContinue(!getFileName().isEmpty());
    } else {
        return;
    }


}

void WyborMetody::on_rbmanual_toggled(bool checked)
{

    if (checked) {
        ui->frManual->setDisabled(false);
        ui->frHalfManual->setDisabled(true);
        ui->frChoiceFile->setDisabled(true);
        ui->rbmanual->setChecked(true);
        ui->rbhalfmanual->setChecked(false);
        ui->rbfile->setChecked(false);
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
    if (checked) {
        ui->frManual->setDisabled(true);
        ui->frHalfManual->setDisabled(false);
        ui->frChoiceFile->setDisabled(true);
        ui->rbmanual->setChecked(false);
        ui->rbhalfmanual->setChecked(true);
        ui->rbfile->setChecked(false);
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

void WyborMetody::visibleRoleta(bool visible, bool left)
{
    ui->frRoletaInfo->setVisible(visible);
    if (visible) {
        ui->pbNormaOffsetSave->setEnabled(false);
        ui->pbNormaOffsetChange->setEnabled(true);
        //qDebig() << "left" << left;
        ui->normaOffsetXL->setVisible(left);
        ui->normaOffsetYL->setVisible(left);
        ui->normaOffsetXP->setVisible(!left);
        ui->normaOffsetYP->setVisible(!left);

        ui->normaOffsetXEditP->setVisible(false);
        ui->normaOffsetYEditP->setVisible(false);
        ui->normaOffsetXEditL->setVisible(false);
        ui->normaOffsetYEditL->setVisible(false);
        ui->pbNormaOffsetChange->setEnabled(true);
        ui->pbNormaOffsetSave->setEnabled(false);
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
        visibleRoleta(false, false);
        wbMode = MODE_2700;
        on_rbfile_toggled(true);
    }
}

void WyborMetody::on_rb1000l_toggled(bool checked)
{
    if (checked) {
        visibleOther(true);
        visibleRoleta(false, false);
        wbMode = MODE_1000L;
        on_rbfile_toggled(true);
    }
}

void WyborMetody::on_rb1000p_toggled(bool checked)
{
    if (checked) {
        visibleOther(true);
        visibleRoleta(false, false);
        wbMode = MODE_1000P;
        on_rbfile_toggled(true);
    }
}

void WyborMetody::on_rbFunSet_toggled(bool checked)
{
    if (checked) {
        visibleOther(false);
        visibleRoleta(false, false);
        wbMode = MODE_FUNSET;
        setEnabledContinue(true);
    }
}

void WyborMetody::on_rbRoletaP_toggled(bool checked)
{
    if (checked) {
        wbMode = MODE_ROLETAP;
        visibleOther(false);
        visibleRoleta(true, false);
        on_rbRoletaDane_toggled(true);
    }
    //qDebig() << "ROLETAP checked" << checked << "wbMode" << wbMode;
}

void WyborMetody::on_rbRoletaL_toggled(bool checked)
{
    if (checked) {
        wbMode = MODE_ROLETAL;
        visibleOther(false);
        visibleRoleta(true, true);
        on_rbRoletaDane_toggled(true);
    }
    //qDebig() << "ROLETAL checked" << checked << "wbMode" << wbMode;
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
    ////qDebig() << __FILE__ << __LINE__ << __FUNCTION__;
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
    if (checked) {
        ui->rbRoletaDane->setChecked(checked);
        ui->rbRoletaPlik->setChecked(!checked);
        ui->frRoleta->setDisabled(!checked);
        ui->frRoletaPlik->setDisabled(checked);
        wbInsData = METHOD_MANUAL;
        setEnabledContinue(isValidRoletaRB() && isValidPostojRolety(ui->normaStabTime) && isValidIloscRolety(ui->NormaIloscProbek));
    }
}

void WyborMetody::on_rbRoletaPlik_toggled(bool checked)
{
    if (checked) {
        ui->rbRoletaDane->setChecked(!checked);
        ui->rbRoletaPlik->setChecked(checked);
        ui->frRoleta->setDisabled(checked);
        ui->frRoletaPlik->setDisabled(!checked);
        wbInsData = METHOD_FILE;
        setEnabledContinue(isValidRoletaRB() && !ui->wybranyPlikRolety->text().isEmpty());
    }
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
    ui->pbNormaCancel->setEnabled(true);

    ui->normaOffsetXEditL->setVisible(wbMode == MODE_ROLETAL);
    ui->normaOffsetYEditL->setVisible(wbMode == MODE_ROLETAL);

    ui->normaOffsetXEditP->setVisible(wbMode == MODE_ROLETAP);
    ui->normaOffsetYEditP->setVisible(wbMode == MODE_ROLETAP);

    ui->normaOffsetXL->setVisible(false);
    ui->normaOffsetYL->setVisible(false);
    ui->normaOffsetXP->setVisible(false);
    ui->normaOffsetYP->setVisible(false);
}

void WyborMetody::on_pbNormaOffsetSave_clicked()
{
    on_pbNormaCancel_clicked();

    bool ok;
    int val;
    if (wbMode == MODE_ROLETAL) {
        val = ui->normaOffsetXEditL->text().toInt(&ok);
        if (ok && val >= 0) {
            ui->normaOffsetXL->setText(ui->normaOffsetXEditL->text());
            data.offsetXL = val;
        } else {
            return;
        }

        val = ui->normaOffsetYEditL->text().toInt(&ok);
        if (ok && val >= 0) {
            ui->normaOffsetYL->setText(ui->normaOffsetYEditL->text());
            data.offsetYL = val;
        } else {
            return;
        }

        ust.setRolOffsetX_L(QString::number(data.offsetXL));
        ust.setRolOffsetY_L(QString::number(data.offsetYL));
    } else if (wbMode == MODE_ROLETAP) {
        val = ui->normaOffsetXEditP->text().toInt(&ok);
        if (ok && val >= 0) {
            ui->normaOffsetXP->setText(ui->normaOffsetXEditP->text());
            data.offsetXP = val;
        } else {
            return;
        }

        val = ui->normaOffsetYEditP->text().toInt(&ok);
        if (ok && val >= 0) {
            ui->normaOffsetYP->setText(ui->normaOffsetYEditP->text());
            data.offsetYP = val;
        } else {
            return;
        }

        ust.setRolOffsetX_P(QString::number(data.offsetXP));
        ust.setRolOffsetY_P(QString::number(data.offsetYP));
    } else {
        return;
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


void WyborMetody::on_normaEtapNumberFile_textChanged(const QString &arg1)
{
    if (!isValidNumberRolety(ui->normaEtapNumberFile)) {
        setEnabledContinue(false);
        return;
    }
    data.etapNrRoleta = arg1.toUInt();
    setEnabledContinue(isValidRoletaRB() && isValidPostojRolety(ui->normaStabTimeFile) && !ui->wybranyPlikRolety->text().isEmpty());
}


void WyborMetody::on_normaStabTimeFile_textChanged(const QString &arg1)
{
    ////qDebig() << data.stableTimeRoleta << arg1;
    if (!isValidPostojRolety(ui->normaStabTimeFile)) {
        setEnabledContinue(false);
        return;
    }
    ////qDebig() << "ok";
    data.stableTimeRoleta = arg1.toUInt();
    ////qDebig() << data.stableTimeRoleta;
    setEnabledContinue(isValidRoletaRB() && isValidNumberRolety(ui->normaEtapNumberFile) && !ui->wybranyPlikRolety->text().isEmpty());
}


void WyborMetody::on_pbNormaCancel_clicked()
{
    ui->pbNormaOffsetChange->setEnabled(true);
    ui->pbNormaOffsetSave->setEnabled(false);
    ui->pbNormaCancel->setEnabled(false);


    ui->normaOffsetXL->setVisible(wbMode == MODE_ROLETAL);
    ui->normaOffsetYL->setVisible(wbMode == MODE_ROLETAL);
    ui->normaOffsetXP->setVisible(wbMode == MODE_ROLETAP);
    ui->normaOffsetYP->setVisible(wbMode == MODE_ROLETAP);

    ui->normaOffsetXEditL->setVisible(false);
    ui->normaOffsetYEditL->setVisible(false);
    ui->normaOffsetXEditP->setVisible(false);
    ui->normaOffsetYEditP->setVisible(false);
}


void WyborMetody::on_pbNornaSet_clicked()
{
    unsigned int offsetX = wbMode == MODE_ROLETAL ? ui->normaOffsetXL->text().toUInt() : ui->normaOffsetXP->text().toUInt();
    unsigned int offsetY = wbMode == MODE_ROLETAL ? ui->normaOffsetYL->text().toUInt() : ui->normaOffsetYP->text().toUInt();
    PozycjonowanieOffsetuNormy * dlg = new PozycjonowanieOffsetuNormy(ust, &sd, wbMode == MODE_ROLETAL,
                                            offsetX, offsetY, this);
    int r = dlg->exec();
    
    if (r == 0) {
        return;
    }
    
    if (wbMode == MODE_ROLETAL) {
        ust.setRolOffsetX_L(QString::number(dlg->getOX()));
        ust.setRolOffsetY_L(QString::number(dlg->getOY()));
        ui->normaOffsetXL->setText(QString::number(dlg->getOX()));
        ui->normaOffsetYL->setText(QString::number(dlg->getOY()));
        ui->normaOffsetXEditL->setText(QString::number(dlg->getOX()));
        ui->normaOffsetYEditL->setText(QString::number(dlg->getOY()));
    } else if (wbMode == MODE_ROLETAP) {
        ust.setRolOffsetX_P(QString::number(dlg->getOX()));
        ust.setRolOffsetY_P(QString::number(dlg->getOY()));
        ui->normaOffsetXP->setText(QString::number(dlg->getOX()));
        ui->normaOffsetYP->setText(QString::number(dlg->getOY()));
        ui->normaOffsetXEditP->setText(QString::number(dlg->getOX()));
        ui->normaOffsetYEditP->setText(QString::number(dlg->getOY()));
    }
    dlg->deleteLater();
}

