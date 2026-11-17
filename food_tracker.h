#pragma once
#include "tracker.h"
#include "other_utilities.h"

// Forward declarations for friend functions
int countHealthyDays(const class FoodTracker &, const class FitnessTracker &, int);
struct DailyReport;
DailyReport getDailyReport(const class ExpenseTracker &, const class FitnessTracker &,
                           const class FoodTracker &, const class StudyTracker &,
                           int, int, int, int);

class FoodTracker : public Tracker {
    Calender<CheckList> daily_meals;
    Hashmap<string, int> meal_counts[12];
    string db_file_name = "";

    friend int countHealthyDays(const FoodTracker &, const class FitnessTracker &, int);
    friend DailyReport getDailyReport(const class ExpenseTracker &, const class FitnessTracker &,
                                      const FoodTracker &, const class StudyTracker &,
                                      int, int, int, int);

public:
    FoodTracker();
    void setup_tracker() override;
    void add_to_db() override;
    void manage_history() override;
    void manage_history(const string &msg) override;
    void add_to_db(int month, int day);
    void set_daily_checklist(int month, int day, const CheckList &checklist);
    void update_meal(int month, int day, int mealIndex, bool had, const string &foodName);
    CheckList get_daily_checklist(int month, int day);
    int get_month_total_meals(int month);
    Hashmap<string, int> get_month_meal_counts(int month);
};
