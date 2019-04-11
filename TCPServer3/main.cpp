#include "mainwindow.h"
#include <QApplication>
#include <servertcp.h>
#include <SpoScriptEngine.h>
//#include <functionfirst.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    SpoJSon * _engine;
    _engine = new SpoJSon();

    _engine->addObject("obj1", new Obj1());
    _engine->addObject("obj2", new Obj2());

    ServerTCP *_server = new ServerTCP(_engine,2323,NULL);
    _server->show();

    return a.exec();
}
