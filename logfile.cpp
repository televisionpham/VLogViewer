#include "logfile.h"
#include <QFileInfo>

LogFile::LogFile(QWidget* parent, const QString& file_path)
{    
    this->file_path = file_path;
    QFileInfo fi(file_path);
    file_button = new QPushButton(fi.fileName(), parent);
}
