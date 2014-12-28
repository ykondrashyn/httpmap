#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "nmap.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    char *res = read_ip_pull("/home/stanislav/Projects/httpmap/scan.txt");
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
