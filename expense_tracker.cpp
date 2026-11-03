#include "expense_tracker.h"

void ExpenseTracker :: setup_tracker(){

}

void ExpenseTracker :: add_to_db(int month, int day, float val, string type){
    
}

void ExpenseTracker :: add_expense(int month, int day, float val, string type){
    if(valid_day(month,day)){
        day = day-1;
        month = month-1;
        int i= daily_spend.array[month][day].search(type);
        daily_spend.array[month][day].value[i]+=val;
   }
   add_to_db(month,day,val,type);
}

float ExpenseTracker :: get_month_total(int month){
    float sum=0;
    for(int i=0 ; i<31 && valid_day(month,i+1) ; i++){
        month = month-1;
        int k = daily_spend.array[month][i+1].search("total");
        sum+= daily_spend.array[month][i+1].value[k];
    }
    return sum;
}

Hashmap<string,int> ExpenseTracker :: getCategoryTotal(int month){
    return categories[month - 1];
} 


