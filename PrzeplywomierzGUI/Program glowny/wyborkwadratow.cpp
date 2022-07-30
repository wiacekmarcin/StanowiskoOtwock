#include "wyborkwadratow.h"
#include "ui_wyborkwadratow.h"
#include <QPushButton>

WyborKwadratow::WyborKwadratow(QWidget *parent, int cols, int rows, unsigned int time, unsigned int maxX, unsigned int maxY) :
    QDialog(parent),
    ui(new Ui::WyborKwadratow)
    , maxX(maxX)
    , maxY(maxY)
    , posX(cols)
    , posY(rows)
    , timeStop(time)
{
    ui->setupUi(this);
    unsigned int x = 0;
    unsigned int y = 0;
    lastChoose = NULL;
    for (int r = 0; r < rows; r++) {
        for (int c =0; c < cols; c++) {
            QPushButton *pb = new QPushButton(ui->frWyborKw);

            pb->setObjectName(QString("pushButton_%1_%2").arg(r).arg(c));
            x = (unsigned int) (maxX * (c + 0.5) / posX);
            y = (unsigned int) (maxY * (r + 0.5) / posY);

            //pb->setText(QString("%1 - %2").arg(r+1).arg(c+1));
            pb->setText(QString("%1 ; %2").arg(x).arg(y));

            ui->grWyborKw->addWidget(pb, r, c, 1, 1);
            connect(pb, SIGNAL(clicked()), this, SLOT(wybranyKwadrat()));
            m_kwadraty[pb] = QPair<int, int>(r, c);
        }
    }
    QStringList headers;
    headers << QString::fromUtf8("Pozycja x\n[mm]") << QString::fromUtf8("Pozycja Y\n[mm]") << QString::fromUtf8("Czas\n[s]");
    ui->tbWybrane->setColumnCount(3);
    ui->tbWybrane->setHorizontalHeaderLabels(headers);
    ui->tbWybrane->setAutoScroll(true);
    ui->tbWybrane->resizeRowsToContents();
    ui->tbWybrane->resizeColumnsToContents();
    ui->tbWybrane->horizontalHeader()->setStretchLastSection(true);
}

WyborKwadratow::~WyborKwadratow()
{
    delete ui;
}

void WyborKwadratow::on_pbBack_clicked()
{
    if (lastChoose) {
        lastChoose->setEnabled(true);
        lastChoose = NULL;
        int row = m_lista.size();
        ui->tbWybrane->removeRow(row - 1);
        ui->tbWybrane->setRowCount(row-1);
        m_lista.pop_back();
    }
}

void WyborKwadratow::on_pbClear_clicked()
{
    auto mkeys = m_kwadraty.keys();
    for (int i =0; i < mkeys.size(); i++) {
        mkeys[i]->setEnabled(true);
    }
    int row = m_lista.size();
    m_lista.clear();

    for (int t = row-1; t >=0; --t) {
        ui->tbWybrane->takeItem(t, 0);
        ui->tbWybrane->takeItem(t, 1);
        ui->tbWybrane->takeItem(t, 2);
    }
    ui->tbWybrane->setRowCount(0);
}

void WyborKwadratow::wybranyKwadrat()
{
    QPushButton *button = (QPushButton *)sender();
    lastChoose = button;
    button->setEnabled(false);
    ui->tbWybrane->setRowCount(ui->tbWybrane->rowCount() + 1);
    int row = m_lista.size();

    unsigned int x = (unsigned int) (maxX * (m_kwadraty[button].second + 0.5) / posX);
    unsigned int y = (unsigned int) (maxY * (m_kwadraty[button].first + 0.5) / posY);
    QTableWidgetItem *item;
    m_lista.append(timeStop, x, y);
    item = new QTableWidgetItem(QString::number(x));
    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
    item->setFlags(item->flags() ^ Qt::ItemIsSelectable);
    ui->tbWybrane->setItem(row, 0, item);

    item = new QTableWidgetItem(QString::number(y));
    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
    item->setFlags(item->flags() ^ Qt::ItemIsSelectable);
    ui->tbWybrane->setItem(row, 1, item);

    item = new QTableWidgetItem(QString::number(timeStop));
    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
    item->setFlags(item->flags() ^ Qt::ItemIsSelectable);
    ui->tbWybrane->setItem(row, 2, item);


}
