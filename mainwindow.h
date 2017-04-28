#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "appsettings.h"
#include <QLabel>
#include <QSplitter>
#include <QDirModel>
#include "refreshthread.h"

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

private:
    Ui::MainWindow *ui;
    AppSettings app_settings;    
    QLabel* file_size_label;
    QLabel* file_time_label;
    QLabel* file_path_label;
    QString current_file_path = tr("");
    QDirModel* dir_model;
    int reload_interval = 1;
    bool follow_tail = true;
    RefreshThread* refresh_thread;

    void closeEvent(QCloseEvent *event) override;
    void LoadSettings();
    void SaveSettings();
    void UpdateCurrentStat();
    void OpenFile(const QString&);
};

#endif // MAINWINDOW_H
