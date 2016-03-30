#include <application.h>
#include <QQmlEngine>
#include <QtQml>
#include <QDebug>
#include <QtNetwork>

namespace {
    const int maj = 1;
    const int min = 0;
}

static const int totalBytes = 50 * 1024 * 1024;
static const int payloadSize = 64 * 1024; // 64 KB

Application::Application(QQmlEngine *qEng, QJSEngine *jEng)
    : qmlSingletonPattern<Application>(*this)
    , m_qEng(qEng)
    , m_jsEng(jEng)
{
    connect(&tcpServer, &QTcpServer::newConnection, this, &Application::onNewConnection);
}

void Application::registerInstance()
{
    qmlRegisterSingletonType<Application>("com.qtuploadClientServer", maj, min,
                                          "App", &Application::qmlInstance);
}

void Application::start()
{
    if (!tcpServer.isListening()
            && !tcpServer.listen(QHostAddress::LocalHost, 4321)) {
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

void Application::displayError(QAbstractSocket::SocketError socketError)
{
    if (socketError == QTcpSocket::RemoteHostClosedError)
        return;

    tcpServer.close();
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
        QFile file("C:\\games\\rec.flv");
        file.open(QIODevice::WriteOnly);
        file.write(array);

        nextBlockSize = 0;
    }
}
