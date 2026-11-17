#pragma once
#include "tracker.h"
#include "other_utilities.h"

// Forward declarations for friend functions
int countHealthyDays(const class FoodTracker &, const class FitnessTracker &, int);
struct DailyReport;
DailyReport getDailyReport(const class ExpenseTracker &, const class FitnessTracker &,
                           const class FoodTracker &, const class StudyTracker &,
                           int, int, int, int);

class FitnessTracker : public Tracker {
    Calender<Hashmap<string, int>> daily_activity;
    Hashmap<string, int> categories[12];
    string db_file_name = "";

    friend int countHealthyDays(const class FoodTracker &, const FitnessTracker &, int);
    friend DailyReport getDailyReport(const class ExpenseTracker &, const FitnessTracker &,
                                      const class FoodTracker &, const class StudyTracker &,
                                      int, int, int, int);

public:
    FitnessTracker();
    void setup_tracker() override;
    void add_to_db() override;
    void manage_history() override;
    void manage_history(const string &msg) override;
    void add_to_db(int month, int day, float val, string type);
    void add_activity(int month, int day, float val, string type);
    void add_activity(int month, int day, Time timeVal, string type);
    float get_month_total(int month);
    Time get_month_total_time(int month);
    Time get_daily_total_time(int month, int day);
    Hashmap<string, int> getCategoryTotal(int month);
    Hashmap<string, int> get_daily_activity(int month, int day);
};
