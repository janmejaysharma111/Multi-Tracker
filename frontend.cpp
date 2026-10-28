#include "frontend.h"
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <QFont>
#include <QApplication>
#include <QDate>
#include <QLayoutItem>
#include <QStackedWidget>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("OOPS Multi-Tracker");
    resize(1000, 650);

    // Track active date state variables
    currentMonth = QDate::currentDate().month();
    currentYear = QDate::currentDate().year();

    // Central widget and page manager
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *rootLayout = new QVBoxLayout(centralWidget);
    pages = new QStackedWidget(centralWidget);
    rootLayout->addWidget(pages);

    // ================= MAIN MENU =================
    mainPage = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(mainPage);

    QLabel *title = new QLabel("OOPS MULTI-TRACKER");
    title->setAlignment(Qt::AlignCenter);
    QFont titleFont;
    titleFont.setPointSize(22);
    titleFont.setBold(true);
    title->setFont(titleFont);
    layout->addWidget(title);

    QLabel *subtitle = new QLabel("Choose a tracker to get started");
    subtitle->setAlignment(Qt::AlignCenter);
    layout->addWidget(subtitle);

    expenseButton = new QPushButton("Expense Tracker");
    fitnessButton = new QPushButton("Fitness Tracker");
    foodButton = new QPushButton("Food Tracker");
    studyButton = new QPushButton("Study Tracker");
    createNewButton = new QPushButton("Create New Tracker");
    exitButton = new QPushButton("Exit");

    expenseButton->setMinimumHeight(45);
    fitnessButton->setMinimumHeight(45);
    foodButton->setMinimumHeight(45);
    studyButton->setMinimumHeight(45);
    createNewButton->setMinimumHeight(50);

    layout->addStretch();
    layout->addWidget(expenseButton);
    layout->addWidget(fitnessButton);
    layout->addWidget(foodButton);
    layout->addWidget(studyButton);
    layout->addWidget(createNewButton);
    layout->addStretch();
    layout->addWidget(exitButton);
    pages->addWidget(mainPage);

    // ================= TRACKER PAGE =================
    trackerPage = new QWidget;
    QVBoxLayout *trackerLayout = new QVBoxLayout(trackerPage);

    trackerTitle = new QLabel;
    trackerTitle->setAlignment(Qt::AlignCenter);
    QFont trackerFont;
    trackerFont.setPointSize(22);
    trackerFont.setBold(true);
    trackerTitle->setFont(trackerFont);
    trackerLayout->addWidget(trackerTitle);

    // Month navigation
    QHBoxLayout *monthLayout = new QHBoxLayout();
    previousButton = new QPushButton("<");
    nextButton = new QPushButton(">");
    monthLabel = new QLabel;
    monthLabel->setAlignment(Qt::AlignCenter);
    QFont monthFont;
    monthFont.setPointSize(16);
    monthFont.setBold(true);
    monthLabel->setFont(monthFont);

    monthLayout->addWidget(previousButton);
    monthLayout->addWidget(monthLabel, 1);
    monthLayout->addWidget(nextButton);
    trackerLayout->addLayout(monthLayout);

    // Calendar and action buttons
    QHBoxLayout *contentLayout = new QHBoxLayout();
    calendarLayout = new QGridLayout();
    calendarLayout->setSpacing(5);

    QWidget *calendarWidget = new QWidget;
    calendarWidget->setLayout(calendarLayout);
    contentLayout->addWidget(calendarWidget, 3);

    QVBoxLayout *actionsLayout = new QVBoxLayout();
    option1 = new QPushButton("Option 1");
    option2 = new QPushButton("Option 2");
    option3 = new QPushButton("Option 3");
    backButton = new QPushButton("Back");

    option1->setMinimumHeight(45);
    option2->setMinimumHeight(45);
    option3->setMinimumHeight(45);

    actionsLayout->addStretch();
    actionsLayout->addWidget(option1);
    actionsLayout->addWidget(option2);
    actionsLayout->addWidget(option3);
    actionsLayout->addStretch();
    actionsLayout->addWidget(backButton);
    contentLayout->addLayout(actionsLayout, 1);
    trackerLayout->addLayout(contentLayout);
    pages->addWidget(trackerPage);

    // ================= INITIALIZE CALENDAR =================
    updateCalendar();

    // ================= SIGNALS & SLOTS =================
    // Dashboard Navigation Slots
    connect(expenseButton, &QPushButton::clicked, this, &MainWindow::openExpenseTracker);
    connect(fitnessButton, &QPushButton::clicked, this, &MainWindow::openFitnessTracker);
    connect(foodButton, &QPushButton::clicked, this, &MainWindow::openFoodTracker);
    connect(studyButton, &QPushButton::clicked, this, &MainWindow::openStudyTracker);
    connect(createNewButton, &QPushButton::clicked, this, &MainWindow::createNewTracker);
    connect(exitButton, &QPushButton::clicked, qApp, &QApplication::quit);

    // Calendar Navigation Slots
    connect(previousButton, &QPushButton::clicked, this, &MainWindow::goToPreviousMonth);
    connect(nextButton, &QPushButton::clicked, this, &MainWindow::goToNextMonth);
    connect(backButton, &QPushButton::clicked, this, &MainWindow::backToMainMenu);

    // Feature Options (Coming soon)
    connect(option1, &QPushButton::clicked, this, [this]() { QMessageBox::information(this, "Option 1", "Coming soon."); });
    connect(option2, &QPushButton::clicked, this, [this]() { QMessageBox::information(this, "Option 2", "Coming soon."); });
    connect(option3, &QPushButton::clicked, this, [this]() { QMessageBox::information(this, "Option 3", "Coming soon."); });
}

MainWindow::~MainWindow() {}

// Helper routing function
void MainWindow::openTracker(const QString &trackerName) {
    trackerTitle->setText(trackerName);
    pages->setCurrentWidget(trackerPage);
}

// Slots Implementations
void MainWindow::openExpenseTracker() { openTracker("Expense Tracker"); }
void MainWindow::openFitnessTracker() { openTracker("Fitness Tracker"); }
void MainWindow::openFoodTracker()    { openTracker("Food Tracker"); }
void MainWindow::openStudyTracker()   { openTracker("Study Tracker"); }
void MainWindow::createNewTracker()   { QMessageBox::information(this, "New Tracker", "Coming soon."); }

void MainWindow::backToMainMenu() {
    pages->setCurrentWidget(mainPage);
}

void MainWindow::goToPreviousMonth() {
    if (--currentMonth < 1) {
        currentMonth = 12;
        --currentYear;
    }
    updateCalendar();
}

void MainWindow::goToNextMonth() {
    if (++currentMonth > 12) {
        currentMonth = 1;
        --currentYear;
    }
    updateCalendar();
}

// Calendar Generator Engine
void MainWindow::updateCalendar() {
    QLayoutItem *item;
    while ((item = calendarLayout->takeAt(0)) != nullptr) {
        if (item->widget()) delete item->widget();
        delete item;
    }

    QDate firstDay(currentYear, currentMonth, 1);
    monthLabel->setText(firstDay.toString("MMMM yyyy"));

    QStringList weekdays = { "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };
    for (int col = 0; col < 7; ++col) {
        QLabel *label = new QLabel(weekdays[col]);
        label->setAlignment(Qt::AlignCenter);
        label->setStyleSheet("font-weight: bold;");
        calendarLayout->addWidget(label, 0, col);
    }

    int offset = firstDay.dayOfWeek() - 1;
    for (int day = 1; day <= firstDay.daysInMonth(); ++day) {
        int index = offset + day - 1;
        int row = index / 7 + 1;
        int col = index % 7;

        QPushButton *dateButton = new QPushButton(QString::number(day));
        dateButton->setMinimumSize(40, 40);
        calendarLayout->addWidget(dateButton, row, col);

        connect(dateButton, &QPushButton::clicked, this, [this, day]() {
            QMessageBox::information(
                this,
                "Selected Date",
                QDate(currentYear, currentMonth, day).toString("dd MMMM yyyy")
                );
        });
    }
}
