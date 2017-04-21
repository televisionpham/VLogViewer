#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "appsettings.h"
#include <QLabel>
#include "logfile.h"

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

private:
    Ui::MainWindow *ui;
    AppSettings app_settings;
    QLabel* file_size_label;
    QLabel* file_time_label;
    QLabel* file_path_label;
    QString current_file_path;
    QVector<LogFile> log_files;
    void LoadSettings();
    void SaveSettings();
    void UpdateCurrentStat();
    void OpenFile(const QString&);
};

#endif // MAINWINDOW_H
