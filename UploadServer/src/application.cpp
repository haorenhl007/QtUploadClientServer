#include <application.h>
#include "mythread.h"
#include <QQmlEngine>
#include <QtQml>
#include <QDebug>
#include <QtNetwork>

namespace {
    const int maj = 1;
    const int min = 0;
}

Application::Application(QQmlEngine *qEng, QJSEngine *jEng)
    : qmlSingletonPattern<Application>(*this)
    , m_qEng(qEng)
    , m_jsEng(jEng)
    , m_port(4321)
{
    connect(&tcpServer, &QTcpServer::newConnection, this, &Application::onNewConnection);
//    connect(&tcpServer, &TcpServerProxy::incomingConnectionSignal,
//            this, &Application::incomingConnection);
}

void Application::registerInstance()
{
    qmlRegisterSingletonType<Application>("com.qtuploadClientServer", maj, min,
                                          "App", &Application::qmlInstance);
}

void Application::start()
{
    if (!tcpServer.isListening()
            && !tcpServer.listen(QHostAddress::Any, m_port)) {
        QString errorStr ("Unable to start server! ");
        errorStr.append(tcpServer.errorString());

        Q_EMIT newMessage(errorStr);
        return;
    }

    Q_EMIT newMessage("Listening");
}

void Application::stop()
{
    tcpServer.close();
    Q_EMIT newMessage("Stopped");
}

QString Application::destPath() const
{
    return m_destPath;
}

int Application::port() const
{
    return m_port;
}

void Application::setDestPath(QString destPath)
{
    if (m_destPath == destPath)
        return;

    destPath.remove("file:///");
    qDebug()<<"new path = "<< destPath;

    m_destPath = destPath;
    emit destPathChanged(destPath);
}

void Application::setPort(int port)
{
    if (m_port == port)
        return;
    m_port = port;
    emit portChanged(port);
}

void Application::displayError(QAbstractSocket::SocketError socketError)
{
    if (socketError == QTcpSocket::RemoteHostClosedError)
        return;

    tcpServer.close();
}

void Application::incomingConnection(qintptr socketDescriptor)
{
    QString message(tr("Connecting..."));
    Q_EMIT newMessage(message);

    MyThread *thread = new MyThread(socketDescriptor, this);

    connect(thread, &MyThread::finished, thread, &MyThread::deleteLater);

    thread->start();
}

void Application::onNewConnection()
{
    QTcpSocket *pSocket = tcpServer.nextPendingConnection();
    connect (pSocket, SIGNAL(disconnected()), pSocket, SLOT(deleteLater()));
    connect (pSocket, SIGNAL(readyRead()), this, SLOT(pSocketReadyRead()));
    nextBlockSize = 0;
}

void Application::pSocketReadyRead()
{
    QTcpSocket * socket = static_cast<QTcpSocket*>(sender());

    forever {
        QDataStream stream(socket);
        stream.setVersion(QDataStream::Qt_5_4);

        if (nextBlockSize == 0) {
            if (socket->bytesAvailable() < sizeof(quint64))
                return;
            stream >> nextBlockSize;
        }

        if (nextBlockSize > socket->bytesAvailable())
            return;

        QByteArray array;

        stream >> array;
        QFile file(m_destPath);
        file.open(QIODevice::WriteOnly);
        file.write(array);

        nextBlockSize = 0;
    }
}

TcpServerProxy::TcpServerProxy(QObject *parent) :
    QTcpServer(parent)
{
}

void TcpServerProxy::incomingConnection(qintptr socketDescriptor)
{
    Q_EMIT incomingConnection(socketDescriptor);
}
