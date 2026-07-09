#include "private/ServiceManager_p.h"

#include <QCoreApplication>
#include <QProcessEnvironment>

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>

namespace Lingmo {

static ServiceManager *s_instance = nullptr;

// sd_notify via raw Unix socket (no Qt dependency)
static bool sdNotify(const char *state)
{
    const QString socketPath = QProcessEnvironment::systemEnvironment()
        .value(QStringLiteral("NOTIFY_SOCKET"));

    if (socketPath.isEmpty())
        return false;

    const QByteArray pathUtf8 = socketPath.toUtf8();
    const char *path = pathUtf8.constData();

    struct sockaddr_un addr{};
    addr.sun_family = AF_UNIX;

    bool isAbstract = false;
    if (path[0] == '@') {
        // Abstract socket: skip '@', use sun_path[0] = '\0'
        isAbstract = true;
        addr.sun_path[0] = '\0';
        strncpy(addr.sun_path + 1, path + 1, sizeof(addr.sun_path) - 2);
    } else {
        strncpy(addr.sun_path, path, sizeof(addr.sun_path) - 1);
    }

    const socklen_t addrLen = static_cast<socklen_t>(
        offsetof(struct sockaddr_un, sun_path)
        + (isAbstract ? strlen(path) : strlen(addr.sun_path))
        + (isAbstract ? 1 : 0));

    const int fd = socket(AF_UNIX, SOCK_DGRAM | SOCK_CLOEXEC, 0);
    if (fd < 0) return false;

    const bool ok = (sendto(fd, state, strlen(state), 0,
                            reinterpret_cast<struct sockaddr *>(&addr), addrLen) > 0);
    close(fd);
    return ok;
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
