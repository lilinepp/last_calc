#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , main_calc(new Transit())
{
    ui->setupUi(this);
    ui->input->setText("0");
    ui->output->setText("");



    // ----- цифры
    connect(ui->b_0,        SIGNAL(clicked()), this, SLOT(processing()));
    connect(ui->b_1,        SIGNAL(clicked()), this, SLOT(processing()));
    connect(ui->b_2,        SIGNAL(clicked()), this, SLOT(processing()));
    connect(ui->b_3,        SIGNAL(clicked()), this, SLOT(processing()));
    connect(ui->b_4,        SIGNAL(clicked()), this, SLOT(processing()));
    connect(ui->b_5,        SIGNAL(clicked()), this, SLOT(processing()));
    connect(ui->b_6,        SIGNAL(clicked()), this, SLOT(processing()));
    connect(ui->b_7,        SIGNAL(clicked()), this, SLOT(processing()));
    connect(ui->b_8,        SIGNAL(clicked()), this, SLOT(processing()));
    connect(ui->b_9,        SIGNAL(clicked()), this, SLOT(processing()));

    // ----- арифметика
    connect(ui->b_plus,     SIGNAL(clicked()), this, SLOT(processing()));
    connect(ui->b_minus,    SIGNAL(clicked()), this, SLOT(processing()));
    connect(ui->b_multiple, SIGNAL(clicked()), this, SLOT(processing()));
    connect(ui->b_division, SIGNAL(clicked()), this, SLOT(processing()));
    connect(ui->b_power,    SIGNAL(clicked()),this, SLOT(processing()));

    // ----- тригонометрия
    connect(ui->b_sin,      SIGNAL(clicked()), this, SLOT(processing()));
    connect(ui->b_cos,      SIGNAL(clicked()), this, SLOT(processing()));
    connect(ui->b_tan,      SIGNAL(clicked()), this, SLOT(processing()));
    connect(ui->b_ctg,      SIGNAL(clicked()), this, SLOT(processing()));

    // ----- прочее
    connect(ui->b_ln,       SIGNAL(clicked()), this, SLOT(processing()));
    connect(ui->b_point,    SIGNAL(clicked()), this, SLOT(processing()));
    connect(ui->b_left,     SIGNAL(clicked()), this, SLOT(processing()));
    connect(ui->b_right,    SIGNAL(clicked()), this, SLOT(processing()));
    connect(ui->b_uminus,   SIGNAL(clicked()), this, SLOT(processing()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete main_calc;
}

// ----- обработка входящего символа
// --
// --
void MainWindow::processing()
{
    QPushButton *__button = (QPushButton *)sender();
    std::string __buf = __button->text().toStdString();
    main_calc->Insert(__buf);
    ui->input->setText(main_calc->Show());
}

// ----- вычислить выражение
// --
// --
void MainWindow::on_b_equals_clicked()
{
    std::stringstream result_buf;
    try
    {
        result_buf << main_calc->Calculate();
        ui->output->setText(QString::fromStdString(result_buf.str()));
    }
    catch (std::invalid_argument& err)
    {
        ui->output->setText("ERROR");
        ui->statusbar->showMessage(err.what());
    }
}

// ----- очистить выражение
// --
// --
void MainWindow::on_b_clear_clicked()
{
    main_calc->Clear();
    ui->input->setText(main_calc->Show());
    ui->output->setText("");
    ui->statusbar->showMessage("");
}

// ----- стереть один символ
// --
// --
void MainWindow::on_b_back_clicked()
{
    main_calc->Erase();
    ui->input->setText(main_calc->Show());
}

// ----- информация об авторе
// --
// --
void MainWindow::on_actionAbout_auther_triggered()
{
    QMessageBox::information(this, "Author", "Myasoedova Alina");
}

