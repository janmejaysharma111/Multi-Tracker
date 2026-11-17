#ifndef FRONTEND_H
#define FRONTEND_H

#include <QMainWindow>
#include <QString>
#include "expense_tracker.h"
#include "fitness_tracker.h"
#include "study_tracker.h"
#include "food_tracker.h"

// Forward declarations to keep compilation fast
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
    void createNewTracker();
    void goToPreviousMonth();
    void goToNextMonth();
    void backToMainMenu();
    void onOption1Clicked();
    void onOption2Clicked();
    void onOption3Clicked();

private:
    void updateCalendar();
    void openTracker(const QString &trackerName);
    void showDayDetails(int day);
    void showAddExpenseDialog(int preselectedDay = -1);
    void showFitnessDayDetails(int day);
    void showAddFitnessDialog(int preselectedDay = -1);
    void showStudyDayDetails(int day);
    void showAddStudyDialog(int preselectedDay = -1);
    void showFoodDayDetails(int day);
    void showLogMealDialog(int preselectedDay = -1);

    // Layout and Page Management
    QWidget *centralWidget;
    QStackedWidget *pages;
    QWidget *mainPage;
    QWidget *trackerPage;

    // Main Dashboard Buttons
    QPushButton *expenseButton;
    QPushButton *fitnessButton;
    QPushButton *foodButton;
    QPushButton *studyButton;
    QPushButton *createNewButton;
    QPushButton *exitButton;

    // Tracker UI Elements
    QLabel *trackerTitle;
    QLabel *monthLabel;
    QGridLayout *calendarLayout;
    QPushButton *previousButton;
    QPushButton *nextButton;
    QPushButton *backButton;
    QPushButton *option1;
    QPushButton *option2;
    QPushButton *option3;

    // State Variables
    int currentMonth;
    QString currentTracker;

    // Expense Tracker Model
    ExpenseTracker expenseTracker;

    // Fitness Tracker Model
    FitnessTracker fitnessTracker;

    // Study Tracker Model
    StudyTracker studyTracker;

    // Food Tracker Model
    FoodTracker foodTracker;
};

#endif // FRONTEND_H
