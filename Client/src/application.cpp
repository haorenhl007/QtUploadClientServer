#include <application.h>
#include <QQmlEngine>
#include <QtQml>
#include <QDebug>
#include <QtNetwork>

namespace {
    const int maj = 1;
    const int min = 0;
}

static const int payloadSize = 64 * 1024; // 64 KB

Application::Application(QQmlEngine *qEng, QJSEngine *jEng)
    : qmlSingletonPattern<Application>(*this)
    , m_qEng(qEng)
    , m_jsEng(jEng)
    , m_totalSize(0)
    , m_buttonsAreLocked(false)
{
    connect(&tcpClient, &QTcpSocket::connected, this, &Application::onConnected);
    connect(&tcpClient, &QTcpSocket::bytesWritten,
            this, &Application::updateClientProgress);
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

void Application::updateClientProgress(qint64 numBytes)
{
    bytesWritten += (int)numBytes;

    if (tcpClient.bytesToWrite() < payloadSize)
        setButtonsAreLocked(false);

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

void Application::onConnected()
{
    QByteArray block;
    QDataStream stream(&block, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_4);

    QFile file("C:\\games\\test.flv"); ///////
    file.open(QIODevice::ReadOnly);
    QByteArray buf = file.readAll();
    stream << quint64(file.size());
    stream << buf;
    tcpClient.write(block);
    tcpClient.flush();

    setTotalSize((int)file.size());
}



