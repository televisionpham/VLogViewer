#include <iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>
#include <QDateTime>
#include <QCloseEvent>
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->splitter->setSizes(QList<int>() << 200 << 800);
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

    dir_model = new QDirModel;
    dir_model->setReadOnly(true);
    dir_model->setSorting(QDir::DirsFirst|QDir::IgnoreCase|QDir::Name);
    ui->dir_tree_view->setModel(dir_model);
    QModelIndex index = dir_model->index(QDir::currentPath());
    ui->dir_tree_view->expand(index);
    ui->dir_tree_view->scrollTo(index);
    ui->dir_tree_view->resizeColumnToContents(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton answer = QMessageBox::question(this, qAppName(), tr("Close the application?"), QMessageBox::Yes|QMessageBox::No);
    if (answer != QMessageBox::Yes)
    {
        event->ignore();
    }
    else
    {
        SaveSettings();
        this->refresh_thread->SetStop(true);
        if (this->refresh_thread->isRunning())
        {
            this->refresh_thread->quit();
            this->refresh_thread->wait(1000);
        }
        event->accept();
    }
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
    reload_interval = settings.value("ReloadInterval").toInt();
    this->follow_tail = settings.value("FollowTail").toBool();
    bool word_wrap = settings.value("WordWrap").toBool();
    app_settings.SetShowToolbar(show_toolbar);
    app_settings.SetShowStatusBar(show_status_bar);
    app_settings.SetReloadInterval(reload_interval);
    app_settings.SetFollowTail(follow_tail);
    app_settings.SetWordWrap(word_wrap);

    ui->actionWordWrap->setChecked(word_wrap);
    ui->actionFollowTail->setChecked(follow_tail);

    refresh_thread = new RefreshThread(this, this->current_file_path, ui->current_file_text_edit, this->reload_interval, this->follow_tail);
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
    int file_size = file_info.size();
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
    this->refresh_thread->SetStop(true);
    this->refresh_thread->wait(1000);
    if (this->refresh_thread->isRunning())
    {
        this->refresh_thread->quit();
    }
    current_file_path = path;
    ui->current_file_label->setText(current_file_path);
    QFileInfo fi(current_file_path);
    QModelIndex index = dir_model->index(fi.absoluteFilePath());
    ui->dir_tree_view->expand(index);
    ui->dir_tree_view->scrollTo(index);
    ui->dir_tree_view->resizeColumnToContents(0);
    QFile file(path);
    if (file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        ui->current_file_text_edit->setPlainText(QString::fromUtf8(file.readAll()));
        if (this->follow_tail)
        {
            ui->current_file_text_edit->verticalScrollBar()->setValue(ui->current_file_text_edit->verticalScrollBar()->maximum());
        }
        this->refresh_thread->SetStop(false);
        this->refresh_thread->start();
    }
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

void MainWindow::on_actionWordWrap_triggered(bool checked)
{
    if (checked)
    {
        ui->current_file_text_edit->setLineWrapMode(QTextEdit::WidgetWidth);
    }
    else
    {
        ui->current_file_text_edit->setLineWrapMode(QTextEdit::NoWrap);
    }
    app_settings.SetWordWrap(checked);
}

void MainWindow::on_actionFollowTail_triggered(bool checked)
{
    this->follow_tail = checked;
    app_settings.SetFollowTail(this->follow_tail);
}
