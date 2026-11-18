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
#include <QLayoutItem>
#include <QStackedWidget>
#include <QDialog>
#include <QFormLayout>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QFrame>
#include <QSizePolicy>

static int getDaysInMonth(int month) {
    if (month == 2) return 28;
    if (month == 4 || month == 6 || month == 9 || month == 11) return 30;
    return 31;
}

static const QString MONTH_NAMES[] = {
    "", "January", "February", "March", "April", "May", "June",
    "July", "August", "September", "October", "November", "December"
};

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("OOPS Multi-Tracker");
    resize(1000, 650);

    currentMonth = 1;
    dashboardMonth = 1;
    currentTracker = "";

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *rootLayout = new QVBoxLayout(centralWidget);
    pages = new QStackedWidget(centralWidget);
    rootLayout->addWidget(pages);

    mainPage = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(mainPage);
    layout->setContentsMargins(30, 20, 30, 20);
    layout->setSpacing(10);

    QHBoxLayout *topBarLayout = new QHBoxLayout();
    greetingLabel = new QLabel(QString("Welcome, %1").arg(QString::fromStdString(userSettings.getUserName())));
    QFont greetFont;
    greetFont.setPointSize(13);
    greetFont.setBold(true);
    greetingLabel->setFont(greetFont);
    greetingLabel->setStyleSheet("color: #cbd5e1;");

    settingsButton = new QPushButton("Settings");
    settingsButton->setFixedSize(100, 36);
    settingsButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #1e293b;"
        "  color: #94a3b8;"
        "  border: 1.5px solid #334155;"
        "  border-radius: 0px;"
        "  font-size: 12px;"
        "  font-weight: 600;"
        "}"
        "QPushButton:hover {"
        "  background-color: #334155;"
        "  color: #f1f5f9;"
        "}"
    );
    topBarLayout->addWidget(greetingLabel);
    topBarLayout->addStretch();
    topBarLayout->addWidget(settingsButton);
    layout->addLayout(topBarLayout);

    QLabel *title = new QLabel("OOPS MULTI-TRACKER");
    title->setAlignment(Qt::AlignCenter);
    QFont titleFont;
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    title->setFont(titleFont);
    layout->addWidget(title);

    QHBoxLayout *statsRowLayout = new QHBoxLayout();
    statsRowLayout->setSpacing(8);

    prevStatsButton = new QPushButton("<");
    nextStatsButton = new QPushButton(">");
    prevStatsButton->setFixedWidth(44);
    nextStatsButton->setFixedWidth(44);
    prevStatsButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    nextStatsButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    prevStatsButton->setCursor(Qt::PointingHandCursor);
    nextStatsButton->setCursor(Qt::PointingHandCursor);
    prevStatsButton->setToolTip("Previous Month");
    nextStatsButton->setToolTip("Next Month");

    QString sideBtnStyle =
        "QPushButton {"
        "  background-color: #0f172a;"
        "  color: #94a3b8;"
        "  border: 1.5px solid #1e293b;"
        "  border-radius: 0px;"
        "  font-size: 20px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "  background-color: #1e293b;"
        "  border-color: #3b82f6;"
        "  color: #ffffff;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #2563eb;"
        "  color: #ffffff;"
        "}";

    prevStatsButton->setStyleSheet(sideBtnStyle);
    nextStatsButton->setStyleSheet(sideBtnStyle);

    QFrame *statsFrame = new QFrame;
    statsFrame->setFrameShape(QFrame::StyledPanel);
    statsFrame->setStyleSheet(
        "QFrame {"
        "  background-color: #0f172a;"
        "  border: 1.5px solid #1e293b;"
        "  border-radius: 0px;"
        "}"
    );

    QVBoxLayout *statsContainerLayout = new QVBoxLayout(statsFrame);
    statsContainerLayout->setContentsMargins(16, 10, 16, 10);
    statsContainerLayout->setSpacing(6);

    dashboardMonthLabel = new QLabel("JANUARY OVERVIEW");
    dashboardMonthLabel->setAlignment(Qt::AlignCenter);
    QFont monthTitleFont;
    monthTitleFont.setPointSize(12);
    monthTitleFont.setBold(true);
    dashboardMonthLabel->setFont(monthTitleFont);
    dashboardMonthLabel->setStyleSheet("color: #38bdf8; letter-spacing: 1.5px; font-weight: 700;");
    statsContainerLayout->addWidget(dashboardMonthLabel);

    QHBoxLayout *statsLayout = new QHBoxLayout();
    statsLayout->setSpacing(0);
    statsLayout->setContentsMargins(0, 0, 0, 0);

    auto makeStatWidget = [](const QString &icon, QLabel *&valueLabel, const QString &color) -> QWidget* {
        QWidget *cell = new QWidget;
        QVBoxLayout *v = new QVBoxLayout(cell);
        v->setSpacing(4);
        v->setContentsMargins(10, 4, 10, 4);

        QLabel *iconLbl = new QLabel(icon);
        iconLbl->setAlignment(Qt::AlignCenter);
        iconLbl->setStyleSheet(QString("color: %1; font-size: 14px; font-weight: bold; letter-spacing: 1.5px;").arg(color));

        valueLabel = new QLabel("--");
        valueLabel->setAlignment(Qt::AlignCenter);
        QFont vf; vf.setPointSize(22); vf.setBold(true);
        valueLabel->setFont(vf);
        valueLabel->setStyleSheet(QString("color: %1;").arg(color));

        v->addWidget(iconLbl);
        v->addWidget(valueLabel);
        return cell;
    };

    statsLayout->addWidget(makeStatWidget("HEALTHY DAYS", healthyDaysLabel, "#10b981"), 1);

    QFrame *sep1 = new QFrame; sep1->setFrameShape(QFrame::VLine);
    sep1->setStyleSheet("color: #1e293b;");
    statsLayout->addWidget(sep1);

    statsLayout->addWidget(makeStatWidget("STUDY TARGET DAYS", studyTargetLabel, "#8b5cf6"), 1);

    QFrame *sep2 = new QFrame; sep2->setFrameShape(QFrame::VLine);
    sep2->setStyleSheet("color: #1e293b;");
    statsLayout->addWidget(sep2);

    statsLayout->addWidget(makeStatWidget("SAVINGS", savingsLabel, "#f59e0b"), 1);

    statsContainerLayout->addLayout(statsLayout);
    statsFrame->setMinimumHeight(120);

    statsRowLayout->addWidget(prevStatsButton);
    statsRowLayout->addWidget(statsFrame, 1);
    statsRowLayout->addWidget(nextStatsButton);

    layout->addLayout(statsRowLayout);

    expenseButton = new QPushButton("Expense Tracker");
    fitnessButton = new QPushButton("Fitness Tracker");
    foodButton = new QPushButton("Food Tracker");
    studyButton = new QPushButton("Study Tracker");
    exitButton = new QPushButton("Exit");

    QFont btnFont;
    btnFont.setPointSize(16);
    btnFont.setBold(true);

    expenseButton->setFont(btnFont);
    fitnessButton->setFont(btnFont);
    foodButton->setFont(btnFont);
    studyButton->setFont(btnFont);

    expenseButton->setMinimumHeight(120);
    expenseButton->setMinimumWidth(260);
    fitnessButton->setMinimumHeight(120);
    fitnessButton->setMinimumWidth(260);
    foodButton->setMinimumHeight(120);
    foodButton->setMinimumWidth(260);
    studyButton->setMinimumHeight(120);
    studyButton->setMinimumWidth(260);

    expenseButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    fitnessButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    foodButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    studyButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    expenseButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #0f172a;"
        "  color: #f8fafc;"
        "  border: 2px solid #2563eb;"
        "  border-radius: 0px;"
        "  padding: 16px;"
        "  letter-spacing: 0.5px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #1e3a8a;"
        "  border-color: #3b82f6;"
        "  color: #ffffff;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #1d4ed8;"
        "}"
    );

    fitnessButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #022c22;"
        "  color: #f8fafc;"
        "  border: 2px solid #059669;"
        "  border-radius: 0px;"
        "  padding: 16px;"
        "  letter-spacing: 0.5px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #047857;"
        "  border-color: #10b981;"
        "  color: #ffffff;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #065f46;"
        "}"
    );

    foodButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #431407;"
        "  color: #f8fafc;"
        "  border: 2px solid #ea580c;"
        "  border-radius: 0px;"
        "  padding: 16px;"
        "  letter-spacing: 0.5px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #9a3412;"
        "  border-color: #f97316;"
        "  color: #ffffff;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #c2410c;"
        "}"
    );

    studyButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #2e1065;"
        "  color: #f8fafc;"
        "  border: 2px solid #7c3aed;"
        "  border-radius: 0px;"
        "  padding: 16px;"
        "  letter-spacing: 0.5px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #5b21b6;"
        "  border-color: #8b5cf6;"
        "  color: #ffffff;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #6d28d9;"
        "}"
    );

    QGridLayout *gridMenu = new QGridLayout();
    gridMenu->setSpacing(20);
    gridMenu->addWidget(expenseButton, 0, 0);
    gridMenu->addWidget(fitnessButton, 0, 1);
    gridMenu->addWidget(foodButton, 1, 0);
    gridMenu->addWidget(studyButton, 1, 1);

    QWidget *gridContainer = new QWidget;
    gridContainer->setLayout(gridMenu);
    gridContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    exitButton->setMinimumHeight(46);
    exitButton->setFixedWidth(180);
    exitButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #1e293b;"
        "  color: #cbd5e1;"
        "  border: 1.5px solid #475569;"
        "  border-radius: 0px;"
        "  font-size: 14px;"
        "  font-weight: 700;"
        "  letter-spacing: 0.5px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #7f1d1d;"
        "  color: #ffffff;"
        "  border-color: #ef4444;"
        "}"
    );

    layout->addWidget(gridContainer, 1);
    layout->addSpacing(8);
    layout->addWidget(exitButton, 0, Qt::AlignCenter);
    pages->addWidget(mainPage);

    expenseTracker.setup_tracker();
    fitnessTracker.setup_tracker();
    studyTracker.setup_tracker();
    foodTracker.setup_tracker();
    updateDashboardStats();

    trackerPage = new QWidget;
    QVBoxLayout *trackerLayout = new QVBoxLayout(trackerPage);
    trackerLayout->setContentsMargins(25, 20, 25, 20);
    trackerLayout->setSpacing(16);

    trackerTitle = new QLabel;
    trackerTitle->setAlignment(Qt::AlignCenter);
    QFont trackerFont;
    trackerFont.setPointSize(22);
    trackerFont.setBold(true);
    trackerTitle->setFont(trackerFont);
    trackerLayout->addWidget(trackerTitle);

    QHBoxLayout *monthLayout = new QHBoxLayout();
    monthLayout->setSpacing(16);
    previousButton = new QPushButton("<");
    nextButton = new QPushButton(">");
    previousButton->setFixedSize(40, 36);
    nextButton->setFixedSize(40, 36);
    previousButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #ffffff;"
        "  border: 1.5px solid #cbd5e1;"
        "  border-radius: 8px;"
        "  font-size: 15px;"
        "  font-weight: bold;"
        "  color: #334155;"
        "}"
        "QPushButton:hover { background-color: #f1f5f9; }"
    );
    nextButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #ffffff;"
        "  border: 1.5px solid #cbd5e1;"
        "  border-radius: 8px;"
        "  font-size: 15px;"
        "  font-weight: bold;"
        "  color: #334155;"
        "}"
        "QPushButton:hover { background-color: #f1f5f9; }"
    );

    monthLabel = new QLabel;
    monthLabel->setAlignment(Qt::AlignCenter);
    QFont monthFont;
    monthFont.setPointSize(17);
    monthFont.setBold(true);
    monthLabel->setFont(monthFont);

    monthLayout->addWidget(previousButton);
    monthLayout->addWidget(monthLabel, 1);
    monthLayout->addWidget(nextButton);
    trackerLayout->addLayout(monthLayout);

    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(24);

    calendarLayout = new QGridLayout();
    calendarLayout->setSpacing(8);

    QWidget *calendarWidget = new QWidget;
    calendarWidget->setLayout(calendarLayout);
    contentLayout->addWidget(calendarWidget, 4);

    QVBoxLayout *actionsLayout = new QVBoxLayout();
    actionsLayout->setSpacing(18);
    actionsLayout->setContentsMargins(15, 10, 15, 10);

    option1 = new QPushButton("Option 1");
    option2 = new QPushButton("Option 2");
    option3 = new QPushButton("Option 3");
    backButton = new QPushButton("Back to Menu");

    option1->setMinimumHeight(58);
    option2->setMinimumHeight(58);
    option3->setMinimumHeight(58);
    option1->setMinimumWidth(200);
    option2->setMinimumWidth(200);
    option3->setMinimumWidth(200);

    QFont optFont;
    optFont.setPointSize(13);
    optFont.setBold(true);
    option1->setFont(optFont);
    option2->setFont(optFont);
    option3->setFont(optFont);

    option1->setStyleSheet(
        "QPushButton {"
        "  background-color: #1e293b;"
        "  color: #ffffff;"
        "  border: none;"
        "  border-radius: 10px;"
        "  padding: 10px 18px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #334155;"
        "}"
    );

    option2->setStyleSheet(
        "QPushButton {"
        "  background-color: #ffffff;"
        "  color: #1e293b;"
        "  border: 1.5px solid #cbd5e1;"
        "  border-radius: 10px;"
        "  padding: 10px 18px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #f1f5f9;"
        "  border-color: #94a3b8;"
        "}"
    );

    option3->setStyleSheet(
        "QPushButton {"
        "  background-color: #ffffff;"
        "  color: #1e293b;"
        "  border: 1.5px solid #cbd5e1;"
        "  border-radius: 10px;"
        "  padding: 10px 18px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #f1f5f9;"
        "  border-color: #94a3b8;"
        "}"
    );

    backButton->setMinimumHeight(44);
    backButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #f8fafc;"
        "  color: #475569;"
        "  border: 1px solid #cbd5e1;"
        "  border-radius: 8px;"
        "  font-size: 13px;"
        "  font-weight: 600;"
        "}"
        "QPushButton:hover {"
        "  background-color: #e2e8f0;"
        "  color: #1e293b;"
        "}"
    );

    actionsLayout->addStretch();
    actionsLayout->addWidget(option1);
    actionsLayout->addWidget(option2);
    actionsLayout->addWidget(option3);
    actionsLayout->addStretch();
    actionsLayout->addWidget(backButton);

    QWidget *actionsContainer = new QWidget;
    actionsContainer->setLayout(actionsLayout);
    contentLayout->addWidget(actionsContainer, 2);

    trackerLayout->addLayout(contentLayout);
    pages->addWidget(trackerPage);

    updateCalendar();

    connect(expenseButton, &QPushButton::clicked, this, &MainWindow::openExpenseTracker);
    connect(fitnessButton, &QPushButton::clicked, this, &MainWindow::openFitnessTracker);
    connect(foodButton, &QPushButton::clicked, this, &MainWindow::openFoodTracker);
    connect(studyButton, &QPushButton::clicked, this, &MainWindow::openStudyTracker);
    connect(exitButton, &QPushButton::clicked, qApp, &QApplication::quit);
    connect(settingsButton, &QPushButton::clicked, this, &MainWindow::openSettingsDialog);

    connect(prevStatsButton, &QPushButton::clicked, this, &MainWindow::prevDashboardMonth);
    connect(nextStatsButton, &QPushButton::clicked, this, &MainWindow::nextDashboardMonth);

    connect(previousButton, &QPushButton::clicked, this, &MainWindow::goToPreviousMonth);
    connect(nextButton, &QPushButton::clicked, this, &MainWindow::goToNextMonth);
    connect(backButton, &QPushButton::clicked, this, &MainWindow::backToMainMenu);

    connect(option1, &QPushButton::clicked, this, &MainWindow::onOption1Clicked);
    connect(option2, &QPushButton::clicked, this, &MainWindow::onOption2Clicked);
    connect(option3, &QPushButton::clicked, this, &MainWindow::onOption3Clicked);
}

MainWindow::~MainWindow() {}

void MainWindow::prevDashboardMonth() {
    if (--dashboardMonth < 1) {
        dashboardMonth = 12;
    }
    updateDashboardStats();
}

void MainWindow::nextDashboardMonth() {
    if (++dashboardMonth > 12) {
        dashboardMonth = 1;
    }
    updateDashboardStats();
}

void MainWindow::updateDashboardStats() {
    int month = dashboardMonth;

    int healthy = countHealthyDays(foodTracker, fitnessTracker, month);
    int studyAchieved = countStudyTargetDays(studyTracker, month, userSettings.getStudyTargetMinutes());
    int savings = calculateSavings(expenseTracker, month, userSettings.getDailyBudget());

    QString monthName = MONTH_NAMES[dashboardMonth];

    dashboardMonthLabel->setText(monthName.toUpper() + " OVERVIEW");
    healthyDaysLabel->setText(QString::number(healthy));
    studyTargetLabel->setText(QString::number(studyAchieved));
    savingsLabel->setText(QString("\xe2\x82\xb9%1").arg(savings));
}

void MainWindow::openSettingsDialog() {
    QDialog dialog(this);
    dialog.setWindowTitle("Settings & Profile");
    dialog.resize(360, 240);
    dialog.setStyleSheet(
        "QDialog { background-color: #0f172a; color: #f1f5f9; }"
        "QLabel { color: #94a3b8; font-size: 13px; }"
        "QLineEdit, QSpinBox { background-color: #1e293b; color: #f1f5f9; border: 1px solid #334155;"
        "                      border-radius: 0px; padding: 4px; }"
        "QPushButton { background-color: #1e3a8a; color: #ffffff; border: none;"
        "              border-radius: 0px; padding: 8px 16px; font-weight: bold; }"
        "QPushButton:hover { background-color: #2563eb; }"
    );

    QFormLayout form(&dialog);
    form.setSpacing(14);
    form.setContentsMargins(24, 24, 24, 24);

    QLineEdit *nameEdit = new QLineEdit(&dialog);
    nameEdit->setText(QString::fromStdString(userSettings.getUserName()));
    nameEdit->setPlaceholderText("Enter your name");
    form.addRow("Your Name:", nameEdit);

    QSpinBox *studyHSpin = new QSpinBox(&dialog);
    studyHSpin->setRange(0, 12);
    studyHSpin->setSuffix(" hrs");
    studyHSpin->setValue(userSettings.getStudyTargetMinutes() / 60);

    QSpinBox *studyMSpin = new QSpinBox(&dialog);
    studyMSpin->setRange(0, 59);
    studyMSpin->setSuffix(" min");
    studyMSpin->setValue(userSettings.getStudyTargetMinutes() % 60);

    QHBoxLayout *studyTimeLayout = new QHBoxLayout;
    studyTimeLayout->addWidget(studyHSpin);
    studyTimeLayout->addWidget(studyMSpin);
    QWidget *studyWidget = new QWidget;
    studyWidget->setLayout(studyTimeLayout);
    form.addRow("Daily Study Target:", studyWidget);

    QSpinBox *budgetSpin = new QSpinBox(&dialog);
    budgetSpin->setRange(50, 100000);
    budgetSpin->setSingleStep(50);
    budgetSpin->setPrefix("\xe2\x82\xb9 ");
    budgetSpin->setValue(userSettings.getDailyBudget());
    form.addRow("Daily Budget:", budgetSpin);

    QDialogButtonBox buttons(QDialogButtonBox::Save | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    form.addRow(&buttons);

    connect(&buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        string newName = nameEdit->text().toStdString();
        if (newName.empty()) newName = "User";
        userSettings.setUserName(newName);
        userSettings.setStudyTargetMinutes(studyHSpin->value() * 60 + studyMSpin->value());
        userSettings.setDailyBudget(budgetSpin->value());
        userSettings.save();

        greetingLabel->setText(QString("Welcome, %1").arg(QString::fromStdString(newName)));
        updateDashboardStats();

        QMessageBox::information(this, "Settings Saved",
            QString("Profile updated!\nName: %1\nStudy Target: %2 hrs %3 min\nDaily Budget: \xe2\x82\xb9%4")
                .arg(QString::fromStdString(newName))
                .arg(studyHSpin->value())
                .arg(studyMSpin->value())
                .arg(budgetSpin->value()));
    }
}

void MainWindow::openTracker(const QString &trackerName) {
    trackerTitle->setText(trackerName);
    pages->setCurrentWidget(trackerPage);
}

void MainWindow::openExpenseTracker() {
    currentTracker = "Expense Tracker";
    currentMonth = 1;
    expenseTracker.setup_tracker();

    activeTracker = &expenseTracker;
    activeTracker->manage_history("This tracker is open");

    openTracker("Expense Tracker");

    option1->setText("Add Expense");
    option2->setText("Month Total");
    option3->setText("Category Totals");

    updateCalendar();
}

void MainWindow::openFitnessTracker() {
    currentTracker = "Fitness Tracker";
    currentMonth = 1;
    fitnessTracker.setup_tracker();

    activeTracker = &fitnessTracker;
    activeTracker->manage_history("This tracker is open");

    openTracker("Fitness Tracker");
    option1->setText("Add Activity");
    option2->setText("Month Total");
    option3->setText("Activity Breakdown");
    updateCalendar();
}

void MainWindow::openFoodTracker() {
    currentTracker = "Food Tracker";
    currentMonth = 1;
    foodTracker.setup_tracker();

    activeTracker = &foodTracker;
    activeTracker->manage_history("This tracker is open");

    openTracker("Food Tracker");
    option1->setText("Log Meals");
    option2->setText("Month Total");
    option3->setText("Meal Breakdown");
    updateCalendar();
}

void MainWindow::openStudyTracker() {
    currentTracker = "Study Tracker";
    currentMonth = 1;
    studyTracker.setup_tracker();

    activeTracker = &studyTracker;
    activeTracker->manage_history("This tracker is open");

    openTracker("Study Tracker");
    option1->setText("Add Study Time");
    option2->setText("Month Total");
    option3->setText("Subject Breakdown");
    updateCalendar();
}

void MainWindow::backToMainMenu() {
    if (activeTracker) {
        activeTracker->manage_history("This tracker was closed, returned to main menu");
        activeTracker = nullptr;
    }
    currentTracker = "";
    pages->setCurrentWidget(mainPage);
    updateDashboardStats();
}

void MainWindow::goToPreviousMonth() {
    if (--currentMonth < 1) {
        currentMonth = 12;
    }
    if (activeTracker) {
        activeTracker->manage_history("Navigated to " + MONTH_NAMES[currentMonth].toStdString());
    }
    updateCalendar();
}

void MainWindow::goToNextMonth() {
    if (++currentMonth > 12) {
        currentMonth = 1;
    }
    if (activeTracker) {
        activeTracker->manage_history("Navigated to " + MONTH_NAMES[currentMonth].toStdString());
    }
    updateCalendar();
}

void MainWindow::updateCalendar() {
    QLayoutItem *item;
    while ((item = calendarLayout->takeAt(0)) != nullptr) {
        if (item->widget()) delete item->widget();
        delete item;
    }

    monthLabel->setText(MONTH_NAMES[currentMonth]);
    int totalDays = getDaysInMonth(currentMonth);
    const int COLS = 7;

    if (currentTracker == "Expense Tracker") {
        expenseTracker.setup_tracker();

        int maxSpend = 0;
        for (int d = 1; d <= totalDays; ++d) {
            Hashmap<string, int> spend = expenseTracker.get_daily_spend(currentMonth, d);
            int tot_idx = spend.search("total");
            int dTot = (tot_idx != -1) ? spend.value[tot_idx] : 0;
            if (dTot > maxSpend) {
                maxSpend = dTot;
            }
        }

        for (int day = 1; day <= totalDays; ++day) {
            int index = day - 1;
            int row = index / COLS;
            int col = index % COLS;

            QPushButton *dateButton = new QPushButton();
            dateButton->setMinimumSize(54, 54);

            Hashmap<string, int> spend = expenseTracker.get_daily_spend(currentMonth, day);
            int tot_idx = spend.search("total");
            int dayTotal = (tot_idx != -1) ? spend.value[tot_idx] : 0;

            if (dayTotal > 0) {
                double ratio = (maxSpend > 0) ? (double)dayTotal / (double)maxSpend : 0.0;
                double t = 0.15 + 0.85 * ratio;

                int r = (int)(224 * (1.0 - t) + 30 * t);
                int g = (int)(242 * (1.0 - t) + 58 * t);
                int b = (int)(254 * (1.0 - t) + 138 * t);

                QString textColor = (t > 0.45) ? "#ffffff" : "#0f172a";

                dateButton->setText(QString("%1\n\xe2\x82\xb9%2").arg(day).arg(dayTotal));
                dateButton->setStyleSheet(QString(
                    "QPushButton {"
                    "  background-color: rgb(%1, %2, %3);"
                    "  color: %4;"
                    "  border: 1px solid rgb(%5, %6, %7);"
                    "  border-radius: 6px;"
                    "  font-weight: bold;"
                    "  font-size: 11px;"
                    "}"
                    "QPushButton:hover {"
                    "  border: 2px solid #2563eb;"
                    "}"
                ).arg(r).arg(g).arg(b)
                 .arg(textColor)
                 .arg(qMax(0, r - 35)).arg(qMax(0, g - 35)).arg(qMax(0, b - 35)));
            } else {
                dateButton->setText(QString::number(day));
                dateButton->setStyleSheet(
                    "QPushButton {"
                    "  background-color: #f8fafc;"
                    "  color: #334155;"
                    "  border: 1px solid #cbd5e1;"
                    "  border-radius: 6px;"
                    "  font-weight: 500;"
                    "  font-size: 13px;"
                    "}"
                    "QPushButton:hover {"
                    "  background-color: #f1f5f9;"
                    "  border: 1px solid #94a3b8;"
                    "}"
                );
            }

            connect(dateButton, &QPushButton::clicked, this, [this, day]() {
                showDayDetails(day);
            });

            calendarLayout->addWidget(dateButton, row, col);
        }
    } else if (currentTracker == "Fitness Tracker") {
        fitnessTracker.setup_tracker();

        int maxTime = 0;
        for (int d = 1; d <= totalDays; ++d) {
            Hashmap<string, int> activity = fitnessTracker.get_daily_activity(currentMonth, d);
            int tot_idx = activity.search("total");
            int dTot = (tot_idx != -1) ? activity.value[tot_idx] : 0;
            if (dTot > maxTime) {
                maxTime = dTot;
            }
        }

        for (int day = 1; day <= totalDays; ++day) {
            int index = day - 1;
            int row = index / COLS;
            int col = index % COLS;

            QPushButton *dateButton = new QPushButton();
            dateButton->setMinimumSize(54, 54);

            Hashmap<string, int> activity = fitnessTracker.get_daily_activity(currentMonth, day);
            int tot_idx = activity.search("total");
            int dayTotal = (tot_idx != -1) ? activity.value[tot_idx] : 0;

            if (dayTotal > 0) {
                double ratio = (maxTime > 0) ? (double)dayTotal / (double)maxTime : 0.0;
                double t = 0.15 + 0.85 * ratio;

                int r = (int)(220 * (1.0 - t) + 16 * t);
                int g = (int)(252 * (1.0 - t) + 120 * t);
                int b = (int)(231 * (1.0 - t) + 70 * t);

                QString textColor = (t > 0.45) ? "#ffffff" : "#064e3b";

                Time dayTime(dayTotal);
                dateButton->setText(QString("%1\n%2").arg(day).arg(QString::fromStdString(dayTime.toString())));
                dateButton->setStyleSheet(QString(
                    "QPushButton {"
                    "  background-color: rgb(%1, %2, %3);"
                    "  color: %4;"
                    "  border: 1px solid rgb(%5, %6, %7);"
                    "  border-radius: 6px;"
                    "  font-weight: bold;"
                    "  font-size: 11px;"
                    "}"
                    "QPushButton:hover {"
                    "  border: 2px solid #059669;"
                    "}"
                ).arg(r).arg(g).arg(b)
                 .arg(textColor)
                 .arg(qMax(0, r - 35)).arg(qMax(0, g - 35)).arg(qMax(0, b - 35)));
            } else {
                dateButton->setText(QString::number(day));
                dateButton->setStyleSheet(
                    "QPushButton {"
                    "  background-color: #f8fafc;"
                    "  color: #334155;"
                    "  border: 1px solid #cbd5e1;"
                    "  border-radius: 6px;"
                    "  font-weight: 500;"
                    "  font-size: 13px;"
                    "}"
                    "QPushButton:hover {"
                    "  background-color: #f1f5f9;"
                    "  border: 1px solid #94a3b8;"
                    "}"
                );
            }

            connect(dateButton, &QPushButton::clicked, this, [this, day]() {
                showFitnessDayDetails(day);
            });

            calendarLayout->addWidget(dateButton, row, col);
        }
    } else if (currentTracker == "Study Tracker") {
        studyTracker.setup_tracker();

        int maxTime = 0;
        for (int d = 1; d <= totalDays; ++d) {
            Hashmap<string, int> study = studyTracker.get_daily_study(currentMonth, d);
            int tot_idx = study.search("total");
            int dTot = (tot_idx != -1) ? study.value[tot_idx] : 0;
            if (dTot > maxTime) {
                maxTime = dTot;
            }
        }

        for (int day = 1; day <= totalDays; ++day) {
            int index = day - 1;
            int row = index / COLS;
            int col = index % COLS;

            QPushButton *dateButton = new QPushButton();
            dateButton->setMinimumSize(54, 54);

            Hashmap<string, int> study = studyTracker.get_daily_study(currentMonth, day);
            int tot_idx = study.search("total");
            int dayTotal = (tot_idx != -1) ? study.value[tot_idx] : 0;

            if (dayTotal > 0) {
                double ratio = (maxTime > 0) ? (double)dayTotal / (double)maxTime : 0.0;
                double t = 0.15 + 0.85 * ratio;

                int r = (int)(243 * (1.0 - t) + 109 * t);
                int g = (int)(232 * (1.0 - t) + 40 * t);
                int b = (int)(255 * (1.0 - t) + 217 * t);

                QString textColor = (t > 0.45) ? "#ffffff" : "#3b0764";

                Time dayTime(dayTotal);
                dateButton->setText(QString("%1\n%2").arg(day).arg(QString::fromStdString(dayTime.toString())));
                dateButton->setStyleSheet(QString(
                    "QPushButton {"
                    "  background-color: rgb(%1, %2, %3);"
                    "  color: %4;"
                    "  border: 1px solid rgb(%5, %6, %7);"
                    "  border-radius: 6px;"
                    "  font-weight: bold;"
                    "  font-size: 11px;"
                    "}"
                    "QPushButton:hover {"
                    "  border: 2px solid #7c3aed;"
                    "}"
                ).arg(r).arg(g).arg(b)
                 .arg(textColor)
                 .arg(qMax(0, r - 35)).arg(qMax(0, g - 35)).arg(qMax(0, b - 35)));
            } else {
                dateButton->setText(QString::number(day));
                dateButton->setStyleSheet(
                    "QPushButton {"
                    "  background-color: #f8fafc;"
                    "  color: #334155;"
                    "  border: 1px solid #cbd5e1;"
                    "  border-radius: 6px;"
                    "  font-weight: 500;"
                    "  font-size: 13px;"
                    "}"
                    "QPushButton:hover {"
                    "  background-color: #f1f5f9;"
                    "  border: 1px solid #94a3b8;"
                    "}"
                );
            }

            connect(dateButton, &QPushButton::clicked, this, [this, day]() {
                showStudyDayDetails(day);
            });

            calendarLayout->addWidget(dateButton, row, col);
        }
    } else if (currentTracker == "Food Tracker") {
        foodTracker.setup_tracker();

        for (int day = 1; day <= totalDays; ++day) {
            int index = day - 1;
            int row = index / COLS;
            int col = index % COLS;

            QPushButton *dateButton = new QPushButton();
            dateButton->setMinimumSize(54, 54);

            CheckList cl = foodTracker.get_daily_checklist(currentMonth, day);
            int mealsHad = cl.totalMealsHad();

            if (mealsHad > 0) {
                int r, g, b;
                QString textColor;
                if (mealsHad == 3) {
                    r = 234; g = 88; b = 12;
                    textColor = "#ffffff";
                } else if (mealsHad == 2) {
                    r = 251; g = 146; b = 60;
                    textColor = "#431407";
                } else {
                    r = 254; g = 215; b = 170;
                    textColor = "#7c2d12";
                }

                dateButton->setText(QString("%1\n%2/3").arg(day).arg(mealsHad));
                dateButton->setStyleSheet(QString(
                    "QPushButton {"
                    "  background-color: rgb(%1, %2, %3);"
                    "  color: %4;"
                    "  border: 1px solid rgb(%5, %6, %7);"
                    "  border-radius: 6px;"
                    "  font-weight: bold;"
                    "  font-size: 11px;"
                    "}"
                    "QPushButton:hover {"
                    "  border: 2px solid #ea580c;"
                    "}"
                ).arg(r).arg(g).arg(b)
                 .arg(textColor)
                 .arg(qMax(0, r - 35)).arg(qMax(0, g - 35)).arg(qMax(0, b - 35)));
            } else {
                dateButton->setText(QString::number(day));
                dateButton->setStyleSheet(
                    "QPushButton {"
                    "  background-color: #f8fafc;"
                    "  color: #334155;"
                    "  border: 1px solid #cbd5e1;"
                    "  border-radius: 6px;"
                    "  font-weight: 500;"
                    "  font-size: 13px;"
                    "}"
                    "QPushButton:hover {"
                    "  background-color: #f1f5f9;"
                    "  border: 1px solid #94a3b8;"
                    "}"
                );
            }

            connect(dateButton, &QPushButton::clicked, this, [this, day]() {
                showFoodDayDetails(day);
            });

            calendarLayout->addWidget(dateButton, row, col);
        }
    } else {
        for (int day = 1; day <= totalDays; ++day) {
            int index = day - 1;
            int row = index / COLS;
            int col = index % COLS;

            QPushButton *dateButton = new QPushButton(QString::number(day));
            dateButton->setMinimumSize(54, 54);
            dateButton->setStyleSheet(
                "QPushButton {"
                "  background-color: #f8fafc;"
                "  color: #334155;"
                "  border: 1px solid #cbd5e1;"
                "  border-radius: 6px;"
                "  font-weight: 500;"
                "  font-size: 13px;"
                "}"
                "QPushButton:hover {"
                "  background-color: #f1f5f9;"
                "  border: 1px solid #94a3b8;"
                "}"
            );

            connect(dateButton, &QPushButton::clicked, this, [this, day]() {
                QMessageBox::information(
                    this,
                    "Selected Date",
                    QString("%1 %2").arg(day).arg(MONTH_NAMES[currentMonth])
                );
            });

            calendarLayout->addWidget(dateButton, row, col);
        }
    }
}

void MainWindow::showDayDetails(int day) {
    if (activeTracker) {
        activeTracker->manage_history("Day details inspected for " + to_string(day) + "/" + to_string(currentMonth));
    }
    expenseTracker.setup_tracker();
    Hashmap<string, int> spend = expenseTracker.get_daily_spend(currentMonth, day);

    auto getVal = [&](const string &cat) {
        int idx = spend.search(cat);
        return (idx != -1) ? spend.value[idx] : 0;
    };

    int food = getVal("food");
    int stationary = getVal("stationary");
    int travel = getVal("travel");
    int lifestyle = getVal("lifestyle");
    int misc = getVal("miscellaneous");
    int total = getVal("total");

    QString dateStr = QString("%1 %2").arg(day).arg(MONTH_NAMES[currentMonth]);
    QString msg = QString("<b>Date: %1</b><br><br>"
                          "<table width='100%' style='font-size: 14px;'>"
                          "<tr><td><b>Food:</b></td><td>\xe2\x82\xb9%2</td></tr>"
                          "<tr><td><b>Stationary:</b></td><td>\xe2\x82\xb9%3</td></tr>"
                          "<tr><td><b>Travel:</b></td><td>\xe2\x82\xb9%4</td></tr>"
                          "<tr><td><b>Lifestyle:</b></td><td>\xe2\x82\xb9%5</td></tr>"
                          "<tr><td><b>Miscellaneous:</b></td><td>\xe2\x82\xb9%6</td></tr>"
                          "<tr style='border-top: 1px solid gray;'><td><b>Total:</b></td><td><b>\xe2\x82\xb9%7</b></td></tr>"
                          "</table>")
                      .arg(dateStr)
                      .arg(food)
                      .arg(stationary)
                      .arg(travel)
                      .arg(lifestyle)
                      .arg(misc)
                      .arg(total);

    QMessageBox box(this);
    box.setWindowTitle("Daily Expense Details");
    box.setText(msg);
    QPushButton *addBtn = box.addButton("Add Expense", QMessageBox::ActionRole);
    box.addButton(QMessageBox::Close);
    box.exec();

    if (box.clickedButton() == addBtn) {
        showAddExpenseDialog(day);
    }
}

void MainWindow::showAddExpenseDialog(int preselectedDay) {
    QDialog dialog(this);
    dialog.setWindowTitle("Add Expense");
    dialog.resize(320, 200);

    QFormLayout form(&dialog);

    int totalDays = getDaysInMonth(currentMonth);
    QSpinBox *daySpin = new QSpinBox(&dialog);
    daySpin->setRange(1, totalDays);
    if (preselectedDay >= 1 && preselectedDay <= totalDays) {
        daySpin->setValue(preselectedDay);
    } else {
        daySpin->setValue(1);
    }
    form.addRow("Day:", daySpin);

    QComboBox *catCombo = new QComboBox(&dialog);
    catCombo->addItem("Food", "food");
    catCombo->addItem("Stationary", "stationary");
    catCombo->addItem("Travel", "travel");
    catCombo->addItem("Lifestyle", "lifestyle");
    catCombo->addItem("Miscellaneous", "miscellaneous");
    form.addRow("Category:", catCombo);

    QDoubleSpinBox *amountSpin = new QDoubleSpinBox(&dialog);
    amountSpin->setRange(1.0, 1000000.0);
    amountSpin->setDecimals(2);
    amountSpin->setValue(10.00);
    form.addRow("Amount (\xe2\x82\xb9):", amountSpin);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);

    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        int day = daySpin->value();
        string category = catCombo->currentData().toString().toStdString();
        float amount = (float)amountSpin->value();

        expenseTracker.add_expense(currentMonth, day, amount, category);
        updateCalendar();

        QMessageBox::information(this, "Success",
            QString("Added \xe2\x82\xb9%1 to %2 for %3 %4.")
                .arg(amount)
                .arg(catCombo->currentText())
                .arg(MONTH_NAMES[currentMonth])
                .arg(day));
    }
}

void MainWindow::showFitnessDayDetails(int day) {
    if (activeTracker) {
        activeTracker->manage_history("Day details inspected for " + to_string(day) + "/" + to_string(currentMonth));
    }
    fitnessTracker.setup_tracker();
    Hashmap<string, int> act = fitnessTracker.get_daily_activity(currentMonth, day);

    auto getVal = [&](const string &cat) {
        int idx = act.search(cat);
        return (idx != -1) ? act.value[idx] : 0;
    };

    Time walking(getVal("walking"));
    Time running(getVal("running"));
    Time sports(getVal("sports"));
    Time gym(getVal("gym"));
    Time yoga(getVal("yoga"));
    Time total = fitnessTracker.get_daily_total_time(currentMonth, day);

    QString dateStr = QString("%1 %2").arg(day).arg(MONTH_NAMES[currentMonth]);
    QString msg = QString("<b>Date: %1</b><br><br>"
                          "<table width='100%' style='font-size: 14px;'>"
                          "<tr><td><b>Walking:</b></td><td>%2</td></tr>"
                          "<tr><td><b>Running:</b></td><td>%3</td></tr>"
                          "<tr><td><b>Sports:</b></td><td>%4</td></tr>"
                          "<tr><td><b>Gym:</b></td><td>%5</td></tr>"
                          "<tr><td><b>Yoga:</b></td><td>%6</td></tr>"
                          "<tr style='border-top: 1px solid gray;'><td><b>Total Active Time:</b></td><td><b>%7 (%8 mins)</b></td></tr>"
                          "</table>")
                      .arg(dateStr)
                      .arg(QString::fromStdString(walking.toString()))
                      .arg(QString::fromStdString(running.toString()))
                      .arg(QString::fromStdString(sports.toString()))
                      .arg(QString::fromStdString(gym.toString()))
                      .arg(QString::fromStdString(yoga.toString()))
                      .arg(QString::fromStdString(total.toString()))
                      .arg(total.getTotalMinutes());

    QMessageBox box(this);
    box.setWindowTitle("Daily Fitness Details");
    box.setText(msg);
    QPushButton *addBtn = box.addButton("Add Activity", QMessageBox::ActionRole);
    box.addButton(QMessageBox::Close);
    box.exec();

    if (box.clickedButton() == addBtn) {
        showAddFitnessDialog(day);
    }
}

void MainWindow::showAddFitnessDialog(int preselectedDay) {
    QDialog dialog(this);
    dialog.setWindowTitle("Add Activity");
    dialog.resize(340, 230);

    QFormLayout form(&dialog);

    int totalDays = getDaysInMonth(currentMonth);
    QSpinBox *daySpin = new QSpinBox(&dialog);
    daySpin->setRange(1, totalDays);
    if (preselectedDay >= 1 && preselectedDay <= totalDays) {
        daySpin->setValue(preselectedDay);
    } else {
        daySpin->setValue(1);
    }
    form.addRow("Day:", daySpin);

    QComboBox *catCombo = new QComboBox(&dialog);
    catCombo->addItem("Walking", "walking");
    catCombo->addItem("Running", "running");
    catCombo->addItem("Sports", "sports");
    catCombo->addItem("Gym", "gym");
    catCombo->addItem("Yoga", "yoga");
    form.addRow("Activity:", catCombo);

    QHBoxLayout *timeLayout = new QHBoxLayout();
    QSpinBox *hourSpin = new QSpinBox(&dialog);
    hourSpin->setRange(0, 23);
    hourSpin->setSuffix(" hrs");
    hourSpin->setValue(0);

    QSpinBox *minSpin = new QSpinBox(&dialog);
    minSpin->setRange(0, 59);
    minSpin->setSuffix(" mins");
    minSpin->setValue(30);

    timeLayout->addWidget(hourSpin);
    timeLayout->addWidget(minSpin);
    form.addRow("Duration:", timeLayout);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);

    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        int day = daySpin->value();
        string category = catCombo->currentData().toString().toStdString();
        Time fitnessTime(hourSpin->value(), minSpin->value());

        if (fitnessTime.getTotalMinutes() > 0) {
            fitnessTracker.add_activity(currentMonth, day, fitnessTime, category);
            updateCalendar();

            QMessageBox::information(this, "Success",
                QString("Added %1 of %2 for %3 %4.")
                    .arg(QString::fromStdString(fitnessTime.toString()))
                    .arg(catCombo->currentText())
                    .arg(MONTH_NAMES[currentMonth])
                    .arg(day));
        }
    }
}

void MainWindow::showStudyDayDetails(int day) {
    if (activeTracker) {
        activeTracker->manage_history("Day details inspected for " + to_string(day) + "/" + to_string(currentMonth));
    }
    studyTracker.setup_tracker();
    Hashmap<string, int> study = studyTracker.get_daily_study(currentMonth, day);

    auto getVal = [&](const string &cat) {
        int idx = study.search(cat);
        return (idx != -1) ? study.value[idx] : 0;
    };

    Time cso(getVal("cso"));
    Time ioop(getVal("ioop"));
    Time itw(getVal("itw"));
    Time mi(getVal("mi"));
    Time nmpt(getVal("nmpt"));
    Time dsa(getVal("dsa"));
    Time total = studyTracker.get_daily_total_time(currentMonth, day);

    QString dateStr = QString("%1 %2").arg(day).arg(MONTH_NAMES[currentMonth]);
    QString msg = QString("<b>Date: %1</b><br><br>"
                          "<table width='100%' style='font-size: 14px;'>"
                          "<tr><td><b>CSO:</b></td><td>%2</td></tr>"
                          "<tr><td><b>IOOP:</b></td><td>%3</td></tr>"
                          "<tr><td><b>ITW:</b></td><td>%4</td></tr>"
                          "<tr><td><b>MI:</b></td><td>%5</td></tr>"
                          "<tr><td><b>NMPT:</b></td><td>%6</td></tr>"
                          "<tr><td><b>DSA:</b></td><td>%7</td></tr>"
                          "<tr style='border-top: 1px solid gray;'><td><b>Total Study Time:</b></td><td><b>%8 (%9 mins)</b></td></tr>"
                          "</table>")
                      .arg(dateStr)
                      .arg(QString::fromStdString(cso.toString()))
                      .arg(QString::fromStdString(ioop.toString()))
                      .arg(QString::fromStdString(itw.toString()))
                      .arg(QString::fromStdString(mi.toString()))
                      .arg(QString::fromStdString(nmpt.toString()))
                      .arg(QString::fromStdString(dsa.toString()))
                      .arg(QString::fromStdString(total.toString()))
                      .arg(total.getTotalMinutes());

    QMessageBox box(this);
    box.setWindowTitle("Daily Study Details");
    box.setText(msg);
    QPushButton *addBtn = box.addButton("Add Study Time", QMessageBox::ActionRole);
    box.addButton(QMessageBox::Close);
    box.exec();

    if (box.clickedButton() == addBtn) {
        showAddStudyDialog(day);
    }
}

void MainWindow::showAddStudyDialog(int preselectedDay) {
    QDialog dialog(this);
    dialog.setWindowTitle("Add Study Time");
    dialog.resize(340, 230);

    QFormLayout form(&dialog);

    int totalDays = getDaysInMonth(currentMonth);
    QSpinBox *daySpin = new QSpinBox(&dialog);
    daySpin->setRange(1, totalDays);
    if (preselectedDay >= 1 && preselectedDay <= totalDays) {
        daySpin->setValue(preselectedDay);
    } else {
        daySpin->setValue(1);
    }
    form.addRow("Day:", daySpin);

    QComboBox *subCombo = new QComboBox(&dialog);
    subCombo->addItem("CSO", "cso");
    subCombo->addItem("IOOP", "ioop");
    subCombo->addItem("ITW", "itw");
    subCombo->addItem("MI", "mi");
    subCombo->addItem("NMPT", "nmpt");
    subCombo->addItem("DSA", "dsa");
    form.addRow("Subject:", subCombo);

    QHBoxLayout *timeLayout = new QHBoxLayout();
    QSpinBox *hourSpin = new QSpinBox(&dialog);
    hourSpin->setRange(0, 23);
    hourSpin->setSuffix(" hrs");
    hourSpin->setValue(1);

    QSpinBox *minSpin = new QSpinBox(&dialog);
    minSpin->setRange(0, 59);
    minSpin->setSuffix(" mins");
    minSpin->setValue(0);

    timeLayout->addWidget(hourSpin);
    timeLayout->addWidget(minSpin);
    form.addRow("Duration:", timeLayout);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);

    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        int day = daySpin->value();
        string subject = subCombo->currentData().toString().toStdString();
        Time studyTime(hourSpin->value(), minSpin->value());

        if (studyTime.getTotalMinutes() > 0) {
            studyTracker.add_study_time(currentMonth, day, studyTime, subject);
            updateCalendar();

            QMessageBox::information(this, "Success",
                QString("Added %1 of %2 for %3 %4.")
                    .arg(QString::fromStdString(studyTime.toString()))
                    .arg(subCombo->currentText())
                    .arg(MONTH_NAMES[currentMonth])
                    .arg(day));
        }
    }
}

void MainWindow::showFoodDayDetails(int day) {
    if (activeTracker) {
        activeTracker->manage_history("Day details inspected for " + to_string(day) + "/" + to_string(currentMonth));
    }
    foodTracker.setup_tracker();
    CheckList cl = foodTracker.get_daily_checklist(currentMonth, day);

    QString dateStr = QString("%1 %2").arg(day).arg(MONTH_NAMES[currentMonth]);

    QString bfStatus = cl.hasBreakfast()
        ? QString("<font color='#16a34a'><b>Had</b></font> - %1")
              .arg(cl.getBreakfastName().empty() ? "Logged" : QString::fromStdString(cl.getBreakfastName()))
        : "<font color='#dc2626'><b>Skipped</b></font>";

    QString lunchStatus = cl.hasLunch()
        ? QString("<font color='#16a34a'><b>Had</b></font> - %1")
              .arg(cl.getLunchName().empty() ? "Logged" : QString::fromStdString(cl.getLunchName()))
        : "<font color='#dc2626'><b>Skipped</b></font>";

    QString dinnerStatus = cl.hasDinner()
        ? QString("<font color='#16a34a'><b>Had</b></font> - %1")
              .arg(cl.getDinnerName().empty() ? "Logged" : QString::fromStdString(cl.getDinnerName()))
        : "<font color='#dc2626'><b>Skipped</b></font>";

    int totalMeals = cl.totalMealsHad();

    QString msg = QString("<b>Date: %1</b><br><br>"
                          "<table width='100%' style='font-size: 14px;'>"
                          "<tr><td><b>Breakfast:</b></td><td>%2</td></tr>"
                          "<tr><td><b>Lunch:</b></td><td>%3</td></tr>"
                          "<tr><td><b>Dinner:</b></td><td>%4</td></tr>"
                          "<tr style='border-top: 1px solid gray;'><td><b>Meals Logged:</b></td><td><b>%5 / 3</b></td></tr>"
                          "</table>")
                      .arg(dateStr)
                      .arg(bfStatus)
                      .arg(lunchStatus)
                      .arg(dinnerStatus)
                      .arg(totalMeals);

    QMessageBox box(this);
    box.setWindowTitle("Daily Meal Details");
    box.setText(msg);
    QPushButton *logBtn = box.addButton("Log / Edit Meals", QMessageBox::ActionRole);
    box.addButton(QMessageBox::Close);
    box.exec();

    if (box.clickedButton() == logBtn) {
        showLogMealDialog(day);
    }
}

void MainWindow::showLogMealDialog(int preselectedDay) {
    QDialog dialog(this);
    dialog.setWindowTitle("Log Meals");
    dialog.resize(400, 260);

    QVBoxLayout rootLayout(&dialog);
    QFormLayout form;

    int totalDays = getDaysInMonth(currentMonth);
    QSpinBox daySpin(&dialog);
    daySpin.setRange(1, totalDays);
    if (preselectedDay >= 1 && preselectedDay <= totalDays) {
        daySpin.setValue(preselectedDay);
    } else {
        daySpin.setValue(1);
    }
    form.addRow("Day:", &daySpin);

    QCheckBox bfCheck("Had Breakfast", &dialog);
    QLineEdit bfEdit(&dialog);
    bfEdit.setPlaceholderText("Food eaten (e.g. Oatmeal & Eggs)");
    QHBoxLayout bfLayout;
    bfLayout.addWidget(&bfCheck);
    bfLayout.addWidget(&bfEdit);
    form.addRow("Breakfast:", &bfLayout);

    QCheckBox lunchCheck("Had Lunch", &dialog);
    QLineEdit lunchEdit(&dialog);
    lunchEdit.setPlaceholderText("Food eaten (e.g. Rice & Chicken)");
    QHBoxLayout lunchLayout;
    lunchLayout.addWidget(&lunchCheck);
    lunchLayout.addWidget(&lunchEdit);
    form.addRow("Lunch:", &lunchLayout);

    QCheckBox dinnerCheck("Had Dinner", &dialog);
    QLineEdit dinnerEdit(&dialog);
    dinnerEdit.setPlaceholderText("Food eaten (e.g. Salad & Pasta)");
    QHBoxLayout dinnerLayout;
    dinnerLayout.addWidget(&dinnerCheck);
    dinnerLayout.addWidget(&dinnerEdit);
    form.addRow("Dinner:", &dinnerLayout);

    rootLayout.addLayout(&form);

    auto loadDayData = [&](int d) {
        CheckList cl = foodTracker.get_daily_checklist(currentMonth, d);
        bfCheck.setChecked(cl.hasBreakfast());
        bfEdit.setText(QString::fromStdString(cl.getBreakfastName()));
        lunchCheck.setChecked(cl.hasLunch());
        lunchEdit.setText(QString::fromStdString(cl.getLunchName()));
        dinnerCheck.setChecked(cl.hasDinner());
        dinnerEdit.setText(QString::fromStdString(cl.getDinnerName()));
    };

    loadDayData(daySpin.value());

    QObject::connect(&daySpin, QOverload<int>::of(&QSpinBox::valueChanged), [&](int d) {
        loadDayData(d);
    });

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    rootLayout.addWidget(&buttonBox);

    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        int day = daySpin.value();
        CheckList cl(bfCheck.isChecked(), lunchCheck.isChecked(), dinnerCheck.isChecked(),
                     bfEdit.text().toStdString(), lunchEdit.text().toStdString(), dinnerEdit.text().toStdString());
        foodTracker.set_daily_checklist(currentMonth, day, cl);
        updateCalendar();

        QMessageBox::information(this, "Success",
            QString("Updated meals for %1 %2.")
                .arg(MONTH_NAMES[currentMonth])
                .arg(day));
    }
}

void MainWindow::onOption1Clicked() {
    if (activeTracker) {
        activeTracker->manage_history("Option 1 clicked: " + option1->text().toStdString());
    }
    if (currentTracker == "Expense Tracker") {
        showAddExpenseDialog(-1);
    } else if (currentTracker == "Fitness Tracker") {
        showAddFitnessDialog(-1);
    } else if (currentTracker == "Study Tracker") {
        showAddStudyDialog(-1);
    } else if (currentTracker == "Food Tracker") {
        showLogMealDialog(-1);
    } else {
        QMessageBox::information(this, "Option 1", "Coming soon.");
    }
}

void MainWindow::onOption2Clicked() {
    if (activeTracker) {
        activeTracker->manage_history("Option 2 clicked: " + option2->text().toStdString() + " for " + MONTH_NAMES[currentMonth].toStdString());
    }
    if (currentTracker == "Expense Tracker") {
        expenseTracker.setup_tracker();
        float monthTotal = expenseTracker.get_month_total(currentMonth);
        QString monthName = MONTH_NAMES[currentMonth];
        QMessageBox::information(this, "Month Total",
            QString("<h3>Total Expenses for %1</h3><br><font size='+2' color='#2563eb'><b>\xe2\x82\xb9%2</b></font>")
                .arg(monthName)
                .arg(monthTotal));
    } else if (currentTracker == "Fitness Tracker") {
        fitnessTracker.setup_tracker();
        Time monthTotal = fitnessTracker.get_month_total_time(currentMonth);
        QString monthName = MONTH_NAMES[currentMonth];
        QMessageBox::information(this, "Month Total",
            QString("<h3>Total Active Time for %1</h3><br><font size='+2' color='#059669'><b>%2 (%3 mins)</b></font>")
                .arg(monthName)
                .arg(QString::fromStdString(monthTotal.toString()))
                .arg(monthTotal.getTotalMinutes()));
    } else if (currentTracker == "Study Tracker") {
        studyTracker.setup_tracker();
        Time monthTotal = studyTracker.get_month_total_time(currentMonth);
        QString monthName = MONTH_NAMES[currentMonth];
        QMessageBox::information(this, "Month Total",
            QString("<h3>Total Study Time for %1</h3><br><font size='+2' color='#7c3aed'><b>%2 (%3 mins)</b></font>")
                .arg(monthName)
                .arg(QString::fromStdString(monthTotal.toString()))
                .arg(monthTotal.getTotalMinutes()));
    } else if (currentTracker == "Food Tracker") {
        foodTracker.setup_tracker();
        int totalMeals = foodTracker.get_month_total_meals(currentMonth);
        int totalDays = getDaysInMonth(currentMonth);
        int maxMeals = totalDays * 3;
        Hashmap<string, int> counts = foodTracker.get_month_meal_counts(currentMonth);
        int full_idx = counts.search("full_days");
        int fullDays = (full_idx != -1) ? counts.value[full_idx] : 0;

        QString monthName = MONTH_NAMES[currentMonth];
        QMessageBox::information(this, "Month Total",
            QString("<h3>Meal Summary for %1</h3><br>"
                    "<font size='+2' color='#ea580c'><b>%2 / %3 Meals Logged</b></font><br><br>"
                    "Days with all 3 meals: <b>%4 / %5 days</b>")
                .arg(monthName)
                .arg(totalMeals)
                .arg(maxMeals)
                .arg(fullDays)
                .arg(totalDays));
    } else {
        QMessageBox::information(this, "Option 2", "Coming soon.");
    }
}

void MainWindow::onOption3Clicked() {
    if (activeTracker) {
        activeTracker->manage_history("Option 3 clicked: " + option3->text().toStdString() + " for " + MONTH_NAMES[currentMonth].toStdString());
    }
    if (currentTracker == "Expense Tracker") {
        expenseTracker.setup_tracker();
        Hashmap<string, int> catTotals = expenseTracker.getCategoryTotal(currentMonth);
        auto getVal = [&](const string &cat) {
            int idx = catTotals.search(cat);
            return (idx != -1) ? catTotals.value[idx] : 0;
        };

        int food = getVal("food");
        int stationary = getVal("stationary");
        int travel = getVal("travel");
        int lifestyle = getVal("lifestyle");
        int misc = getVal("miscellaneous");
        int total = getVal("total");

        QString monthName = MONTH_NAMES[currentMonth];
        QString msg = QString("<h3>Category Breakdown for %1</h3>"
                              "<table width='100%' style='font-size: 14px; margin-top: 10px;'>"
                              "<tr><td><b>Food:</b></td><td>\xe2\x82\xb9%2</td></tr>"
                              "<tr><td><b>Stationary:</b></td><td>\xe2\x82\xb9%3</td></tr>"
                              "<tr><td><b>Travel:</b></td><td>\xe2\x82\xb9%4</td></tr>"
                              "<tr><td><b>Lifestyle:</b></td><td>\xe2\x82\xb9%5</td></tr>"
                              "<tr><td><b>Miscellaneous:</b></td><td>\xe2\x82\xb9%6</td></tr>"
                              "<tr style='border-top: 1px solid gray;'><td><b>Total:</b></td><td><b>\xe2\x82\xb9%7</b></td></tr>"
                              "</table>")
                          .arg(monthName)
                          .arg(food)
                          .arg(stationary)
                          .arg(travel)
                          .arg(lifestyle)
                          .arg(misc)
                          .arg(total);

        QMessageBox::information(this, "Category Totals", msg);
    } else if (currentTracker == "Fitness Tracker") {
        fitnessTracker.setup_tracker();
        Hashmap<string, int> catTotals = fitnessTracker.getCategoryTotal(currentMonth);
        auto getVal = [&](const string &cat) {
            int idx = catTotals.search(cat);
            return (idx != -1) ? catTotals.value[idx] : 0;
        };

        Time walking(getVal("walking"));
        Time running(getVal("running"));
        Time sports(getVal("sports"));
        Time gym(getVal("gym"));
        Time yoga(getVal("yoga"));
        Time total = fitnessTracker.get_month_total_time(currentMonth);

        QString monthName = MONTH_NAMES[currentMonth];
        QString msg = QString("<h3>Activity Breakdown for %1</h3>"
                              "<table width='100%' style='font-size: 14px; margin-top: 10px;'>"
                              "<tr><td><b>Walking:</b></td><td>%2</td></tr>"
                              "<tr><td><b>Running:</b></td><td>%3</td></tr>"
                              "<tr><td><b>Sports:</b></td><td>%4</td></tr>"
                              "<tr><td><b>Gym:</b></td><td>%5</td></tr>"
                              "<tr><td><b>Yoga:</b></td><td>%6</td></tr>"
                              "<tr style='border-top: 1px solid gray;'><td><b>Total Active Time:</b></td><td><b>%7 (%8 mins)</b></td></tr>"
                              "</table>")
                          .arg(monthName)
                          .arg(QString::fromStdString(walking.toString()))
                          .arg(QString::fromStdString(running.toString()))
                          .arg(QString::fromStdString(sports.toString()))
                          .arg(QString::fromStdString(gym.toString()))
                          .arg(QString::fromStdString(yoga.toString()))
                          .arg(QString::fromStdString(total.toString()))
                          .arg(total.getTotalMinutes());

        QMessageBox::information(this, "Activity Breakdown", msg);
    } else if (currentTracker == "Study Tracker") {
        studyTracker.setup_tracker();
        Hashmap<string, int> subTotals = studyTracker.getSubjectTotal(currentMonth);
        auto getVal = [&](const string &sub) {
            int idx = subTotals.search(sub);
            return (idx != -1) ? subTotals.value[idx] : 0;
        };

        Time cso(getVal("cso"));
        Time ioop(getVal("ioop"));
        Time itw(getVal("itw"));
        Time mi(getVal("mi"));
        Time nmpt(getVal("nmpt"));
        Time dsa(getVal("dsa"));
        Time total = studyTracker.get_month_total_time(currentMonth);

        QString monthName = MONTH_NAMES[currentMonth];
        QString msg = QString("<h3>Subject Breakdown for %1</h3>"
                              "<table width='100%' style='font-size: 14px; margin-top: 10px;'>"
                              "<tr><td><b>CSO:</b></td><td>%2</td></tr>"
                              "<tr><td><b>IOOP:</b></td><td>%3</td></tr>"
                              "<tr><td><b>ITW:</b></td><td>%4</td></tr>"
                              "<tr><td><b>MI:</b></td><td>%5</td></tr>"
                              "<tr><td><b>NMPT:</b></td><td>%6</td></tr>"
                              "<tr><td><b>DSA:</b></td><td>%7</td></tr>"
                              "<tr style='border-top: 1px solid gray;'><td><b>Total Study Time:</b></td><td><b>%8 (%9 mins)</b></td></tr>"
                              "</table>")
                          .arg(monthName)
                          .arg(QString::fromStdString(cso.toString()))
                          .arg(QString::fromStdString(ioop.toString()))
                          .arg(QString::fromStdString(itw.toString()))
                          .arg(QString::fromStdString(mi.toString()))
                          .arg(QString::fromStdString(nmpt.toString()))
                          .arg(QString::fromStdString(dsa.toString()))
                          .arg(QString::fromStdString(total.toString()))
                          .arg(total.getTotalMinutes());

        QMessageBox::information(this, "Subject Breakdown", msg);
    } else if (currentTracker == "Food Tracker") {
        foodTracker.setup_tracker();
        Hashmap<string, int> counts = foodTracker.get_month_meal_counts(currentMonth);
        auto getVal = [&](const string &m) {
            int idx = counts.search(m);
            return (idx != -1) ? counts.value[idx] : 0;
        };

        int bf = getVal("breakfast");
        int lunch = getVal("lunch");
        int dinner = getVal("dinner");
        int total = getVal("total");
        int fullDays = getVal("full_days");
        int totalDays = getDaysInMonth(currentMonth);

        QString monthName = MONTH_NAMES[currentMonth];
        QString msg = QString("<h3>Meal Breakdown for %1</h3>"
                              "<table width='100%' style='font-size: 14px; margin-top: 10px;'>"
                              "<tr><td><b>Breakfasts Had:</b></td><td>%2 / %3 days</td></tr>"
                              "<tr><td><b>Lunches Had:</b></td><td>%4 / %3 days</td></tr>"
                              "<tr><td><b>Dinners Had:</b></td><td>%5 / %3 days</td></tr>"
                              "<tr><td><b>Full 3-Meal Days:</b></td><td>%6 / %3 days</td></tr>"
                              "<tr style='border-top: 1px solid gray;'><td><b>Total Meals:</b></td><td><b>%7 / %8</b></td></tr>"
                              "</table>")
                          .arg(monthName)
                          .arg(bf)
                          .arg(totalDays)
                          .arg(lunch)
                          .arg(dinner)
                          .arg(fullDays)
                          .arg(total)
                          .arg(totalDays * 3);

        QMessageBox::information(this, "Meal Breakdown", msg);
    } else {
        QMessageBox::information(this, "Option 3", "Coming soon.");
    }
}
