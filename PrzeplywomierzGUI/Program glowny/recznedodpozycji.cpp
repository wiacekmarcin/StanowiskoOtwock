#include "recznedodpozycji.h"
#include "ui_recznedodpozycji.h"

ReczneDodPozycji::ReczneDodPozycji(QWidget *parent, unsigned int pos, unsigned int ts, unsigned int maxX, unsigned int maxY) :
    QDialog(parent),
    ui(new Ui::ReczneDodPozycji),
    timeStop(ts),
    maxNumberX(maxX),
    maxNumberY(maxY)
{
    ui->setupUi(this);
    QStringList headers;
    headers << QString::fromUtf8("Pozycja X [mm]") << QString::fromUtf8("Pozycja Y [mm]") << QString::fromUtf8("Czas [s]");
    ui->tbWybrane->setColumnCount(3);
    ui->tbWybrane->setHorizontalHeaderLabels(headers);
    ui->tbWybrane->setAutoScroll(true);
    ui->tbWybrane->resizeRowsToContents();
    ui->tbWybrane->resizeColumnsToContents();
    ui->tbWybrane->horizontalHeader()->setStretchLastSection(true);
    ui->tbWybrane->setRowCount(pos);
    unsigned long posIdx = 0;
    for (unsigned int row=0; row < pos; row++) {
        QTableWidgetItem *item;

        item = new QTableWidgetItem(QString(""));
        ui->tbWybrane->setItem(row, 0, item);
        cells[item] = posIdx++;


        item = new QTableWidgetItem(QString(""));
        ui->tbWybrane->setItem(row, 1, item);

        cells[item] = posIdx++;

        item = new QTableWidgetItem(QString::number(timeStop));
        ui->tbWybrane->setItem(row, 2, item);

        cells[item] = posIdx++;
    }
    ui->pbOk->setEnabled(false);

    connect(ui->tbWybrane, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(changeCell1(QTableWidgetItem*)));
}

ReczneDodPozycji::~ReczneDodPozycji()
{
    delete ui;
}

void ReczneDodPozycji::changeCell1(QTableWidgetItem *item)
{
    QString data = item->text();
    bool ok;
    int val = data.toInt(&ok, 10);
    if (!ok || val < 0) {
        item->setText("0");
    }
    unsigned long pos = cells[item];
    //unsigned int rows = pos / 3;
    unsigned int cols = pos % 3;
    ui->pbOk->setEnabled(false);
    switch(cols) {
        case 0:
            if (val > (int)maxNumberX)
                item->setBackground(QBrush(QColor(Qt::red), Qt::SolidPattern));
            else
                item->setBackground(QBrush(QColor(Qt::white), Qt::SolidPattern));
        break;

        case 1:
            if (val > (int)maxNumberY)
                item->setBackground(QBrush(QColor(Qt::red), Qt::SolidPattern));
            else
                item->setBackground(QBrush(QColor(Qt::white), Qt::SolidPattern));

        break;
    }
}



void ReczneDodPozycji::on_tbWybrane_cellChanged(int row, int column)
{
    if (column != 2) { //x or y
        QTableWidgetItem *item = ui->tbWybrane->item(row, column);
        QString data = item->text();
        if (data == "")
            return;
        bool ok;
        unsigned int val = data.toInt(&ok, 10);
        if (!ok || val > (column == 0 ? maxNumberX : maxNumberY)) {
            item->setText(QString("%1").arg(val));
        }
    }
}

void ReczneDodPozycji::on_pbSprawdz_clicked()
{
    m_lista.clear();
    QString sx, sy, stime;
    bool ok;
    int time = 0, x, y;
    bool valid = true;
    bool localvalid = true;
    for (int r = 0; r < ui->tbWybrane->rowCount() ; r++) {
        sx = ui->tbWybrane->item(r, 0)->text();
        sy = ui->tbWybrane->item(r, 1)->text();
        stime = ui->tbWybrane->item(r, 2)->text();
        localvalid = true;
        QTableWidgetItem * item = ui->tbWybrane->item(r , 0);
        x = sx.toInt(&ok);
        if (sx == "" || !ok || x < 0 || x > (int)maxNumberX) {
            item->setBackground(QBrush(QColor(Qt::red), Qt::SolidPattern));
            valid = false;
            localvalid = false;
        }

        item = ui->tbWybrane->item(r , 1);
        y = sy.toInt(&ok);
        if (sy == "" || !ok || y < 0 || y > (int)maxNumberY) {
            item->setBackground(QBrush(QColor(Qt::red), Qt::SolidPattern));
            valid = false;
            localvalid = false;
        }

        item = ui->tbWybrane->item(r , 2);
        time = stime.toInt(&ok);
        if (stime == "" || !ok || time < 0) {
            item->setBackground(QBrush(QColor(Qt::red), Qt::SolidPattern));
            valid = false;
            localvalid = false;
        }
        if (localvalid)
            m_lista.append(time, x, y);
    }
    ui->pbOk->setEnabled(valid);
}


void ReczneDodPozycji::on_pbOk_clicked()
{
    accept();
}


void ReczneDodPozycji::on_pbAnuluj_clicked()
{
    reject();
}

