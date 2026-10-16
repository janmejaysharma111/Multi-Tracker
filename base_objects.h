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

class Time{
    int hours;
    int mins;

    public:
    Time(){
        hours = 0;
        mins = 0;
    }

    Time(int hour , int min){
        hours = hour+min/60;
        mins = min%60;
    }
};

