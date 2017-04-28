#ifndef REFRESHTHREAD_H
#define REFRESHTHREAD_H

#include <QString>
#include <QTextEdit>
#include <QThread>

class RefreshThread : public QThread
{
    Q_OBJECT
public:
    explicit RefreshThread(QObject* parent, QString& file_path, QTextEdit* text_edit, int& interval, bool& follow_tail);
    void SetStop(bool);

    // QThread interface
protected:
    void run();

private:
    const QString& file_path;
    QString previous_file_path = "";
    QTextEdit* text_edit;
    const int& reload_interval;
    const bool& follow_tail;
    long previous_file_size = 0;
    bool should_stop = false;
};

#endif // REFRESHTHREAD_H
