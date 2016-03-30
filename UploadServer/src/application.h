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

public:
    explicit Application(QQmlEngine *qEng, QJSEngine *jEng);

    static void registerInstance();

    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();

    int totalSize() const;
    int currentWritten() const;
    QString progressString() const;
    bool buttonsAreLocked() const;

    QString destPath() const;

public slots:
    void setDestPath(QString destPath);

private Q_SLOTS:
    void onNewConnection();
    void pSocketReadyRead();
    void displayError(QAbstractSocket::SocketError socketError);
    void incomingConnection(qintptr socketDescriptor);

Q_SIGNALS:
    void newMessage(const QString& arg);

    void destPathChanged(QString destPath);

private:
    QQmlEngine *m_qEng;
    QJSEngine *m_jsEng;

//    TcpServerProxy tcpServer;
    QTcpServer tcpServer;

    quint64 nextBlockSize;
    QString m_destPath;
};
