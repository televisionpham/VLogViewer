#include "refreshthread.h"
#include <QFileInfo>
#include <QScrollBar>
#include <QDebug>

RefreshThread::RefreshThread(QObject *parent, QString& path, QTextEdit *text_edit, int& interval, bool& follow_tail)
    : QThread(parent), file_path(path), reload_interval(interval), follow_tail(follow_tail)
{
    this->text_edit = text_edit;
    QFileInfo fi(path);
    this->previous_file_size = fi.size();
}

void RefreshThread::SetStop(bool value)
{
    this->should_stop = value;
}

void RefreshThread::run()
{
    while (true) {
        if (this->should_stop)
        {
            return;
        }
        if (this->file_path.isNull() || this->file_path.isEmpty())
        {
            msleep(this->reload_interval * 1000);
            continue;
        }
        if (this->previous_file_path != this->file_path)
        {
            QFileInfo fi(this->file_path);
            this->previous_file_size = fi.size();
            this->previous_file_path = this->file_path;
             msleep(this->reload_interval * 1000);
            continue;
        }
        QFileInfo fi(this->file_path);
        long current_file_size = fi.size();
        if (current_file_size == this->previous_file_size)
        {
            msleep(this->reload_interval * 1000);
            continue;
        }
        if (current_file_size < this->previous_file_size)
        {
            QFile file(this->file_path);
            if (file.open(QIODevice::ReadOnly|QIODevice::Text))
            {
                QString content = QString::fromUtf8(file.readAll());
                QMetaObject::invokeMethod(
                            this->text_edit,
                            "setPlainText",
                            Qt::QueuedConnection,
                            Q_ARG(QString, content));
                if (this->follow_tail)
                {
                    int max_value = this->text_edit->verticalScrollBar()->maximum();
                    QMetaObject::invokeMethod(
                                this->text_edit->verticalScrollBar(),
                                "setValue",
                                Qt::QueuedConnection,
                                Q_ARG(int, max_value));
                }
            }
            else
            {
                qDebug() << "Cannot open file: " << this->file_path;
            }
        }
        else if (current_file_size > this->previous_file_size)
        {
            QFile file(this->file_path);
            if (file.open(QIODevice::ReadOnly|QIODevice::Text))
            {
                file.seek(this->previous_file_size );
                long diff = current_file_size - this->previous_file_size;
                QString content = QString::fromUtf8(file.read(diff));
                this->text_edit->moveCursor(QTextCursor::End);
                QMetaObject::invokeMethod(
                            this->text_edit,
                            "insertPlainText",
                            Qt::QueuedConnection,
                            Q_ARG(QString, content));
            }
            else
            {
                qDebug() << "Cannot open file: " << this->file_path;
            }
        }
        this->previous_file_size = current_file_size;
        msleep(this->reload_interval * 1000);
    }
}
