#include "appsettings.h"

AppSettings::AppSettings()
{

}
int AppSettings::GetRunTimes()
{
    return run_times;
}

void AppSettings::SetRunTimes(int value)
{
    if (run_times != value && value >= 0)
    {
        run_times = value;
    }
}

bool AppSettings::GetShowToolbar()
{
    return show_toolbar;
}

void AppSettings::SetShowToolbar(bool value)
{
    if (show_toolbar != value)
    {
        show_toolbar = value;
    }
}

bool AppSettings::GetShowStatusBar()
{
    return show_status_bar;
}

void AppSettings::SetShowStatusBar(bool value)
{
    if (show_status_bar != value)
    {
        show_status_bar = value;
    }
}

int AppSettings::GetReloadInterval()
{
    return reload_interval;
}

void AppSettings::SetReloadInterval(int value)
{
    if (reload_interval != value && value > 0)
    {
        reload_interval = value;
    }
}

bool AppSettings::GetFollowTail()
{
    return follow_tail;
}

void AppSettings::SetFollowTail(bool value)
{
    if (follow_tail != value)
    {
        follow_tail = value;
    }
}

bool AppSettings::GetWordWrap()
{
    return word_wrap;
}

void AppSettings::SetWordWrap(bool value)
{
    if (word_wrap != value)
    {
        word_wrap = value;
    }
}
