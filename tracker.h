#include<iostream>
using namespace std;

class Tracker{
    private:
        string type;
        virtual void setup_tracker() = 0;
        virtual void add_to_db() = 0;
};