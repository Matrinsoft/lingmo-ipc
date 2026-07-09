#include "private/ServiceManager_p.h"

#include <QCoreApplication>
#include <QProcessEnvironment>
#include <QLocalSocket>

namespace Lingmo {

static ServiceManager *s_instance = nullptr;

// sd_notify implementation via direct socket connection
static bool sdNotify(const char *state)
{
    const QString socketPath = QProcessEnvironment::systemEnvironment()
        .value(QStringLiteral("NOTIFY_SOCKET"));

    if (socketPath.isEmpty())
        return false;

    // Abstract socket
    if (socketPath.startsWith(QLatin1Char('@'))) {
        // Abstract socket not supported via Qt, fall back
        return false;
    }

    // Unix socket
    QLocalSocket socket;
    socket.connectToServer(socketPath);
    if (socket.state() != QLocalSocket::ConnectedState)
        return false;

    const QByteArray msg = QByteArray::fromRawData(state, static_cast<int>(strlen(state)));
    socket.write(msg);
    socket.flush();
    socket.disconnectFromServer();
    return true;
}

ServiceManager::ServiceManager(QObject *parent)
    : QObject(parent)
    , d(std::make_unique<ServiceManagerPrivate>())
{
    d->watchdogTimer = new QTimer(this);
    connect(d->watchdogTimer, &QTimer::timeout, this, [this]() {
        setWatchdog();
    });
}

ServiceManager::~ServiceManager() = default;

ServiceManager *ServiceManager::instance()
{
    if (!s_instance) {
        s_instance = new ServiceManager;
    }
    return s_instance;
}

bool ServiceManager::setReady()
{
    return sdNotify("READY=1");
}

bool ServiceManager::setWatchdog()
{
    return sdNotify("WATCHDOG=1");
}

bool ServiceManager::setStatus(const QString &status)
{
    const QByteArray msg = "STATUS=" + status.toUtf8();
    return sdNotify(msg.constData());
}

bool ServiceManager::setMainPid(qint64 pid)
{
    if (pid < 0)
        pid = QCoreApplication::applicationPid();

    const QByteArray msg = QByteArray("MAINPID=") + QByteArray::number(pid);
    return sdNotify(msg.constData());
}

bool ServiceManager::setStopping()
{
    return sdNotify("STOPPING=1");
}

bool ServiceManager::isSystemdManaged()
{
    return !QProcessEnvironment::systemEnvironment()
        .value(QStringLiteral("INVOCATION_ID")).isEmpty();
}

bool ServiceManager::isWatchdogEnabled() const
{
    return d->watchdogEnabled;
}

void ServiceManager::setWatchdogEnabled(bool enabled)
{
    d->watchdogEnabled = enabled;
    if (enabled) {
        // Read watchdog usec from environment
        const QString usecStr = QProcessEnvironment::systemEnvironment()
            .value(QStringLiteral("WATCHDOG_USEC"));
        if (!usecStr.isEmpty()) {
            d->watchdogIntervalMs = usecStr.toLongLong() / 1000 / 2; // Half the interval
            d->watchdogTimer->start(d->watchdogIntervalMs);
        }
    } else {
        d->watchdogTimer->stop();
    }
}

} // namespace Lingmo
