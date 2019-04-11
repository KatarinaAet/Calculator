#ifndef _MyServer_h_
#define _MyServer_h_
#include <QWidget>
#include <QTcpServer>
#include <QTextEdit>
#include <QTcpSocket>
#include <QTimerEvent>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QTime>
#include <QLineEdit>
#include <QDebug>
#include <QJSEngine>
#include <QJSValue>
#include "SpoScriptEngine.h"
#include <QMutex>
#include <QDataStream>
#include <QByteArray>

class ServerTCP: public QWidget {
Q_OBJECT
    int _timer;
    QString _stringLog = "";
public:
    ServerTCP(SpoJSon *Engine,int nPort, QWidget* pwgt) : QWidget(pwgt), _nNextBlockSize(0){

        _engine = Engine;
        _engine->addObject("console",this);

        _tcpServer = new QTcpServer(this);
        if (!_tcpServer->listen(QHostAddress::Any, nPort)) {
            QMessageBox::critical(0,"Server Error","Unable to start the server:"+ _tcpServer->errorString());
            _tcpServer->close();
            return;
        }
        connect(_tcpServer, SIGNAL(newConnection()),this, SLOT(slotNewConnection()));

        _logOfServer = new QTextEdit;
        _logOfServer->setReadOnly(true);

        //Layout setup
        QVBoxLayout* pvbxLayout = new QVBoxLayout;
        pvbxLayout->addWidget(new QLabel("<H1>Server</H1>"));
        pvbxLayout->addWidget(_logOfServer);
        setLayout(pvbxLayout);
        _timer = this->startTimer(1000);
    }

    void sendToClient(QTcpSocket* _pSocket, const QString& str){
        qDebug()<<"Зашли в сенд";


        QByteArray  arrBlock;
        QDataStream out(&arrBlock, QIODevice::WriteOnly);
        QString _log;
        /*
        foreach (Client _client, _listOfClient){
            if(_client._clientSocket == _pSocket){
                _log = _client._stringLog;
            }
        }
        */
        //out << quint16(0) << QTime::currentTime() << str << _log;

        out << quint16(0) << QTime::currentTime() << str << _stringLog;

        out.device()->seek(0);
        out << quint16(arrBlock.size() - sizeof(quint16));

        _stringLog = "";

        _pSocket->write(arrBlock);
    }
private:
    struct Client
    {
        QTcpSocket* _clientSocket;
        int _time;//3 минуты время работы клиента на сервере, если не было отклика
        //QString _stringLog="";
        Client(){
        }
    };
    QMutex *_mutex;
    QMutexLocker *_locker;
    QTcpServer* _tcpServer;
    QTextEdit*  _logOfServer;
    quint16  _nNextBlockSize;
    SpoJSon *_engine;
    QList<Client> _listOfClient;



public slots:
    void log(QString var){
        //
        if(!_stringLog.isEmpty()){
         _stringLog.append("\n");
        }
        _stringLog.append("log: " + var);

    }
    virtual void slotNewConnection(){
        Client _client;
        _client._time = 180;
        _client._clientSocket = _tcpServer->nextPendingConnection();

        connect(_client._clientSocket,SIGNAL(disconnected()),this,SLOT(disconnectedFromServer()));
        connect(_client._clientSocket,SIGNAL(readyRead()),this,SLOT(slotReadClient()));
        sendToClient(_client._clientSocket, "Server Response: Connected!");

        _listOfClient.append(_client);
    }
    void slotReadClient   (){
        QTcpSocket* pClientSocket = (QTcpSocket*)sender();

        int i=0;
        foreach (Client _client, _listOfClient){
            if(_client._clientSocket == pClientSocket){
                _listOfClient[i]._time = 180;
            }
            i++;
        }
        QDataStream in(pClientSocket);
        for (;;) {
            if (!_nNextBlockSize) {

                if (pClientSocket->bytesAvailable() < sizeof(quint16)) {
                    break;
                }

            if (pClientSocket->bytesAvailable())
                in >> _nNextBlockSize;
            }

            if (pClientSocket->bytesAvailable() < _nNextBlockSize) {
                break;
            }

            QTime   time;
            QString str;

            in >> time >> str;
            QString strMessage = time.toString() + " Client has sended - " + str;
            _logOfServer->append(strMessage);

            QString result = _engine->parseMsg(str);
            qDebug()<<result;

            _nNextBlockSize = 0;
            sendToClient(pClientSocket,"Server Response: " + result);
        }
        //_mutex->unlock();
    }
    void timerEvent(QTimerEvent *e) {
        //при каждом вызове таймера мы должы пробегаться по листу наших клиентов и уменьшать их время жизни
        if(e->timerId() == _timer) {
            int i=0;
            foreach (Client _client, _listOfClient){
                _listOfClient[i]._time--;
                qDebug()<<_listOfClient[i]._time;
                if(_client._time == 0){
                    sendToClient(_listOfClient[i]._clientSocket,"Server Response:Disconnected(timeout expired)");
                    emit _listOfClient[i]._clientSocket->disconnect();
                    _listOfClient.removeAt(i);
                    continue;
                }
                i++;
            }
        }
    }
    void disconnectedFromServer(){
        //при дисконнекте одного из пользователей мы должны удалить его из списка пользователей
            QTcpSocket *socket;
            socket = (QTcpSocket*)sender();
            int i=0;
            foreach (Client _client, _listOfClient){
                if(_client._clientSocket == socket){
                    _client._clientSocket->close();
                    _listOfClient.removeAt(i);
                }
                i++;
            }
    }
};
#endif  //_MyServer_h_
