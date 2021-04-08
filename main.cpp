#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QDataStream>
#include <QTimer>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonDocument>


class BackendStuff : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString receiveBuffer READ getBuffer NOTIFY bufferChanged)

public:
    explicit BackendStuff(QObject *parent = nullptr);
    QTcpSocket *tcpSocket;

    QString getBuffer(){
        if (tcpSocket->bytesAvailable() > (quint16)sizeof(quint16) )
            {
                 receive_dump.clear();
                 receive_dump.append(tcpSocket->readAll());
            }
        m_receivebuffer = ((QString)receive_dump).remove(0,2);
        return m_receivebuffer;
    };

    //from QAfbWebSocketClient
    using closure_t = std::function<void(bool, const QJsonValue&)>;
    void call(const QString& api, const QString& verb, const QJsonValue& arg = QJsonValue(), closure_t closure = nullptr);

signals:
    void statusChanged(QString newStatus);
    void someError(QString err);
    void statusChanged2(bool);
    void hasReadSome(QString msg);
    void bufferChanged();

public slots:
    void gotError(QAbstractSocket::SocketError err);
    void sendClicked(QString msg);
    void sendMessage(QString api, QString verb, QJsonValue arg);
    void connectClicked();
    void disconnectClicked();
    void closeConnection();
    void connect2host();

private slots:
    void readyRead();
    void connected();
    void connectionTimeout();

private:
    //from QAfbWebSocketClient
    int m_nextCallId, port;
    QMap<QString, closure_t> m_closures;
    QString host, m_receivebuffer ;
    bool status;
    QTimer *timeoutTimer;
    QByteArray receive_dump;
};

//constructor
BackendStuff::BackendStuff(QObject *parent) : QObject(parent)
{
    status = false;
    tcpSocket = new QTcpSocket();

    host = "127.0.0.1";
    port = 1234;

    timeoutTimer = new QTimer();
    timeoutTimer->setSingleShot(true);
    connect(timeoutTimer, &QTimer::timeout, this, &BackendStuff::connectionTimeout);

    //connect(client, &BackendStuff::statusChanged, this, &Backend::setStatus);
    //connect(client->tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(gotError(QAbstractSocket::SocketError)));
    connect(tcpSocket, &QTcpSocket::disconnected, this, &BackendStuff::closeConnection);
}

//call an api's verb with an argument
void BackendStuff::call(const QString &api, const QString &verb, const QJsonValue &arg, closure_t closure)
{
    QString callId = QString::number(m_nextCallId);
    m_closures[callId] = closure;

    QJsonArray msg;
    msg.append(2);  //refers to Call
    //msg.append(callId);
    msg.append("999999");
    msg.append(api + "/" + verb);
    msg.append(arg);

    m_nextCallId++;

    QJsonDocument value;
    value.setArray(msg);

    QByteArray AfbJson;
    AfbJson.append("\x81\x23");     //Byte-Wörder für SET_OPCODE usw. wichtig damit Array erkannt wird
    AfbJson.append(value.toJson(QJsonDocument::Compact));
    tcpSocket->write(AfbJson);
}

void BackendStuff::gotError(QAbstractSocket::SocketError err)
{
    QString strError = "unknown";
    switch (err)
    {
        case 0:
            strError = "Connection was refused";
            break;
        case 1:
            strError = "Remote host closed the connection";
            break;
        case 2:
            strError = "Host address was not found";
            break;
        case 5:
            strError = "Connection timed out";
            break;
        default:
            strError = "Unknown error";
    }

    emit someError(strError);
}

void BackendStuff::connectClicked()
{
    connect2host();
}

void BackendStuff::sendClicked(QString msg)
{
    QByteArray arrBlock;
    /*
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    //out.setVersion(QDataStream::Qt_5_10);
    out << quint16(0) << msg;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));
    */
    arrBlock = msg.toUtf8();
    tcpSocket->write(arrBlock);
}

void BackendStuff::sendMessage(QString api, QString verb, QJsonValue arg)
{
    call(api,verb,QJsonValue(arg),nullptr);
}

void BackendStuff::disconnectClicked()
{
    closeConnection();
}

//here the connection is executed via QAbstractSocket
void BackendStuff::connect2host()
{
    timeoutTimer->start(3000);
    tcpSocket->connectToHost(host, port);
    connect(tcpSocket, &QTcpSocket::connected, this, &BackendStuff::connected);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &BackendStuff::readyRead);
}

void BackendStuff::connectionTimeout()
{
    //qDebug() << tcpSocket->state();
    if(tcpSocket->state() == QAbstractSocket::ConnectingState)
    {
        tcpSocket->abort();
        emit tcpSocket->error(QAbstractSocket::SocketTimeoutError);
    }
}

void BackendStuff::connected()
{
    status = true;
    emit statusChanged2(status);
}

void BackendStuff::readyRead()
{
   if(getBuffer() != 0) emit bufferChanged();
}

void BackendStuff::closeConnection()
{
    timeoutTimer->stop();

    //qDebug() << tcpSocket->state();
    disconnect(tcpSocket, &QTcpSocket::connected, 0, 0);
    disconnect(tcpSocket, &QTcpSocket::readyRead, 0, 0);

    bool shouldEmit = false;
    switch (tcpSocket->state())
    {
        case 0:
            tcpSocket->disconnectFromHost();
            shouldEmit = true;
            break;
        case 2:
            tcpSocket->abort();
            shouldEmit = true;
            break;
        default:
            tcpSocket->abort();
    }

    if (shouldEmit)
    {
        status = false;
        emit statusChanged2(status);
    }
}


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<BackendStuff>("io.qt.BackendStuff", 1, 0, "BackendStuff");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty()) { return -1; }

    return app.exec();
}

#include <main.moc>
