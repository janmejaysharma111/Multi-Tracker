#ifndef FRONTEND_H
#define FRONTEND_H

#include <QMainWindow>
#include <QString>

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

private:
    void updateCalendar();
    void openTracker(const QString &trackerName);

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
    int currentYear;
};

#endif // FRONTEND_H
