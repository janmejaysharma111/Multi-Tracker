#include "food_tracker.h"

FoodTracker::FoodTracker() {
    type = "Food";
    db_file_name = "food_tracker.txt";
}

void FoodTracker::setup_tracker() {
    db_file_name = "food_tracker.txt";

    for (int m = 0; m < 12; m++) {
        meal_counts[m] = Hashmap<string, int>();
        meal_counts[m].add("breakfast", 0);
        meal_counts[m].add("lunch", 0);
        meal_counts[m].add("dinner", 0);
        meal_counts[m].add("total", 0);
        meal_counts[m].add("full_days", 0);
    }

    for (int m = 0; m < 12; m++) {
        for (int d = 0; d < 31; d++) {
            daily_meals.array[m][d] = CheckList();
        }
    }

    vector<string> lines;
    fstream file(db_file_name, ios::in);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            lines.push_back(line);
        }
        file.close();
    }

    if (lines.size() < 365) {
        while (lines.size() < 365) {
            lines.push_back("0,,0,,0,");
        }
        fstream out(db_file_name, ios::out | ios::trunc);
        if (out.is_open()) {
            for (const auto &l : lines) {
                out << l << "\n";
            }
            out.close();
        }
    }

    for (int month = 1; month <= 12; month++) {
        int m_idx = month - 1;
        int bf_count = 0;
        int lunch_count = 0;
        int dinner_count = 0;
        int full_days = 0;

        for (int day = 1; day <= 31; day++) {
            if (!valid_day(month, day)) continue;
            int d_idx = day - 1;
            int line_num = day_to_line(month, day);

            if (line_num >= 0 && line_num < (int)lines.size()) {
                string curr_line = lines[line_num];
                if (!curr_line.empty()) {
                    vector<string> parts = line_to_array(curr_line);
                    bool bfd = (parts.size() > 0 && parts[0] == "1");
                    string bfn = (parts.size() > 1) ? parts[1] : "";
                    bool ld = (parts.size() > 2 && parts[2] == "1");
                    string ln = (parts.size() > 3) ? parts[3] : "";
                    bool dd = (parts.size() > 4 && parts[4] == "1");
                    string dn = (parts.size() > 5) ? parts[5] : "";

                    daily_meals.array[m_idx][d_idx] = CheckList(bfd, ld, dd, bfn, ln, dn);

                    if (bfd) bf_count++;
                    if (ld) lunch_count++;
                    if (dd) dinner_count++;
                    if (bfd && ld && dd) full_days++;
                }
            }
        }

        meal_counts[m_idx] = Hashmap<string, int>();
        meal_counts[m_idx].add("breakfast", bf_count);
        meal_counts[m_idx].add("lunch", lunch_count);
        meal_counts[m_idx].add("dinner", dinner_count);
        meal_counts[m_idx].add("total", bf_count + lunch_count + dinner_count);
        meal_counts[m_idx].add("full_days", full_days);
    }
}

void FoodTracker::add_to_db() {
    fstream out(db_file_name, ios::out | ios::trunc);
    if (!out.is_open()) return;

    for (int month = 1; month <= 12; month++) {
        for (int day = 1; day <= 31; day++) {
            if (!valid_day(month, day)) continue;
            int m_idx = month - 1;
            int d_idx = day - 1;
            CheckList cl = daily_meals.array[m_idx][d_idx];

            string bfn = cl.getBreakfastName();
            string ln = cl.getLunchName();
            string dn = cl.getDinnerName();
            for (char &c : bfn) if (c == ',') c = ' ';
            for (char &c : ln) if (c == ',') c = ' ';
            for (char &c : dn) if (c == ',') c = ' ';

            out << (cl.hasBreakfast() ? "1" : "0") << "," << bfn << ","
                << (cl.hasLunch() ? "1" : "0") << "," << ln << ","
                << (cl.hasDinner() ? "1" : "0") << "," << dn << "\n";
        }
    }
    out.close();
}

void FoodTracker::add_to_db(int month, int day) {
    if (!valid_day(month, day)) return;
    int line_num = day_to_line(month, day);
    if (line_num < 0) return;

    vector<string> lines;
    fstream file(db_file_name, ios::in);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            lines.push_back(line);
        }
        file.close();
    }

    while ((int)lines.size() < 365) {
        lines.push_back("0,,0,,0,");
    }

    int m_idx = month - 1;
    int d_idx = day - 1;
    CheckList cl = daily_meals.array[m_idx][d_idx];

    string bfn = cl.getBreakfastName();
    string ln = cl.getLunchName();
    string dn = cl.getDinnerName();
    for (char &c : bfn) if (c == ',') c = ' ';
    for (char &c : ln) if (c == ',') c = ' ';
    for (char &c : dn) if (c == ',') c = ' ';

    lines[line_num] = (cl.hasBreakfast() ? "1" : "0") + string(",") + bfn + "," +
                      (cl.hasLunch() ? "1" : "0") + "," + ln + "," +
                      (cl.hasDinner() ? "1" : "0") + "," + dn;

    fstream out(db_file_name, ios::out | ios::trunc);
    if (out.is_open()) {
        for (const auto &l : lines) {
            out << l << "\n";
        }
        out.close();
    }
}

void FoodTracker::manage_history() {
    fstream file("history.txt", ios::app);
    if (file.is_open()) {
        file << "Food tracker updated\n";
        file.close();
    }
}

void FoodTracker::set_daily_checklist(int month, int day, const CheckList &checklist) {
    if (!valid_day(month, day)) return;
    int m_idx = month - 1;
    int d_idx = day - 1;
    daily_meals.array[m_idx][d_idx] = checklist;

    // Refresh month counts
    int bf_count = 0, lunch_count = 0, dinner_count = 0, full_days = 0;
    for (int d = 1; d <= 31; d++) {
        if (!valid_day(month, d)) continue;
        CheckList cl = daily_meals.array[m_idx][d - 1];
        if (cl.hasBreakfast()) bf_count++;
        if (cl.hasLunch()) lunch_count++;
        if (cl.hasDinner()) dinner_count++;
        if (cl.hasBreakfast() && cl.hasLunch() && cl.hasDinner()) full_days++;
    }

    meal_counts[m_idx] = Hashmap<string, int>();
    meal_counts[m_idx].add("breakfast", bf_count);
    meal_counts[m_idx].add("lunch", lunch_count);
    meal_counts[m_idx].add("dinner", dinner_count);
    meal_counts[m_idx].add("total", bf_count + lunch_count + dinner_count);
    meal_counts[m_idx].add("full_days", full_days);

    add_to_db(month, day);
    manage_history();
}

void FoodTracker::update_meal(int month, int day, int mealIndex, bool had, const string &foodName) {
    if (!valid_day(month, day)) return;
    CheckList cl = get_daily_checklist(month, day);
    cl.setMeal(mealIndex, had, foodName);
    set_daily_checklist(month, day, cl);
}

CheckList FoodTracker::get_daily_checklist(int month, int day) {
    if (!valid_day(month, day)) return CheckList();
    return daily_meals.array[month - 1][day - 1];
}

int FoodTracker::get_month_total_meals(int month) {
    if (month < 1 || month > 12) return 0;
    int idx = meal_counts[month - 1].search("total");
    return (idx != -1) ? meal_counts[month - 1].value[idx] : 0;
}

Hashmap<string, int> FoodTracker::get_month_meal_counts(int month) {
    if (month < 1 || month > 12) return Hashmap<string, int>();
    return meal_counts[month - 1];
}
