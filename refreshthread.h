#ifndef REFRESHTHREAD_H
#define REFRESHTHREAD_H

#include <QString>
#include <QTextEdit>
#include <QThread>

class RefreshThread : public QThread
{
    Q_OBJECT
public:
    explicit RefreshThread(QObject* parent, QString& file_path, QTextEdit* text_edit, int& interval);

    // QThread interface
protected:
    void run();

private:
    QString& file_path;
    QTextEdit* text_edit;
    int& reload_interval;
    long previous_file_size = 0;
};

#endif // REFRESHTHREAD_H
