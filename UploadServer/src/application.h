#pragma once

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

//#include <QSharedPointer>

#include <qmlsingletonpattern.h>

QT_FORWARD_DECLARE_CLASS(QQmlEngine)
QT_FORWARD_DECLARE_CLASS(QJSEngine)

QT_BEGIN_NAMESPACE
class QTcpServer;
class QTcpSocket;
QT_END_NAMESPACE


// It is unused
class TcpServerProxy: public QTcpServer
{
    Q_OBJECT

public:
    explicit TcpServerProxy(QObject *parent = 0);
    virtual void incomingConnection(qintptr socketDescriptor);

Q_SIGNALS:
    void incomingConnectionSignal(qintptr socketDescriptor);
};



class Application : public QObject, public qmlSingletonPattern<Application>
{
    Q_OBJECT

    Q_PROPERTY(QString destPath READ destPath WRITE setDestPath NOTIFY destPathChanged)
    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)

public:
    explicit Application(QQmlEngine *qEng, QJSEngine *jEng);

    static void registerInstance();

    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();

    int totalSize() const;
    int currentWritten() const;
    QString progressString() const;

    QString destPath() const;
    int port() const;

public slots:
    void setDestPath(QString destPath);
    void setPort(int port);

private Q_SLOTS:
    void onNewConnection();
    void pSocketReadyRead();
    void displayError(QAbstractSocket::SocketError socketError);
    void incomingConnection(qintptr socketDescriptor);

Q_SIGNALS:
    void newMessage(const QString& arg);

    void destPathChanged(QString destPath);
    void portChanged(int port);

private:
    QQmlEngine *m_qEng;
    QJSEngine *m_jsEng;

//    TcpServerProxy tcpServer;
    QTcpServer tcpServer;

    quint64 nextBlockSize;
    QString m_destPath;
    int m_port;
};
