#include "cross_tracker_analysis.h"

// =====================================================================
// int countHealthyDays(FoodTracker, FitnessTracker, month)
// Friend of: FoodTracker, FitnessTracker
// Logic: Day is healthy if >= 2 meals logged AND >= 30 mins of activity
// =====================================================================
int countHealthyDays(const FoodTracker &food, const FitnessTracker &fitness, int month) {
    if (month < 1 || month > 12) return 0;
    int m_idx = month - 1;
    int healthyCount = 0;

    for (int d = 0; d < 31; d++) {
        // Access private members of FoodTracker directly (friend)
        const CheckList &cl = food.daily_meals.array[m_idx][d];
        int mealsHad = (cl.hasBreakfast() ? 1 : 0)
                     + (cl.hasLunch()     ? 1 : 0)
                     + (cl.hasDinner()    ? 1 : 0);

        // Access private members of FitnessTracker directly (friend)
        const Hashmap<string, int> &activity = fitness.daily_activity.array[m_idx][d];
        int tot_idx = activity.search("total");
        int activityMins = (tot_idx != -1) ? activity.value[tot_idx] : 0;

        if (mealsHad >= 2 && activityMins >= 30) {
            healthyCount++;
        }
    }
    return healthyCount;
}

// =====================================================================
// int countStudyTargetDays(StudyTracker, month, targetMinutes)
// Friend of: StudyTracker
// Logic: Count days where total study time >= targetMinutes
// =====================================================================
int countStudyTargetDays(const StudyTracker &study, int month, int targetMinutes) {
    if (month < 1 || month > 12) return 0;
    int m_idx = month - 1;
    int achievedCount = 0;

    for (int d = 0; d < 31; d++) {
        // Access private members of StudyTracker directly (friend)
        const Hashmap<string, int> &dayStudy = study.daily_study.array[m_idx][d];
        int tot_idx = dayStudy.search("total");
        int studyMins = (tot_idx != -1) ? dayStudy.value[tot_idx] : 0;

        if (studyMins >= targetMinutes) {
            achievedCount++;
        }
    }
    return achievedCount;
}

// =====================================================================
// int calculateSavings(ExpenseTracker, month, dailyBudget)
// Friend of: ExpenseTracker
// Logic: Sum (budget - spend) for all days where spend < dailyBudget
// =====================================================================
int calculateSavings(const ExpenseTracker &expense, int month, int dailyBudget) {
    if (month < 1 || month > 12) return 0;
    int m_idx = month - 1;
    int totalSavings = 0;

    for (int d = 0; d < 31; d++) {
        // Access private members of ExpenseTracker directly (friend)
        const Hashmap<string, int> &spend = expense.daily_spend.array[m_idx][d];
        int tot_idx = spend.search("total");
        int daySpend = (tot_idx != -1) ? spend.value[tot_idx] : 0;

        if (daySpend > 0 && daySpend < dailyBudget) {
            totalSavings += (dailyBudget - daySpend);
        }
    }
    return totalSavings;
}

// =====================================================================
// DailyReport getDailyReport(all 4 trackers, month, day, budget, target)
// Friend of: ALL FOUR trackers
// Logic: Aggregate all 4 tracker data for a single day into one struct
// =====================================================================
DailyReport getDailyReport(
    const ExpenseTracker &et,
    const FitnessTracker &ft,
    const FoodTracker &food,
    const StudyTracker &st,
    int month, int day,
    int dailyBudget, int studyTarget)
{
    DailyReport report{};
    if (month < 1 || month > 12 || day < 1 || day > 31) return report;
    int m_idx = month - 1;
    int d_idx = day - 1;

    // Expense (friend of ExpenseTracker)
    const Hashmap<string, int> &spend = et.daily_spend.array[m_idx][d_idx];
    int s_idx = spend.search("total");
    report.expenses = (s_idx != -1) ? spend.value[s_idx] : 0;

    // Fitness (friend of FitnessTracker)
    const Hashmap<string, int> &activity = ft.daily_activity.array[m_idx][d_idx];
    int a_idx = activity.search("total");
    report.activityMinutes = (a_idx != -1) ? activity.value[a_idx] : 0;

    // Food (friend of FoodTracker)
    const CheckList &cl = food.daily_meals.array[m_idx][d_idx];
    report.mealsHad = (cl.hasBreakfast() ? 1 : 0)
                    + (cl.hasLunch()     ? 1 : 0)
                    + (cl.hasDinner()    ? 1 : 0);

    // Study (friend of StudyTracker)
    const Hashmap<string, int> &studyDay = st.daily_study.array[m_idx][d_idx];
    int st_idx = studyDay.search("total");
    report.studyMinutes = (st_idx != -1) ? studyDay.value[st_idx] : 0;

    // Derived flags
    report.isHealthy       = (report.mealsHad >= 2 && report.activityMinutes >= 30);
    report.studyTargetMet  = (report.studyMinutes >= studyTarget);
    report.underBudget     = (report.expenses > 0 && report.expenses < dailyBudget);

    return report;
}
