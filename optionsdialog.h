#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>
#include "appsettings.h"
#include <QStandardItemModel>
#include <QStandardItem>

namespace Ui {
class OptionsDialog;
}

class OptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionsDialog(QWidget *parent = 0);
    ~OptionsDialog();
    void SetSettings(AppSettings&);
    void UpdateSettings(AppSettings&);

private:
    Ui::OptionsDialog *ui;
    QStandardItemModel* interval_model;
    const int intervals_count = 10;
    int values[10] {1, 2, 5, 10, 15, 30, 60, 2*60, 5*60, 10*60};
};

#endif // OPTIONSDIALOG_H
