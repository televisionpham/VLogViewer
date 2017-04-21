#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QWidget::setWindowIcon(QIcon(":/images/appicon.png"));
    QWidget::showMaximized();
    QCoreApplication::setOrganizationName("Vanpt");
    QCoreApplication::setApplicationName("VLogViewer");

    file_size_label = new QLabel(this);
    file_time_label = new QLabel(this);
    file_path_label = new QLabel(this);
    ui->statusBar->addPermanentWidget(file_size_label);
    ui->statusBar->addPermanentWidget(file_time_label);
    ui->statusBar->addPermanentWidget(file_path_label);

    LoadSettings();
    app_settings.SetRunTimes(app_settings.GetRunTimes() + 1);
    SaveSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::LoadSettings()
{
    QSettings settings;
    int runTimes = settings.value("RunTimes").toInt();
    app_settings.SetRunTimes(runTimes);
    if (runTimes <= 0)
    {
        return;
    }
    bool show_toolbar = settings.value("ShowToolbar").toBool();
    bool show_status_bar = settings.value("ShowStatusBar").toBool();
    int reload_interval = settings.value("ReloadInterval").toInt();
    bool follow_tail = settings.value("FollowTail").toBool();
    bool word_wrap = settings.value("WordWrap").toBool();
    app_settings.SetShowToolbar(show_toolbar);
    app_settings.SetShowStatusBar(show_status_bar);
    app_settings.SetReloadInterval(reload_interval);
    app_settings.SetFollowTail(follow_tail);
    app_settings.SetWordWrap(word_wrap);
}

void MainWindow::SaveSettings()
{
    QSettings settings;
    settings.setValue("RunTimes", app_settings.GetRunTimes());
    settings.setValue("ShowToolbar", app_settings.GetShowToolbar());
    settings.setValue("ShowStatusBar", app_settings.GetShowStatusBar());
    settings.setValue("ReloadInterval", app_settings.GetReloadInterval());
    settings.setValue("FollowTail", app_settings.GetFollowTail());
    settings.setValue("WordWrap", app_settings.GetWordWrap());
}

void MainWindow::UpdateCurrentStat()
{
    file_path_label->setText(current_file_path);
    QFileInfo file_info(current_file_path);
    auto file_size = file_info.size();
    if (file_size > 1000000)
    {
        file_size_label->setText(QString::number(file_size/1000000) + " MB");
    }
    else if (file_size > 1000)
    {
        file_size_label->setText(QString::number(file_size/1000) + " KB");
    }
    else
    {
        file_size_label->setText(QString::number(file_info.size()) + " B");
    }
    QDateTime file_time = file_info.lastModified();
    file_time_label->setText(file_time.toString(Qt::SystemLocaleShortDate));
}

void MainWindow::OpenFile(const QString& path)
{
    current_file_path = path;
    LogFile* log_file = new LogFile(this, path);
    ui->logButtonsHorizontalLayout->addWidget(log_file->file_button);
}

void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open file", "", "Log files (*.log);;All files (*.*)");
    if (filename.isEmpty())
    {
        return;
    }
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        OpenFile(filename);
    }
    else
    {
        QMessageBox::critical(this, qApp->applicationName(), "Unable to open file", QMessageBox::Ok);
        return;
    }
    UpdateCurrentStat();
}
