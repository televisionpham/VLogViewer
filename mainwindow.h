#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "appsettings.h"
#include <QLabel>
#include <QSplitter>
#include <QFileSystemModel>
#include "refreshthread.h"
#include "optionsdialog.h"
#include "dialogs/finddialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();    

    void on_actionWordWrap_triggered(bool checked);

    void on_actionFollowTail_triggered(bool checked);

    void on_actionClose_triggered();

    void on_actionClear_triggered();

    void on_dir_tree_view_doubleClicked(const QModelIndex &index);

    void on_actionReload_triggered();

    void on_actionOptions_triggered();

    void on_actionSelectAll_triggered();

    void on_actionCopy_triggered();

    void on_actionFind_triggered();

    void on_actionFindNext_triggered();

    void on_actionFindPrevious_triggered();

    void on_actionAbout_triggered();

    void on_actionFont_triggered();

private:
    Ui::MainWindow *ui;
    AppSettings app_settings;    
    QLabel* file_size_label;
    QLabel* file_time_label;
    QLabel* file_path_label;    
    QFileSystemModel* dir_model;
    RefreshThread* refresh_thread;
    OptionsDialog* options_dialog;
    FindDialog* find_dialog;

    void closeEvent(QCloseEvent *event) override;
    void LoadSettings();
    void SaveSettings();
    void UpdateCurrentStat();
    void OpenFile(const QString&);
    void StopRefreshThread();
};

#endif // MAINWINDOW_H
