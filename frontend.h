#ifndef FRONTEND_H
#define FRONTEND_H

#include <QMainWindow>

class QWidget;
class QPushButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    QWidget *centralWidget;

    QPushButton *expenseButton;
    QPushButton *fitnessButton;
    QPushButton *foodButton;
    QPushButton *studyButton;
    QPushButton *createNewButton;
    QPushButton *exitButton;

private slots:
    void openExpenseTracker();
    void openFitnessTracker();
    void openFoodTracker();
    void openStudyTracker();
    void createNewTracker();
};

#endif