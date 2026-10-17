#include "expense_tracker.h"

bool ExpenseTracker :: valid_day(int month, int day){
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

void ExpenseTracker :: add_expense(int month, int day, float value){
    if(valid_day(month,day)){
        day = day-1;
           month = month-1;
           daily_spend.array[month][day] = value;
   }
}

float ExpenseTracker :: get_month_total(int month){
    float sum=0;
    for(int i=0 ; i<31 && valid_day(month,i+1) ; i++){
        month = month-1;
        sum+=daily_spend.array[month][i];
    }
    return sum;
}