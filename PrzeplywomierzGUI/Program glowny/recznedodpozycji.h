#ifndef RECZNEDODPOZYCJI_H
#define RECZNEDODPOZYCJI_H

#include <QDialog>
#include <QMap>

#include "pozycje.h"
class QTableWidgetItem;
namespace Ui {
class ReczneDodPozycji;
}

class ReczneDodPozycji : public QDialog
{
    Q_OBJECT

public:
    explicit ReczneDodPozycji(QWidget *parent = nullptr, unsigned int pos = 1, unsigned int timeStop = 300, unsigned int maxX = 2700,
                              unsigned int maxY = 3000);
    ~ReczneDodPozycji();

    Pozycje getList() const { return m_lista; }

public slots:
    void changeCell1(QTableWidgetItem *item);


private slots:
    void on_tbWybrane_cellChanged(int row, int column);

    void on_buttonBox_accepted();

private:
    Ui::ReczneDodPozycji *ui;
    Pozycje m_lista;
    unsigned int timeStop;
    unsigned int maxNumberX;
    unsigned int maxNumberY;
    QMap<QTableWidgetItem*, unsigned long> cells;

};

#endif // RECZNEDODPOZYCJI_H
