#pragma once
#include "food_tracker.h"
#include "fitness_tracker.h"
#include "study_tracker.h"
#include "expense_tracker.h"

// =====================================================================
// CROSS-TRACKER FRIEND FUNCTIONS
// Each function accesses private members of one or more tracker classes
// using the C++ friend mechanism to correlate data across trackers.
// =====================================================================

// Friend of FoodTracker AND FitnessTracker
// A day is "healthy" if: at least 2 meals logged AND at least 30 min activity
int countHealthyDays(const FoodTracker &food, const FitnessTracker &fitness, int month);

// Friend of StudyTracker
// Returns number of days where total study time >= targetMinutes
int countStudyTargetDays(const StudyTracker &study, int month, int targetMinutes);

// Friend of ExpenseTracker
// Returns total rupees saved (sum of budget-spend on days where spend < budget)
int calculateSavings(const ExpenseTracker &expense, int month, int dailyBudget);

// Friend of ALL FOUR trackers
// Returns a formatted summary string for a single day across all trackers
// (used for a full day report on the main screen or in a dialog)
struct DailyReport {
    int expenses;
    int activityMinutes;
    int mealsHad;
    int studyMinutes;
    bool isHealthy;
    bool studyTargetMet;
    bool underBudget;
};

DailyReport getDailyReport(
    const ExpenseTracker &et,
    const FitnessTracker &ft,
    const FoodTracker &food,
    const StudyTracker &st,
    int month, int day,
    int dailyBudget, int studyTarget
);
