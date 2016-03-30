#include "mythread.h"


MyThread::MyThread(qintptr ID, QObject *parent):
    QThread(parent)
{
    this->socketDescriptor = ID;
}

void MyThread::run()
{
    qDebug() << " Thread started";

    socket = new QTcpSocket();

    // Set the descriptor
    if(!socket->setSocketDescriptor(this->socketDescriptor))
    {
        Q_EMIT error(socket->error());
        return;
    }

    // Qt::DirectConnection is used because it's multithreaded
    connect(socket, &QTcpSocket::readyRead,
            this, &MyThread::readyRead, Qt::DirectConnection);
    connect(socket, &QTcpSocket::disconnected,this, &MyThread::disconnected);

    qDebug() << socketDescriptor << " Client connected";
    exec();
}

void MyThread::readyRead()
{
    QByteArray Data = socket->readAll();
    qDebug() << socketDescriptor << " Data in: " << Data;
    socket->write(Data);
}

void MyThread::disconnected()
{
    qDebug() << socketDescriptor << " Disconnected";
    socket->deleteLater();
    exit(0);
}
