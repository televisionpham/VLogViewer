#ifndef LOGFILE_H
#define LOGFILE_H

#include <QWidget>
#include <QPushButton>

class LogFile
{
public:
    explicit LogFile(QWidget* parent, const QString& file_path);
    QString file_path;
    QPushButton* file_button;
};

#endif // LOGFILE_H
