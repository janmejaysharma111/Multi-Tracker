#include<iostream>
using namespace std;

class Tracker{
    private:
        string type;
        virtual void get_tracker_id() = 0;
};