#ifndef ETAPYROLETDLG_H
#define ETAPYROLETDLG_H

#include <QDialog>

namespace Ui {
class EtapyRoletDlg;
}

class EtapyRoletDlg : public QDialog
{
    Q_OBJECT

public:
    explicit EtapyRoletDlg(QWidget *parent = 0);
    ~EtapyRoletDlg();

private:
    Ui::EtapyRoletDlg *ui;
};

#endif // ETAPYROLETDLG_H
