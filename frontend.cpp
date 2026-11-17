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

    // Track active month (12-month tracker, defaults to January = 1)
    currentMonth = 1;
    currentTracker = "";

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
    calendarLayout->setSpacing(6);

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

    // Action Slots
    connect(option1, &QPushButton::clicked, this, &MainWindow::onOption1Clicked);
    connect(option2, &QPushButton::clicked, this, &MainWindow::onOption2Clicked);
    connect(option3, &QPushButton::clicked, this, &MainWindow::onOption3Clicked);
}

MainWindow::~MainWindow() {}

// Helper routing function
void MainWindow::openTracker(const QString &trackerName) {
    trackerTitle->setText(trackerName);
    pages->setCurrentWidget(trackerPage);
}

// Slots Implementations
void MainWindow::openExpenseTracker() {
    currentTracker = "Expense Tracker";
    currentMonth = 1; // Default open as January
    expenseTracker.setup_tracker();
    openTracker("Expense Tracker");

    option1->setText("Add Expense");
    option2->setText("Month Total");
    option3->setText("Category Totals");

    updateCalendar();
}

void MainWindow::openFitnessTracker() {
    currentTracker = "Fitness Tracker";
    currentMonth = 1; // Default open as January
    fitnessTracker.setup_tracker();
    openTracker("Fitness Tracker");
    option1->setText("Add Activity");
    option2->setText("Month Total");
    option3->setText("Activity Breakdown");
    updateCalendar();
}

void MainWindow::openFoodTracker() {
    currentTracker = "Food Tracker";
    currentMonth = 1; // Default open as January
    foodTracker.setup_tracker();
    openTracker("Food Tracker");
    option1->setText("Log Meals");
    option2->setText("Month Total");
    option3->setText("Meal Breakdown");
    updateCalendar();
}

void MainWindow::openStudyTracker() {
    currentTracker = "Study Tracker";
    currentMonth = 1; // Default open as January
    studyTracker.setup_tracker();
    openTracker("Study Tracker");
    option1->setText("Add Study Time");
    option2->setText("Month Total");
    option3->setText("Subject Breakdown");
    updateCalendar();
}

void MainWindow::createNewTracker() {
    QMessageBox::information(this, "New Tracker", "Coming soon.");
}

void MainWindow::backToMainMenu() {
    currentTracker = "";
    pages->setCurrentWidget(mainPage);
}

void MainWindow::goToPreviousMonth() {
    if (--currentMonth < 1) {
        currentMonth = 12;
    }
    updateCalendar();
}

void MainWindow::goToNextMonth() {
    if (++currentMonth > 12) {
        currentMonth = 1;
    }
    updateCalendar();
}

// Calendar Generator Engine (12-month calendar without weekdays, normal square grid with heatmap)
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
        // Ensure bidirectional sync: reload any changes from file
        expenseTracker.setup_tracker();

        // 1. Find max expense across all days in this month to scale the heatmap
        int maxSpend = 0;
        for (int d = 1; d <= totalDays; ++d) {
            Hashmap<string, int> spend = expenseTracker.get_daily_spend(currentMonth, d);
            int tot_idx = spend.search("total");
            int dTot = (tot_idx != -1) ? spend.value[tot_idx] : 0;
            if (dTot > maxSpend) {
                maxSpend = dTot;
            }
        }

        // 2. Generate days in a normal square grid starting at (0,0) without reserved spaces
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
                // Ratio from lowest relative spend to highest
                double ratio = (maxSpend > 0) ? (double)dayTotal / (double)maxSpend : 0.0;
                double t = 0.15 + 0.85 * ratio;

                // Color gradient: higher expense is darker, lower expense is lighter
                int r = (int)(224 * (1.0 - t) + 30 * t);
                int g = (int)(242 * (1.0 - t) + 58 * t);
                int b = (int)(254 * (1.0 - t) + 138 * t);

                // Dynamically adjust text color based on lightness of the day box
                QString textColor = (t > 0.45) ? "#ffffff" : "#0f172a";

                dateButton->setText(QString("%1\n$%2").arg(day).arg(dayTotal));
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
        // Ensure bidirectional sync: reload any changes from file
        fitnessTracker.setup_tracker();

        // 1. Find max active time across all days in this month to scale the heatmap
        int maxTime = 0;
        for (int d = 1; d <= totalDays; ++d) {
            Hashmap<string, int> activity = fitnessTracker.get_daily_activity(currentMonth, d);
            int tot_idx = activity.search("total");
            int dTot = (tot_idx != -1) ? activity.value[tot_idx] : 0;
            if (dTot > maxTime) {
                maxTime = dTot;
            }
        }

        // 2. Generate days in a square grid with emerald/green fitness heatmap
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

                // Color gradient: emerald green for fitness
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
        // Ensure bidirectional sync: reload any changes from file
        studyTracker.setup_tracker();

        // 1. Find max study time across all days in this month to scale the heatmap
        int maxTime = 0;
        for (int d = 1; d <= totalDays; ++d) {
            Hashmap<string, int> study = studyTracker.get_daily_study(currentMonth, d);
            int tot_idx = study.search("total");
            int dTot = (tot_idx != -1) ? study.value[tot_idx] : 0;
            if (dTot > maxTime) {
                maxTime = dTot;
            }
        }

        // 2. Generate days in a square grid with purple/indigo study heatmap
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

                // Color gradient: rich indigo/purple for study
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
        // Ensure bidirectional sync: reload any changes from file
        foodTracker.setup_tracker();

        // Generate days in a square grid with warm amber/orange food heatmap
        for (int day = 1; day <= totalDays; ++day) {
            int index = day - 1;
            int row = index / COLS;
            int col = index % COLS;

            QPushButton *dateButton = new QPushButton();
            dateButton->setMinimumSize(54, 54);

            CheckList cl = foodTracker.get_daily_checklist(currentMonth, day);
            int mealsHad = cl.totalMealsHad();

            if (mealsHad > 0) {
                // Warm amber/orange gradient based on meals had (1, 2, or 3)
                int r, g, b;
                QString textColor;
                if (mealsHad == 3) {
                    // All 3 meals had - rich vibrant orange
                    r = 234; g = 88; b = 12;
                    textColor = "#ffffff";
                } else if (mealsHad == 2) {
                    // 2 meals - warm medium amber
                    r = 251; g = 146; b = 60;
                    textColor = "#431407";
                } else {
                    // 1 meal - soft light amber
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
        // Normal square grid for other trackers
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
    expenseTracker.setup_tracker(); // Ensure fresh sync
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
                          "<tr><td><b>Food:</b></td><td>$%2</td></tr>"
                          "<tr><td><b>Stationary:</b></td><td>$%3</td></tr>"
                          "<tr><td><b>Travel:</b></td><td>$%4</td></tr>"
                          "<tr><td><b>Lifestyle:</b></td><td>$%5</td></tr>"
                          "<tr><td><b>Miscellaneous:</b></td><td>$%6</td></tr>"
                          "<tr style='border-top: 1px solid gray;'><td><b>Total:</b></td><td><b>$%7</b></td></tr>"
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
    form.addRow("Amount ($):", amountSpin);

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
            QString("Added $%1 to %2 for %3 %4.")
                .arg(amount)
                .arg(catCombo->currentText())
                .arg(MONTH_NAMES[currentMonth])
                .arg(day));
    }
}

void MainWindow::showFitnessDayDetails(int day) {
    fitnessTracker.setup_tracker(); // Ensure fresh sync
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
    studyTracker.setup_tracker(); // Ensure fresh sync
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

void MainWindow::onOption1Clicked() {
    if (currentTracker == "Expense Tracker") {
        showAddExpenseDialog(-1);
    } else if (currentTracker == "Fitness Tracker") {
        showAddFitnessDialog(-1);
    } else if (currentTracker == "Study Tracker") {
        showAddStudyDialog(-1);
    } else {
        QMessageBox::information(this, "Option 1", "Coming soon.");
    }
}

void MainWindow::onOption2Clicked() {
    if (currentTracker == "Expense Tracker") {
        expenseTracker.setup_tracker(); // Ensure fresh sync
        float monthTotal = expenseTracker.get_month_total(currentMonth);
        QString monthName = MONTH_NAMES[currentMonth];
        QMessageBox::information(this, "Month Total",
            QString("<h3>Total Expenses for %1</h3><br><font size='+2' color='#2563eb'><b>$%2</b></font>")
                .arg(monthName)
                .arg(monthTotal));
    } else if (currentTracker == "Fitness Tracker") {
        fitnessTracker.setup_tracker(); // Ensure fresh sync
        Time monthTotal = fitnessTracker.get_month_total_time(currentMonth);
        QString monthName = MONTH_NAMES[currentMonth];
        QMessageBox::information(this, "Month Total",
            QString("<h3>Total Active Time for %1</h3><br><font size='+2' color='#059669'><b>%2 (%3 mins)</b></font>")
                .arg(monthName)
                .arg(QString::fromStdString(monthTotal.toString()))
                .arg(monthTotal.getTotalMinutes()));
    } else if (currentTracker == "Study Tracker") {
        studyTracker.setup_tracker(); // Ensure fresh sync
        Time monthTotal = studyTracker.get_month_total_time(currentMonth);
        QString monthName = MONTH_NAMES[currentMonth];
        QMessageBox::information(this, "Month Total",
            QString("<h3>Total Study Time for %1</h3><br><font size='+2' color='#7c3aed'><b>%2 (%3 mins)</b></font>")
                .arg(monthName)
                .arg(QString::fromStdString(monthTotal.toString()))
                .arg(monthTotal.getTotalMinutes()));
    } else {
        QMessageBox::information(this, "Option 2", "Coming soon.");
    }
}

void MainWindow::onOption3Clicked() {
    if (currentTracker == "Expense Tracker") {
        expenseTracker.setup_tracker(); // Ensure fresh sync
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
                              "<tr><td><b>Food:</b></td><td>$%2</td></tr>"
                              "<tr><td><b>Stationary:</b></td><td>$%3</td></tr>"
                              "<tr><td><b>Travel:</b></td><td>$%4</td></tr>"
                              "<tr><td><b>Lifestyle:</b></td><td>$%5</td></tr>"
                              "<tr><td><b>Miscellaneous:</b></td><td>$%6</td></tr>"
                              "<tr style='border-top: 1px solid gray;'><td><b>Total:</b></td><td><b>$%7</b></td></tr>"
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
        fitnessTracker.setup_tracker(); // Ensure fresh sync
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
        studyTracker.setup_tracker(); // Ensure fresh sync
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
    } else {
        QMessageBox::information(this, "Option 3", "Coming soon.");
    }
}
