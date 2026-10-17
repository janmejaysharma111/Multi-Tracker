#include "frontend.h"
#include <iostream>

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QFont>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("OOPS Multi-Tracker");
    resize(800, 600);

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    QLabel *title = new QLabel("OOPS MULTI-TRACKER");
    title->setAlignment(Qt::AlignCenter);

    QFont titleFont;
    titleFont.setPointSize(22);
    titleFont.setBold(true);

    title->setFont(titleFont);

    layout->addWidget(title);

    // Subtitle
    QLabel *subtitle = new QLabel(
        "Choose a tracker to get started"
        );

    subtitle->setAlignment(Qt::AlignCenter);

    layout->addWidget(subtitle);

    // Buttons
    expenseButton = new QPushButton("Expense Tracker");
    fitnessButton = new QPushButton("Fitness Tracker");
    foodButton = new QPushButton("Food Tracker");
    studyButton = new QPushButton("Study Tracker");
    createNewButton = new QPushButton("Create New Tracker");
    exitButton = new QPushButton("Exit");

    // Button sizing
    expenseButton->setMinimumHeight(45);
    fitnessButton->setMinimumHeight(45);
    foodButton->setMinimumHeight(45);
    studyButton->setMinimumHeight(45);
    createNewButton->setMinimumHeight(70);
    exitButton->setMinimumHeight(40);

    // Add buttons to layout
    layout->addSpacing(80);
    layout->addWidget(expenseButton);
    layout->addWidget(fitnessButton);
    layout->addWidget(foodButton);
    layout->addWidget(studyButton);
    layout->addWidget(createNewButton);

    layout->addStretch();
    layout->addWidget(exitButton);

    // Connect buttons to functions
    connect(
        expenseButton,
        &QPushButton::clicked,
        this,
        &MainWindow::openExpenseTracker
        );

    connect(
        fitnessButton,
        &QPushButton::clicked,
        this,
        &MainWindow::openFitnessTracker
        );

    connect(
        foodButton,
        &QPushButton::clicked,
        this,
        &MainWindow::openFoodTracker
        );

    connect(
        studyButton,
        &QPushButton::clicked,
        this,
        &MainWindow::openStudyTracker
        );

    connect(
        createNewButton,
        &QPushButton :: clicked,
        this,
        &MainWindow :: createNewTracker
        );

    connect(
        exitButton,
        &QPushButton::clicked,
        qApp,
        &QApplication::quit
        );
}

MainWindow::~MainWindow()
{
    std :: cout << "window dead\n";
}

void MainWindow::openExpenseTracker()
{
    QMessageBox::information(
        this,
        "Expense Tracker",
        "Expense Tracker page will be implemented here."
        );
}

void MainWindow::openFitnessTracker()
{
    QMessageBox::information(
        this,
        "Fitness Tracker",
        "Fitness Tracker page will be implemented here."
        );
}

void MainWindow::openFoodTracker()
{
    QMessageBox::information(
        this,
        "Food Tracker",
        "Food Tracker page will be implemented here."
        );
}

void MainWindow::openStudyTracker()
{
    QMessageBox::information(
        this,
        "Study Tracker",
        "Study Tracker page will be implemented here."
        );
}

void MainWindow::createNewTracker()
{
    QMessageBox::information(
        this,
        "New Tracker",
        "Create New Tracker page will be implemented here."
        );
}