#ifndef APPSETTINGS_H
#define APPSETTINGS_H

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

private:
    int run_times = 0;
    bool show_toolbar = true;
    bool show_status_bar = true;
    int reload_interval = 1;
    bool follow_tail = true;
    bool word_wrap = false;
};

#endif // APPSETTINGS_H
