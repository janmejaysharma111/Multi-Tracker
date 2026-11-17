#pragma once
#include "tracker.h"
#include "other_utilities.h"

class StudyTracker : public Tracker {
    Calender<Hashmap<string, int>> daily_study;
    Hashmap<string, int> subjects[12];
    string db_file_name = "";

public:
    StudyTracker();
    void setup_tracker() override;
    void add_to_db() override;
    void manage_history() override;
    void add_to_db(int month, int day, float val, string type);
    void add_study_time(int month, int day, float val, string type);
    void add_study_time(int month, int day, Time timeVal, string type);
    float get_month_total(int month);
    Time get_month_total_time(int month);
    Time get_daily_total_time(int month, int day);
    Hashmap<string, int> getSubjectTotal(int month);
    Hashmap<string, int> get_daily_study(int month, int day);
};
