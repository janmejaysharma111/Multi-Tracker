#include "tracker.h"
#include "base_objects.h"
#include "other_utilities.h"

class ExpenseTracker : public Tracker{
        Calender<int> daily_spend;
        Hashmap<string,int> categories;
        public:

        bool valid_day(int month, int day);
        void add_expense(int month, int day, float value);
        float get_month_total(int month);

};
