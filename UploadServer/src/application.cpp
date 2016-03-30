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
    , tcpServerConnection(nullptr)
    , m_totalSize(0)
    , m_buttonsAreLocked(false)
{
        connect(&tcpServer, &QTcpServer::newConnection,
                this, &Application::acceptConnection);
}

void Application::registerInstance()
{
    qmlRegisterSingletonType<Application>("com.qtuploadClientServer", maj, min,
                                          "App", &Application::qmlInstance);
}

void Application::start()
{
    while (!tcpServer.isListening() && !tcpServer.listen(QHostAddress::LocalHost, 4321)) {
        QString errorStr ("Unable to start server! ");
        errorStr.append(tcpServer.errorString());

        qDebug()<<errorStr;

//        if (ret == Cancel) {
        Q_EMIT newMessage(errorStr);
        return;
//        }
    }

    Q_EMIT newMessage("Listening");
}

int Application::totalSize() const
{
    return m_totalSize;
}

int Application::currentWritten() const
{
    return m_currentWritten;
}

QString Application::progressString() const
{
    return m_progressString;
}

bool Application::buttonsAreLocked() const
{
    return m_buttonsAreLocked;
}

void Application::acceptConnection()
{
    tcpServerConnection = tcpServer.nextPendingConnection();

    connect(tcpServerConnection, &QTcpSocket::readyRead,
            this, &Application::updateServerProgress);

//    connect(tcpServerConnection, &QTcpSocket::error,
//            this, &Application::displayError);

    Q_EMIT newMessage("Accepted connection");

    tcpServer.close();
}

void Application::updateServerProgress()
{
    bytesReceived += (int)tcpServerConnection->bytesAvailable();
    tcpServerConnection->readAll();

//    setTotalSize(totalBytes);
//    setCurrentWritten(bytesWritten);
//    setProgressString(tr("Sent %1MB").arg(bytesWritten / (1024 * 1024)));

    if (bytesReceived == totalBytes) {
        tcpServerConnection->close();
    }
}

void Application::displayError(QAbstractSocket::SocketError socketError)
{
    if (socketError == QTcpSocket::RemoteHostClosedError)
        return;

//    tcpClient.close();
    tcpServer.close();
    setTotalSize(0);
    setCurrentWritten(0);
    setProgressString("");

    setButtonsAreLocked(false);

}

void Application::setTotalSize(int totalSize)
{
    if (m_totalSize == totalSize)
        return;
    m_totalSize = totalSize;
    emit totalSizeChanged(totalSize);
}

void Application::setCurrentWritten(int currentWritten)
{
    if (m_currentWritten == currentWritten)
        return;
    m_currentWritten = currentWritten;
    emit currentWrittenChanged(currentWritten);
}

void Application::setProgressString(QString progressString)
{
    if (m_progressString == progressString)
        return;
    m_progressString = progressString;
    emit progressStringChanged(progressString);
}

void Application::setButtonsAreLocked(bool buttonsAreLocked)
{
    if (m_buttonsAreLocked == buttonsAreLocked)
        return;
    m_buttonsAreLocked = buttonsAreLocked;
    emit buttonsAreLockedChanged(buttonsAreLocked);
}



