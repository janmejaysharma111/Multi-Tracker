#pragma once
#include "tracker.h"
#include "other_utilities.h"

class ExpenseTracker : public Tracker {
    Calender<Hashmap<string,int>> daily_spend;
    Hashmap<string,int> categories[12];
    string db_file_name = "";

public:
    ExpenseTracker();
    void setup_tracker() override;
    void add_to_db() override;
    void manage_history() override;
    void add_to_db(int month, int day, float val, string type);
    void add_expense(int month, int day, float val, string type);
    float get_month_total(int month);
    Hashmap<string,int> getCategoryTotal(int month);
    Hashmap<string,int> get_daily_spend(int month, int day);
};
