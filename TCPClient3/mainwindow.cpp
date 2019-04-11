#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clientwidgets.h"
#include "SpoScriptEngine.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    SpoJSon * _scriptEngine;

    _scriptEngine = new SpoJSon();
    _scriptEngine->addObject("obj1", new Obj1());
    _scriptEngine->addObject("obj2", new Obj2());

    ClientWidgets *a = new ClientWidgets(_scriptEngine);

    a->setMinimumWidth(800);
    a->setMinimumHeight(400);
    setCentralWidget(a);
}

MainWindow::~MainWindow()
{
    delete ui;
}
