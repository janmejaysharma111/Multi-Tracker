#include<iostream>
using namespace std;

template<class T>
class Calender{
    public:
    T array[12][31];
};

template< class T1, class T2 >
class Hashmap{
    T1 key[30];
    T2 value [30];
    int num;

    Hashmap(){
        num = 0;
    }

    void add(T1 a, T2 b){
        key[num]=a;
        value[num]=b;
        num++;
    }
    
    T2* search(T1 a) {
        for (int i = 0; i < num; i++) {
            if (key[i] == a) {
                return &value[i];
            }
        }
        return nullptr;
    }
};

