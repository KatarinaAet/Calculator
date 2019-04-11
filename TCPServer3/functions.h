#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <QScriptEngine>
#include <QJSEngine>
#include <QJSValue>
#include <QJsonArray>
#include <QObject>
#include <QAction>
#include <QString>
#include <QJsonObject>

class FunctionsObject : public QObject
{
    Q_OBJECT

public:
    FunctionsObject(){
    }
private:

public slots:
    float Sum(float a,float b){
        return a+b;
    }
    float Razn(float a,float b){
        return a-b;
    }
    float Del(float a,float b){
        return a/b;
    }    
    QJsonObject obj(QString str,QJsonObject obj2){
        obj2.insert(str,1);
        return obj2;
    }






};

#endif //FUNCTIONS_H
