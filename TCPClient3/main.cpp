#include "mainwindow.h"
#include <QApplication>
#include "clientwidgets.h"
#include "SpoScriptEngine.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    SpoJSon * _scriptEngine;

    _scriptEngine = new SpoJSon();

    /*ClientWidgets *v = new ClientWidgets(_scriptEngine);

    v->setMinimumWidth(800);
    v->setMinimumHeight(400);
    v->show();

    ClientWidgets *h = new ClientWidgets(_scriptEngine);

    h->setMinimumWidth(800);
    h->setMinimumHeight(400);
    h->show();
    */

    return a.exec();
}
