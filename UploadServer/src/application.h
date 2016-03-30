#pragma once

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#include <QSharedPointer>

#include <qmlsingletonpattern.h>

QT_FORWARD_DECLARE_CLASS(QQmlEngine)
QT_FORWARD_DECLARE_CLASS(QJSEngine)

QT_BEGIN_NAMESPACE
class QTcpServer;
class QTcpSocket;
QT_END_NAMESPACE

class Application : public QObject, public qmlSingletonPattern<Application>
{
    Q_OBJECT

public:
    explicit Application(QQmlEngine *qEng, QJSEngine *jEng);

    static void registerInstance();

    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();

    int totalSize() const;
    int currentWritten() const;
    QString progressString() const;
    bool buttonsAreLocked() const;

private Q_SLOTS:
    void onNewConnection();
    void pSocketReadyRead();
    void displayError(QAbstractSocket::SocketError socketError);

Q_SIGNALS:
    void newMessage(const QString& arg);

private:
    QQmlEngine *m_qEng;
    QJSEngine *m_jsEng;

    QTcpServer tcpServer;

    quint64 nextBlockSize;
};
