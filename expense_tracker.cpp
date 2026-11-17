#include "expense_tracker.h"

ExpenseTracker :: ExpenseTracker() {
    type = "Expense";
    db_file_name = "expense_tracker.txt";
}

void ExpenseTracker :: setup_tracker(){
    db_file_name = "expense_tracker.txt";

    const vector<string> cat_names = {"food", "stationary", "travel", "lifestyle", "miscellaneous"};

    for(int m = 0; m < 12; m++){
        categories[m] = Hashmap<string, int>();
        for(const string &cat : cat_names){
            categories[m].add(cat, 0);
        }
        categories[m].add("total", 0);
    }

    for(int m = 0; m < 12; m++){
        for(int d = 0; d < 31; d++){
            daily_spend.array[m][d] = Hashmap<string, int>();
        }
    }

    vector<string> lines;
    fstream file(db_file_name, ios::in);
    if(file.is_open()){
        string line;
        while(getline(file, line)){
            if(!line.empty() && line.back() == '\r') line.pop_back();
            lines.push_back(line);
        }
        file.close();
    }

    if(lines.size() < 365){
        while(lines.size() < 365){
            lines.push_back("0,0,0,0,0");
        }
        fstream out(db_file_name, ios::out | ios::trunc);
        if(out.is_open()){
            for(const auto &l : lines){
                out << l << "\n";
            }
            out.close();
        }
    }

    for(int month = 1; month <= 12; month++){
        for(int day = 1; day <= 31; day++){
            if(!valid_day(month, day)) continue;

            int m_idx = month - 1;
            int d_idx = day - 1;

            int line_num = day_to_line(month, day);
            vector<int> expenses(5, 0);

            if(line_num >= 0 && line_num < (int)lines.size()){
                string curr_day_expense = lines[line_num];
                if(!curr_day_expense.empty()){
                    vector<string> expense_str = line_to_array(curr_day_expense);
                    for(int i = 0; i < 5 && i < (int)expense_str.size(); i++){
                        if(!expense_str[i].empty()){
                            try {
                                expenses[i] = stoi(expense_str[i]);
                            } catch(...) {
                                expenses[i] = 0;
                            }
                        }
                    }
                }
            }

            int day_total = 0;
            for(int i = 0; i < 5; i++){
                daily_spend.array[m_idx][d_idx].add(cat_names[i], expenses[i]);
                day_total += expenses[i];

                int cat_idx = categories[m_idx].search(cat_names[i]);
                if(cat_idx != -1){
                    categories[m_idx].value[cat_idx] += expenses[i];
                }
            }
            daily_spend.array[m_idx][d_idx].add("total", day_total);

            int cat_tot_idx = categories[m_idx].search("total");
            if(cat_tot_idx != -1){
                categories[m_idx].value[cat_tot_idx] += day_total;
            }
        }
    }
}

void ExpenseTracker :: add_to_db(){
    fstream out(db_file_name, ios::out | ios::trunc);
    if(!out.is_open()) return;
    const vector<string> cat_names = {"food", "stationary", "travel", "lifestyle", "miscellaneous"};
    for(int month = 1; month <= 12; month++){
        for(int day = 1; day <= 31; day++){
            if(!valid_day(month, day)) continue;
            int m_idx = month - 1;
            int d_idx = day - 1;
            string line = "";
            for(size_t i = 0; i < cat_names.size(); i++){
                int idx = daily_spend.array[m_idx][d_idx].search(cat_names[i]);
                int v = (idx != -1) ? daily_spend.array[m_idx][d_idx].value[idx] : 0;
                line += to_string(v);
                if(i + 1 < cat_names.size()) line += ",";
            }
            out << line << "\n";
        }
    }
    out.close();
}

void ExpenseTracker :: add_to_db(int month, int day, float val, string type){
    if(!valid_day(month, day)) return;
    int line_num = day_to_line(month, day);
    if(line_num < 0) return;

    vector<string> lines;
    fstream file(db_file_name, ios::in);
    if(file.is_open()){
        string line;
        while(getline(file, line)){
            if(!line.empty() && line.back() == '\r') line.pop_back();
            lines.push_back(line);
        }
        file.close();
    }

    while((int)lines.size() < 365){
        lines.push_back("0,0,0,0,0");
    }

    int m_idx = month - 1;
    int d_idx = day - 1;
    const vector<string> cat_names = {"food", "stationary", "travel", "lifestyle", "miscellaneous"};
    string new_line = "";
    for(size_t i = 0; i < cat_names.size(); i++){
        int idx = daily_spend.array[m_idx][d_idx].search(cat_names[i]);
        int v = (idx != -1) ? daily_spend.array[m_idx][d_idx].value[idx] : 0;
        new_line += to_string(v);
        if(i + 1 < cat_names.size()) new_line += ",";
    }
    lines[line_num] = new_line;

    fstream out(db_file_name, ios::out | ios::trunc);
    if(out.is_open()){
        for(const auto &l : lines){
            out << l << "\n";
        }
        out.close();
    }
}

void ExpenseTracker :: manage_history(const string &msg){
    fstream file("history.txt", ios::app);
    if(file.is_open()){
        if(msg.empty()){
            file << "[Expense Tracker] History updated\n";
        } else {
            file << "[Expense Tracker] " << msg << "\n";
        }
        file.close();
    }
}

void ExpenseTracker :: manage_history(){
    manage_history("History updated");
}

void ExpenseTracker :: add_expense(int month, int day, float val, string type){
    if(valid_day(month, day)){
        int m_idx = month - 1;
        int d_idx = day - 1;
        int int_val = (int)val;

        // Normalize type to lowercase
        for(char &c : type) c = tolower(c);

        int i = daily_spend.array[m_idx][d_idx].search(type);
        if(i != -1){
            daily_spend.array[m_idx][d_idx].value[i] += int_val;
        }else{
            daily_spend.array[m_idx][d_idx].add(type, int_val);
        }

        int tot_idx = daily_spend.array[m_idx][d_idx].search("total");
        if(tot_idx != -1){
            daily_spend.array[m_idx][d_idx].value[tot_idx] += int_val;
        }else{
            daily_spend.array[m_idx][d_idx].add("total", int_val);
        }

        int cat_i = categories[m_idx].search(type);
        if(cat_i != -1){
            categories[m_idx].value[cat_i] += int_val;
        }else{
            categories[m_idx].add(type, int_val);
        }

        int cat_tot = categories[m_idx].search("total");
        if(cat_tot != -1){
            categories[m_idx].value[cat_tot] += int_val;
        }else{
            categories[m_idx].add("total", int_val);
        }

        add_to_db(month, day, val, type);
        manage_history("This element is added to the db: " + type + " - ₹" + to_string(int_val) + " on day " + to_string(day) + "/" + to_string(month));
    }
}

float ExpenseTracker :: get_month_total(int month){
    if(month < 1 || month > 12) return 0.0f;
    int m_idx = month - 1;
    int cat_tot_idx = categories[m_idx].search("total");
    if(cat_tot_idx != -1){
        return (float)categories[m_idx].value[cat_tot_idx];
    }
    float sum = 0;
    for(int i = 0; i < 31 && valid_day(month, i + 1); i++){
        int k = daily_spend.array[m_idx][i].search("total");
        if(k != -1){
            sum += daily_spend.array[m_idx][i].value[k];
        }
    }
    return sum;
}

Hashmap<string,int> ExpenseTracker :: getCategoryTotal(int month){
    if(month < 1 || month > 12) return Hashmap<string, int>();
    return categories[month - 1];
}

Hashmap<string,int> ExpenseTracker :: get_daily_spend(int month, int day){
    if(!valid_day(month, day)) return Hashmap<string, int>();
    return daily_spend.array[month - 1][day - 1];
}
