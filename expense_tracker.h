#include "tracker.h"
#include "base_objects.h"

class ExpenseTracker : public Tracker{
        Calender<Hashmap<string,int>> daily_spend;
        Hashmap<string,int> categories;
        public:
        void add_expense(int month, int day, float val, string type);
        float get_month_total(int month);

};
