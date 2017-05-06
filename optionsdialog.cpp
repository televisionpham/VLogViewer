#include "optionsdialog.h"
#include "ui_optionsdialog.h"
#include <QDebug>

OptionsDialog::OptionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionsDialog)
{
    ui->setupUi(this);
    interval_model = new QStandardItemModel(10, 1, this);
    QString names[10] {"1 sec", "2 sec", "5 sec", "10 sec", "15 sec", "30 sec", "1 min", "2 min", "5 min", "10 min"};
    for (int i = 0; i < intervals_count; ++i) {
        QStandardItem* item = new QStandardItem(names[i]);
        interval_model->setItem(i, item);
    }
    ui->cboInterval->setModel(interval_model);
}

OptionsDialog::~OptionsDialog()
{
    delete ui;
}

void OptionsDialog::SetSettings(AppSettings& app_settings)
{
    ui->chkShowClearLogConfirmation->setChecked(app_settings.show_clear_log_confirmation);
    ui->chkShowExitConfirmation->setChecked(app_settings.show_exit_confirmation);
    ui->chkShowStatusBar->setChecked(app_settings.GetShowStatusBar());
    ui->chkShowToolbar->setChecked(app_settings.GetShowToolbar());
    for (int i = 0; i < intervals_count; ++i) {
        if (values[i] == app_settings.reload_interval) {
            ui->cboInterval->setCurrentIndex(i);
            break;
        }
    }
}

void OptionsDialog::UpdateSettings(AppSettings& app_settings)
{
    app_settings.show_clear_log_confirmation = ui->chkShowClearLogConfirmation->isChecked();
    app_settings.show_exit_confirmation = ui->chkShowExitConfirmation->isChecked();
    app_settings.show_status_bar = ui->chkShowStatusBar->isChecked();
    app_settings.show_toolbar = ui->chkShowToolbar->isChecked();
    app_settings.reload_interval = values[ui->cboInterval->currentIndex()];
}
