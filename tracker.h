#pragma once
#include <iostream>
#include <string>
using namespace std;

class Tracker {
    protected:
        string type;
    public:
        virtual ~Tracker() = default;
        virtual void setup_tracker() = 0;
        virtual void add_to_db() = 0;
        virtual void manage_history() = 0;
        void print_current_tracker();
};
