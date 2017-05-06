#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QString>

class AppSettings
{
public:
    AppSettings();
public:
    int GetRunTimes();
    void SetRunTimes(int);
    bool GetShowToolbar();
    void SetShowToolbar(bool);
    bool GetShowStatusBar();
    void SetShowStatusBar(bool);
    int GetReloadInterval();
    void SetReloadInterval(int);
    bool GetFollowTail();
    void SetFollowTail(bool);
    bool GetWordWrap();
    void SetWordWrap(bool);
    int reload_interval = 1;
    bool follow_tail = true;
    bool show_toolbar = true;
    bool show_status_bar = true;
    bool word_wrap = false;
    bool show_exit_confirmation = true;
    bool show_clear_log_confirmation = true;
    QString last_file = "";

private:
    int run_times = 0;    
};

#endif // APPSETTINGS_H
