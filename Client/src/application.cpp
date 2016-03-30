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
    , m_totalSize(0)
    , m_buttonsAreLocked(false)
{
        connect(&tcpClient, SIGNAL(connected()), this, SLOT(startTransfer()));
        connect(&tcpClient, SIGNAL(bytesWritten(qint64)),
                this, SLOT(updateClientProgress(qint64)));
        connect(&tcpClient, SIGNAL(error(QAbstractSocket::SocketError)),
                this, SLOT(displayError(QAbstractSocket::SocketError)));
}

void Application::registerInstance()
{
    qmlRegisterSingletonType<Application>("com.qtuploadClientServer", maj, min,
                                          "App", &Application::qmlInstance);
}

void Application::start()
{
    setButtonsAreLocked(true);
    bytesWritten = 0;
    bytesReceived = 0;

    tcpClient.connectToHost(QHostAddress::LocalHost, 4321);
    tcpClient.waitForConnected(5000);
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

void Application::startTransfer()
{
    bytesToWrite = totalBytes - (int)tcpClient.write(QByteArray(payloadSize, '@'));
    Q_EMIT newMessage("Connected");
}

void Application::updateClientProgress(qint64 numBytes)
{
    bytesWritten += (int)numBytes;

    if (bytesToWrite > 0 && tcpClient.bytesToWrite() <= 4*payloadSize)
        bytesToWrite -= (int)tcpClient.write(QByteArray(qMin(bytesToWrite, payloadSize), '@'));
    else
        setButtonsAreLocked(false);

    setTotalSize(totalBytes);
    setCurrentWritten(bytesWritten);
    setProgressString(tr("%1MB").arg(bytesWritten / (1024 * 1024)));
}

void Application::displayError(QAbstractSocket::SocketError socketError)
{
    if (socketError == QTcpSocket::RemoteHostClosedError)
        return;

    QString errStr("Network error: ");
    errStr.append(tcpClient.errorString());
    Q_EMIT newMessage(errStr);

    tcpClient.close();
    setTotalSize(0);
    setCurrentWritten(0);
    setProgressString("");
    Q_EMIT newMessage("Client ready");
    setButtonsAreLocked(false);
}

void Application::setTotalSize(int totalSize)
{
    if (m_totalSize == totalSize)
        return;
    m_totalSize = totalSize;
    Q_EMIT totalSizeChanged(totalSize);
}

void Application::setCurrentWritten(int currentWritten)
{
    if (m_currentWritten == currentWritten)
        return;
    m_currentWritten = currentWritten;
    Q_EMIT currentWrittenChanged(currentWritten);
}

void Application::setProgressString(QString progressString)
{
    if (m_progressString == progressString)
        return;
    m_progressString = progressString;
    Q_EMIT progressStringChanged(progressString);
}

void Application::setButtonsAreLocked(bool buttonsAreLocked)
{
    if (m_buttonsAreLocked == buttonsAreLocked)
        return;
    m_buttonsAreLocked = buttonsAreLocked;
    Q_EMIT buttonsAreLockedChanged(buttonsAreLocked);
}



