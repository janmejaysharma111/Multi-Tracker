#include<iostream>
#include "other_utilities.h"
using namespace std;

//Expense class is used by expense tracker
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

// Time class will be used by Study and fitness tracker
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

// Checklist function will be used by food tracker
class CheckList{
    bool had_meal[3];
    string meal_name[3];

    public:
    CheckList(){
        had_meal[0]=had_meal[1]=had_meal[2]=0;
        meal_name[0]=meal_name[1]=meal_name[2]="";
    }

    CheckList(bool bfd, bool ld, bool dd, string bfn , string ln, string dn){
        had_meal[0]=bfd;
        had_meal[1]=ld;
        had_meal[2]=dd;
        meal_name[0]=bfn;
        meal_name[1]=ln;
        meal_name[2]=dn;
    }
};