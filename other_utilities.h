#pragma once
#include<bits/stdc++.h>
using namespace std;

inline bool valid_day(int month, int day){
    if(month > 12 || month < 1 || day < 1) return false;
    if(month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12){
        if(day <= 31) return true;
    }else if(month == 2){
        if(day <= 28) return true;
    }else{
        if(day <= 30)return true;
    }
    return false;
}

inline int day_to_line(int month, int day){
    if(!valid_day(month,day)) return -1;
    const int daysBeforeMonth[] = {0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
    return daysBeforeMonth[month] + (day - 1);
}

inline vector<string> line_to_array(string str){
    string curr  = "";
    vector<string> output;
    for(char c : str){
        if(c == ','){
            output.push_back(curr);
            curr = "";
        }else{
            curr += c;
        }
    }
    output.push_back(curr);
    return output;
}

template<class T>
class Calender{
    public:
    T array[12][31];
};

template< class T1, class T2 >
class Hashmap{
    public:
    T1 key[30];
    T2 value[30];
    int num;
    Hashmap(){
        num = 0;
        for(int i=0 ; i<30 ; i++)value[i]=0;
    }

    int search(T1 a) {
        for (int i = 0; i < num; i++) {
            if (key[i] == a) {
                return i;
            }
        }
        return -1;
    }

    void add(T1 a, T2 b){
        key[num]=a;
        value[num]=b;
        num++;
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

    Time(int totalMinutes){
        hours = totalMinutes / 60;
        mins = totalMinutes % 60;
    }

    Time(int hour , int min){
        hours = hour + min / 60;
        mins = min % 60;
    }

    int getHours() const { return hours; }
    int getMins() const { return mins; }
    int getTotalMinutes() const { return hours * 60 + mins; }

    Time operator+(const Time &other) const {
        return Time(0, getTotalMinutes() + other.getTotalMinutes());
    }

    Time& operator+=(const Time &other) {
        int total = getTotalMinutes() + other.getTotalMinutes();
        hours = total / 60;
        mins = total % 60;
        return *this;
    }

    string toString() const {
        if (hours > 0) {
            return to_string(hours) + "h " + to_string(mins) + "m";
        }
        return to_string(mins) + "m";
    }
};

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

    bool hasBreakfast() const { return had_meal[0]; }
    bool hasLunch() const { return had_meal[1]; }
    bool hasDinner() const { return had_meal[2]; }
    bool hasMeal(int index) const {
        if(index >= 0 && index < 3) return had_meal[index];
        return false;
    }

    string getBreakfastName() const { return meal_name[0]; }
    string getLunchName() const { return meal_name[1]; }
    string getDinnerName() const { return meal_name[2]; }
    string getMealName(int index) const {
        if(index >= 0 && index < 3) return meal_name[index];
        return "";
    }

    void setBreakfast(bool had, const string &name = "") {
        had_meal[0] = had;
        meal_name[0] = name;
    }

    void setLunch(bool had, const string &name = "") {
        had_meal[1] = had;
        meal_name[1] = name;
    }

    void setDinner(bool had, const string &name = "") {
        had_meal[2] = had;
        meal_name[2] = name;
    }

    void setMeal(int index, bool had, const string &name = "") {
        if(index >= 0 && index < 3) {
            had_meal[index] = had;
            meal_name[index] = name;
        }
    }

    int totalMealsHad() const {
        return (had_meal[0] ? 1 : 0) + (had_meal[1] ? 1 : 0) + (had_meal[2] ? 1 : 0);
    }
};

