#pragma once
#include <string>
#include <fstream>
using namespace std;

class UserSettings {
    string userName;
    int studyTargetMinutes;  // daily study target in minutes
    int dailyBudget;         // daily budget in rupees
    string settingsFile = "settings.txt";

public:
    UserSettings() : userName("User"), studyTargetMinutes(120), dailyBudget(500) {
        load();
    }

    // Getters
    string getUserName() const { return userName; }
    int getStudyTargetMinutes() const { return studyTargetMinutes; }
    int getDailyBudget() const { return dailyBudget; }

    // Setters
    void setUserName(const string &name) { userName = name; }
    void setStudyTargetMinutes(int mins) { studyTargetMinutes = mins; }
    void setDailyBudget(int budget) { dailyBudget = budget; }

    void save() {
        fstream file(settingsFile, ios::out | ios::trunc);
        if (file.is_open()) {
            file << userName << "\n";
            file << studyTargetMinutes << "\n";
            file << dailyBudget << "\n";
            file.close();
        }
    }

    void load() {
        fstream file(settingsFile, ios::in);
        if (file.is_open()) {
            string line;
            if (getline(file, line) && !line.empty()) {
                if (!line.empty() && line.back() == '\r') line.pop_back();
                userName = line;
            }
            if (getline(file, line) && !line.empty()) {
                try { studyTargetMinutes = stoi(line); } catch (...) { studyTargetMinutes = 120; }
            }
            if (getline(file, line) && !line.empty()) {
                try { dailyBudget = stoi(line); } catch (...) { dailyBudget = 500; }
            }
            file.close();
        }
    }
};
