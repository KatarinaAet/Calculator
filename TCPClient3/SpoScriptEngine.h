#ifndef SCRIPT_H
#define SCRIPT_H
#include <QScriptEngine>
#include <QJSEngine>
#include <QJSValue>
#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>

class Obj1:public QObject{
    Q_OBJECT
public slots:
    float sum(float a, float b) {return a+b;}
    float mul(float a, float b) {return a*b;}
};

class Obj2:public QObject{
    Q_OBJECT
public slots:

    float sub(float a, float b) {return a-b;}
    float div(float a, float b) {return a/b;}

};

class SpoJSon : public QObject
{
    Q_OBJECT

public:
    SpoJSon(){}
    void addObject(const QString name, QObject *obj){
        QJSValue _newEngineObject = _engine.newQObject(obj);
        _engine.globalObject().setProperty(name,_newEngineObject);
    }
    QString parseMsg(const QString &msg) {
         QJSValue result = _engine.evaluate(msg);
         QString res = result.toString();
         if(result.isError()) {
            QJsonObject jobj;
            jobj.insert("error", "script:"+res);
            jobj.insert("src", msg);
            QJsonDocument doc(jobj);
            return doc.toJson();
         }
         else {
             return res;
         }
    }
private:
    QJSEngine _engine;
};

#endif // SCRIPT_H

