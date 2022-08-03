#include "wybranyplik.h"
#include "ui_wybranyplik.h"

#include <QFile>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QTextStream>

WybranyPlik::WybranyPlik(QWidget *parent, const QString &file, unsigned int maxNumber, unsigned int maxNumber2) :
    QDialog(parent),
    ui(new Ui::WybranyPlik),
    filename(file),
    maxVal1(maxNumber),
    maxVal2(maxNumber2)
{
    ui->setupUi(this);
    ui->lfilename->setText(file);
    QStringList headers;
    headers << QString::fromUtf8("Pozycja x [mm]") << QString::fromUtf8("Pozycja Y [mm]") << QString::fromUtf8("Czas [s]");
    ui->tbWybrane->setColumnCount(3);
    ui->tbWybrane->setHorizontalHeaderLabels(headers);
    ui->tbWybrane->setAutoScroll(true);
    ui->tbWybrane->resizeRowsToContents();
    ui->tbWybrane->resizeColumnsToContents();
    ui->tbWybrane->horizontalHeader()->setStretchLastSection(true);
    parseFile();
}

WybranyPlik::~WybranyPlik()
{
    delete ui;
}

void WybranyPlik::parseFile()
{
    QFile inputFile(filename);
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          QStringList vals = line.split(",");
          if (vals.size() != 3) {
              ui->leError->appendPlainText(QString::fromUtf8("Za dużo/mało kolumn : ") +  line);
              continue;
          }
          bool ok;
          int x = vals[0].toInt(&ok);
          if (!ok) {
              ui->leError->appendPlainText(QString::fromUtf8("Pozycja x nie jest liczbą :") + line);
              continue;
          }

          if (x < 0 || x > (int)maxVal1) {
              ui->leError->appendPlainText(QString::fromUtf8("Pozycja x po zakresem : ") + line);
              continue;
          }

          int y = vals[1].toInt(&ok);
          if (!ok) {
              ui->leError->appendPlainText(QString::fromUtf8("Pozycja y nie jest liczbą : ") + line);
              continue;
          }

          if (y < 0 || y > (int)maxVal2) {
              ui->leError->appendPlainText(QString::fromUtf8("Pozycja y po zakresem : ") + line);
              continue;
          }

          int time = vals[2].toInt(&ok);
          if (!ok) {
              ui->leError->appendPlainText(QString::fromUtf8("Wartość czas nie jest liczbą : ") + line);
              continue;
          }

          if (time < 0) {
              ui->leError->appendPlainText(QString::fromUtf8("Wartość czas po zakresem : ") + line);
              continue;
          }

          m_lista.append(time, x, y);
          int row = ui->tbWybrane->rowCount();
          ui->tbWybrane->setRowCount(row+1);

          QTableWidgetItem *item;
          item = new QTableWidgetItem(QString::number(x));
          item->setFlags(item->flags() ^ Qt::ItemIsEditable);
          item->setFlags(item->flags() ^ Qt::ItemIsSelectable);
          ui->tbWybrane->setItem(row, 0, item);

          item = new QTableWidgetItem(QString::number(y));
          item->setFlags(item->flags() ^ Qt::ItemIsEditable);
          item->setFlags(item->flags() ^ Qt::ItemIsSelectable);
          ui->tbWybrane->setItem(row, 1, item);

          item = new QTableWidgetItem(QString::number(time));
          item->setFlags(item->flags() ^ Qt::ItemIsEditable);
          item->setFlags(item->flags() ^ Qt::ItemIsSelectable);
          ui->tbWybrane->setItem(row, 2, item);
       }
       inputFile.close();
    }
}

void WybranyPlik::reject()
{
    QMessageBox::StandardButton resBtn = QMessageBox::Yes;
    resBtn = QMessageBox::question( this, "Przeplywomierz",
                                        tr("Are you sure?\n"),
                                        QMessageBox::No | QMessageBox::Yes,
                                        QMessageBox::Yes);

    if (resBtn == QMessageBox::Yes) {
        QDialog::reject();
    }
}

