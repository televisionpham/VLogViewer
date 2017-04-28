#include "refreshthread.h"

RefreshThread::RefreshThread(QObject *parent, QString& path, QTextEdit *text_edit, int& interval) : QThread(parent), file_path(path), reload_interval(interval)
{
    this->text_edit = text_edit;
}

void RefreshThread::run()
{

}
