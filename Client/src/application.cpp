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
    , m_port(4321)
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

    if (!m_host.length() || m_host.contains("localhost", Qt::CaseInsensitive))
        tcpClient.connectToHost(QHostAddress::LocalHost, m_port);
    else
        tcpClient.connectToHost(m_host, m_port);
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

QString Application::filePath() const
{
    return m_filePath;
}

QString Application::host() const
{
    return m_host;
}

int Application::port() const
{
    return m_port;
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

void Application::setFilePath(QString filePath)
{
    if (m_filePath == filePath)
        return;

    filePath.remove("file:///");
    qDebug()<<"new path = "<< filePath;

    m_filePath = filePath;
    emit filePathChanged(filePath);
}

void Application::setHost(QString host)
{
    if (m_host == host)
        return;
    m_host = host;
    emit hostChanged(host);
}

void Application::setPort(int port)
{
    if (m_port == port)
        return;
    m_port = port;
    emit portChanged(port);
}

void Application::onConnected()
{
    QByteArray block;
    QDataStream stream(&block, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_4);

    QFile file(m_filePath);
    file.open(QIODevice::ReadOnly);
    QByteArray buf = file.readAll();
    stream << quint64(file.size());
    stream << buf;
    tcpClient.write(block);
    tcpClient.flush();

    setTotalSize((int)file.size());
}
