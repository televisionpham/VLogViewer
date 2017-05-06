#include <iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>
#include <QDateTime>
#include <QCloseEvent>
#include <QScrollBar>
#include <QClipboard>
#include <QFontDialog>
#include <QDebug>

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

    options_dialog = new OptionsDialog(this);

    LoadSettings();
    app_settings.SetRunTimes(app_settings.GetRunTimes() + 1);
    SaveSettings();

    dir_model = new QFileSystemModel(this);
    dir_model->setReadOnly(true);
    dir_model->setFilter(QDir::AllEntries|QDir::NoDotAndDotDot);
    dir_model->setRootPath("C:");
    ui->dir_tree_view->setModel(dir_model);    
    ui->dir_tree_view->resizeColumnToContents(0);

    refresh_thread = new RefreshThread(
                this,
                app_settings.last_file,
                ui->current_file_text_edit,
                app_settings.reload_interval,
                app_settings.follow_tail);
    ui->mainToolBar->setVisible(app_settings.show_toolbar);
    ui->statusBar->setVisible(app_settings.show_status_bar);
    if (!app_settings.last_file.isNull() && !app_settings.last_file.isEmpty())
    {
        OpenFile(app_settings.last_file);
    }
    find_dialog = new FindDialog(this);
    find_dialog->setModal(false);
    find_dialog->setTextEdit(ui->current_file_text_edit);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (!app_settings.show_exit_confirmation)
    {
        SaveSettings();
        StopRefreshThread();
        return;
    }
    QMessageBox::StandardButton answer = QMessageBox::question(this, qAppName(), tr("Close the application?"), QMessageBox::Yes|QMessageBox::No);
    if (answer != QMessageBox::Yes)
    {
        event->ignore();
    }
    else
    {
        SaveSettings();
        StopRefreshThread();
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
    int reload_interval = settings.value("ReloadInterval").toInt();
    bool follow_tail = settings.value("FollowTail").toBool();
    bool word_wrap = settings.value("WordWrap").toBool();
    app_settings.show_exit_confirmation = settings.value("ShowExitConfirmation").toBool();
    app_settings.show_clear_log_confirmation = settings.value("ShowClearLogConfirmation").toBool();
    app_settings.last_file = settings.value("LastFile").toString();
    app_settings.SetShowToolbar(show_toolbar);
    app_settings.SetShowStatusBar(show_status_bar);
    app_settings.SetReloadInterval(reload_interval);
    app_settings.SetFollowTail(follow_tail);
    app_settings.SetWordWrap(word_wrap);        

    ui->actionWordWrap->setChecked(word_wrap);
    ui->actionFollowTail->setChecked(follow_tail);    
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
    settings.setValue("ShowExitConfirmation", app_settings.show_exit_confirmation);
    settings.setValue("ShowClearLogConfirmation", app_settings.show_clear_log_confirmation);
    settings.setValue("LastFile", app_settings.last_file);
}

void MainWindow::UpdateCurrentStat()
{
    file_path_label->setText(app_settings.last_file);
    if (app_settings.last_file != "")
    {
        QFileInfo file_info(app_settings.last_file);
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
    else
    {
        file_size_label->setText("0");
        file_time_label->setText("");
    }
}

void MainWindow::OpenFile(const QString& path)
{
    StopRefreshThread();    
    app_settings.last_file = path;
    ui->current_file_label->setText(app_settings.last_file);
    QFileInfo fi(app_settings.last_file);
    QModelIndex index = dir_model->index(fi.absoluteFilePath());
    ui->dir_tree_view->expand(index);
    ui->dir_tree_view->scrollTo(index);
    ui->dir_tree_view->resizeColumnToContents(0);
    if (app_settings.last_file != "")
    {
        QFile file(path);
        if (file.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            ui->current_file_text_edit->setPlainText(QString::fromUtf8(file.readAll()));
            if (app_settings.follow_tail)
            {
                ui->current_file_text_edit->verticalScrollBar()->setValue(ui->current_file_text_edit->verticalScrollBar()->maximum());
            }
            this->refresh_thread->SetStop(false);
            this->refresh_thread->start();
        }
        else
        {
            QMessageBox::critical(this, qAppName(), "Cannot open file:\n" + app_settings.last_file);
        }
    }
    this->UpdateCurrentStat();
}

void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open file", "", "Log files (*.log);;All files (*.*)");
    if (filename.isEmpty())
    {
        return;
    }
    OpenFile(filename);
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
    app_settings.SetFollowTail(checked);
}

void MainWindow::on_actionClose_triggered()
{
    this->StopRefreshThread();
    ui->current_file_label->setText("");
    ui->current_file_text_edit->setPlainText("");
    this->OpenFile("");
}

void MainWindow::StopRefreshThread()
{
    this->refresh_thread->SetStop(true);
    if (this->refresh_thread->isRunning())
    {
        this->refresh_thread->quit();
        this->refresh_thread->wait(1000);
    }
}

void MainWindow::on_actionClear_triggered()
{
    if (app_settings.last_file == "")
    {
        return;
    }
    if (app_settings.show_clear_log_confirmation)
    {
        QMessageBox::StandardButton answer =
                QMessageBox::question(this, qAppName(), "Are you sure want to clear the file content?", QMessageBox::Yes|QMessageBox::No);
        if (answer != QMessageBox::Yes)
        {
            return;
        }
    }
    QFile file(app_settings.last_file);
    if (file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        file.write("");
    }
    else
    {
        QMessageBox::critical(this, qAppName(), "Cannot open file:\n" + app_settings.last_file);
    }
}

void MainWindow::on_dir_tree_view_doubleClicked(const QModelIndex &index)
{
    QString path = this->dir_model->filePath(index);
    if (!path.endsWith(".log", Qt::CaseInsensitive))
    {
        return;
    }
    this->OpenFile(path);
}

void MainWindow::on_actionReload_triggered()
{    
    this->OpenFile(app_settings.last_file);
}

void MainWindow::on_actionOptions_triggered()
{
    options_dialog->SetSettings(app_settings);
    int result = options_dialog->exec();
    if (result)
    {
        options_dialog->UpdateSettings(app_settings);
        ui->mainToolBar->setVisible(app_settings.show_toolbar);
        ui->statusBar->setVisible(app_settings.show_status_bar);
    }
}

void MainWindow::on_actionSelectAll_triggered()
{
    ui->current_file_text_edit->setFocus();
    ui->current_file_text_edit->selectAll();
}

void MainWindow::on_actionCopy_triggered()
{
    QString selected_text = ui->current_file_text_edit->textCursor().selectedText();
    QApplication::clipboard()->setText(selected_text);
}

void MainWindow::on_actionFind_triggered()
{
    find_dialog->show();
}

void MainWindow::on_actionFindNext_triggered()
{
    find_dialog->findNext();
}

void MainWindow::on_actionFindPrevious_triggered()
{
    find_dialog->findPrev();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, qAppName(),
                       tr("A log viewer application.<br><br>") +
                       tr("Author: Vanpt"));
}

void MainWindow::on_actionFont_triggered()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, ui->current_file_text_edit->font(), this);
    if (ok)
    {
        ui->current_file_text_edit->setFont(font);
    }
}
