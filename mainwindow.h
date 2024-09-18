#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QMessageBox>
#include <string>
#include <sstream>

#include "transit.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    //слот обработки входящего числа
    void processing();

    // вычисление выражения
    void on_b_equals_clicked();

    // очистка выражения
    void on_b_clear_clicked();

    // стереть один символ из выражения
    void on_b_back_clicked();


    void on_actionAbout_auther_triggered();

private:
    Ui::MainWindow *ui;
    Transit* main_calc;
};
#endif // MAINWINDOW_H
