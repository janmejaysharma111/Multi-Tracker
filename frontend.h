#ifndef FRONTEND_H
#define FRONTEND_H

#include <QMainWindow>
#include <QString>
#include "tracker.h"
#include "expense_tracker.h"
#include "fitness_tracker.h"
#include "study_tracker.h"
#include "food_tracker.h"
#include "user_settings.h"
#include "cross_tracker_analysis.h"

class QWidget;
class QPushButton;
class QStackedWidget;
class QLabel;
class QGridLayout;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void openExpenseTracker();
    void openFitnessTracker();
    void openFoodTracker();
    void openStudyTracker();
    void goToPreviousMonth();
    void goToNextMonth();
    void backToMainMenu();
    void onOption1Clicked();
    void onOption2Clicked();
    void onOption3Clicked();
    void openSettingsDialog();
    void prevDashboardMonth();
    void nextDashboardMonth();

private:
    void updateCalendar();
    void updateDashboardStats();
    void openTracker(const QString &trackerName);
    void showDayDetails(int day);
    void showAddExpenseDialog(int preselectedDay = -1);
    void showFitnessDayDetails(int day);
    void showAddFitnessDialog(int preselectedDay = -1);
    void showStudyDayDetails(int day);
    void showAddStudyDialog(int preselectedDay = -1);
    void showFoodDayDetails(int day);
    void showLogMealDialog(int preselectedDay = -1);

    QWidget *centralWidget;
    QStackedWidget *pages;
    QWidget *mainPage;
    QWidget *trackerPage;

    QPushButton *expenseButton;
    QPushButton *fitnessButton;
    QPushButton *foodButton;
    QPushButton *studyButton;
    QPushButton *exitButton;
    QPushButton *settingsButton;

    QPushButton *prevStatsButton;
    QPushButton *nextStatsButton;
    QLabel *greetingLabel;
    QLabel *dashboardMonthLabel;
    QLabel *healthyDaysLabel;
    QLabel *studyTargetLabel;
    QLabel *savingsLabel;

    QLabel *trackerTitle;
    QLabel *monthLabel;
    QGridLayout *calendarLayout;
    QPushButton *previousButton;
    QPushButton *nextButton;
    QPushButton *backButton;
    QPushButton *option1;
    QPushButton *option2;
    QPushButton *option3;

    int currentMonth;
    int dashboardMonth;
    QString currentTracker;

    Tracker *activeTracker = nullptr;

    UserSettings userSettings;

    ExpenseTracker expenseTracker;
    FitnessTracker fitnessTracker;
    StudyTracker studyTracker;
    FoodTracker foodTracker;
};

#endif // FRONTEND_H
