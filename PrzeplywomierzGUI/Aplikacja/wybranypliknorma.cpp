#include "wybranypliknorma.h"
#include "ui_wybranypliknorma.h"

#include <QFile>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QTextStream>

WybranyPlikNorma::WybranyPlikNorma(QWidget *parent, bool reverse, const QString &file, unsigned int maxNumber,
                                   unsigned int maxNumber2, unsigned int maxR, unsigned int width,
                                   unsigned int offsetx, unsigned int offsety) :
    QDialog(parent),
    ui(new Ui::WybranyPlikNorma),
    filename(file),
    etapMax(maxNumber),
    timeStable(maxNumber2),
    m_lista(reverse, etapMax, maxR, 0, timeStable, width, offsetx, offsety)
{
    ui->setupUi(this);
    ui->lfilename->setText(file);
    QStringList headers;
    headers << QString::fromUtf8("Nr Etapu") << QString::fromUtf8("Pozycja x [-0.5 : 0.5]") << QString::fromUtf8("Pozycja Y [-0.5 : 0.5]") << QString::fromUtf8("Czas [s]");
    ui->tbWybrane->setColumnCount(4);
    ui->tbWybrane->setHorizontalHeaderLabels(headers);
    ui->tbWybrane->setAutoScroll(true);
    ui->tbWybrane->resizeRowsToContents();
    ui->tbWybrane->resizeColumnsToContents();
    ui->tbWybrane->horizontalHeader()->setStretchLastSection(true);

    width = 800;
    maxR = 1500;

    m_lista.setInit(etapMax, maxR , width, 0, timeStable);
    m_lista.setOffset(reverse, offsetx, offsety);

    prevNorma = -1;

    parseFile();
}

WybranyPlikNorma::~WybranyPlikNorma()
{
    delete ui;
}

void WybranyPlikNorma::parseFile()
{
    QFile inputFile(filename);
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          QStringList vals = line.split(",");
          if (vals.size() != 4) {
              ui->leError->appendPlainText(QString::fromUtf8("Za dużo/mało kolumn : ") +  line);
              continue;
          }
          bool ok;
          int n = vals[0].toInt(&ok);
          if (!ok) {
              ui->leError->appendPlainText(QString::fromUtf8("Pozycja etap nie jest liczbą :") + line);
              continue;
          }
          if (n < 1 || n > 15) {
              ui->leError->appendPlainText(QString::fromUtf8("Pozycja etap jest po zakresem [1-15] :") + line);
              continue;
          }

          if (n > (int)etapMax) {
              ui->leError->appendPlainText(QString::fromUtf8("Pozycja etap jest za duża :") + line);
              continue;
          }

          float x = vals[1].toFloat(&ok);
          if (!ok) {
              ui->leError->appendPlainText(QString::fromUtf8("Pozycja x nie jest liczbą :") + line);
              continue;
          }

          if (x < -0.5 || x > 0.5) {
              ui->leError->appendPlainText(QString::fromUtf8("Pozycja x po zakresem [-0.5 : 0.5] : ") + line);
              continue;
          }

          float y = vals[2].toFloat(&ok);
          if (!ok) {
              ui->leError->appendPlainText(QString::fromUtf8("Pozycja y nie jest liczbą : ") + line);
              continue;
          }

          if (y < -0.5 || y > 0.5) {
              ui->leError->appendPlainText(QString::fromUtf8("Pozycja y po zakresem [-0.5 : 0.5] : ") + line);
              continue;
          }

          int time = vals[3].toInt(&ok);
          if (!ok) {
              ui->leError->appendPlainText(QString::fromUtf8("Wartość czas nie jest liczbą : ") + line);
              continue;
          }

          if (time < 0) {
              ui->leError->appendPlainText(QString::fromUtf8("Wartość czas po zakresem : ") + line);
              continue;
          }


          int row = ui->tbWybrane->rowCount();
          ui->tbWybrane->setRowCount(row+1);

          if  (prevNorma != n) {
              m_lista.appendRoletaEtap(n-1);
              prevNorma = n;
          }

          m_lista.appendPoint(n-1, x, y, time);


          QTableWidgetItem *item;
          item = new QTableWidgetItem(QString::number(n));
          item->setFlags(item->flags() ^ Qt::ItemIsEditable);
          item->setFlags(item->flags() ^ Qt::ItemIsSelectable);
          ui->tbWybrane->setItem(row, 0, item);

          item = new QTableWidgetItem(QString::number(x, 'g', 3));
          item->setFlags(item->flags() ^ Qt::ItemIsEditable);
          item->setFlags(item->flags() ^ Qt::ItemIsSelectable);
          ui->tbWybrane->setItem(row, 1, item);

          item = new QTableWidgetItem(QString::number(y, 'g', 3));
          item->setFlags(item->flags() ^ Qt::ItemIsEditable);
          item->setFlags(item->flags() ^ Qt::ItemIsSelectable);
          ui->tbWybrane->setItem(row, 2, item);

          item = new QTableWidgetItem(QString::number(time));
          item->setFlags(item->flags() ^ Qt::ItemIsEditable);
          item->setFlags(item->flags() ^ Qt::ItemIsSelectable);
          ui->tbWybrane->setItem(row, 3, item);
       }
       inputFile.close();
    }
}

void WybranyPlikNorma::reject()
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

