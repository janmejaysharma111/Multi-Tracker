#include "tracker.h"
#include "other_utilities.h"

class ExpenseTracker : public Tracker{
        Calender<Hashmap<string,int>> daily_spend;
        Hashmap<string,int> categories[12];
        string db_file_name="";
        public:
        void setup_tracker();
        void add_to_db(int month, int day, float val, string type);
        void add_expense(int month, int day, float val, string type);
        float get_month_total(int month);
        Hashmap<string,int> getCategoryTotal(int month);

};
