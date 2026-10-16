#include<iostream>
using namespace std;

class Expense{
    float price;
    string type="";

    public:
    Expense(float p){
        price=p;
        type="Miscellaneous";
    }

    Expense(float p, string n){
        price=p;
        type=n;
    }
};

class Tracker{
    private:
        string type;
        virtual void get_tracker_id() = 0;
};

class ExpenseTracker : public Tracker{
    private:
        int daily_this_month[31];

};

class FoodTracker : public Tracker{
    private:
};

class FitnessTracker : public Tracker{
    private:
};

class StudyTracker : public Tracker{
    private:
};

template< class T1, class T2 >
class Hashmap{

};