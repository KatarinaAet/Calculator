#ifndef FUNCTIONFIRST_H
#define FUNCTIONFIRST_H
#include <QScriptEngine>
#include <QJSEngine>
#include <QJSValue>
#include <QJsonArray>
#include <QObject>
#include <QAction>
#include <QString>
#include <QJsonObject>

class FuncionFirst:public QObject
{
    Q_OBJECT
public:
    FuncionFirst(){
    }
public slots:
    float sumic(float a,float b ){
        return a+b+a+b;
    }


};

#endif // FUNCTIONFIRST_H
