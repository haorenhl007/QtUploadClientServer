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
    Q_PROPERTY(int totalSize READ totalSize WRITE setTotalSize NOTIFY totalSizeChanged)
    Q_PROPERTY(int currentWritten READ currentWritten WRITE setCurrentWritten NOTIFY currentWrittenChanged)
    Q_PROPERTY(QString progressString READ progressString WRITE setProgressString NOTIFY progressStringChanged)
    Q_PROPERTY(bool buttonsAreLocked READ buttonsAreLocked WRITE setButtonsAreLocked NOTIFY buttonsAreLockedChanged)

public:
    explicit Application(QQmlEngine *qEng, QJSEngine *jEng);

    static void registerInstance();

    Q_INVOKABLE void start();

    int totalSize() const;
    int currentWritten() const;
    QString progressString() const;
    bool buttonsAreLocked() const;

public Q_SLOTS:
    void startTransfer();
    void updateClientProgress(qint64 numBytes);
    void displayError(QAbstractSocket::SocketError socketError);

    void setTotalSize(int totalSize);
    void setCurrentWritten(int currentWritten);
    void setProgressString(QString progressString);
    void setButtonsAreLocked(bool buttonsAreLocked);

Q_SIGNALS:
    void totalSizeChanged(int totalSize);
    void currentWrittenChanged(int currentWritten);
    void progressStringChanged(QString progressString);
    void buttonsAreLockedChanged(bool buttonsAreLocked);

    void newMessage(const QString& arg);

private:
    QQmlEngine *m_qEng;
    QJSEngine *m_jsEng;

    QTcpSocket tcpClient;

    int bytesToWrite;
    int bytesWritten;
    int bytesReceived;

    int m_totalSize;
    int m_currentWritten;
    QString m_progressString;
    bool m_buttonsAreLocked;
};
