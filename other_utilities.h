#include<iostream>
using namespace std;

bool valid_day(int month, int day){
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

