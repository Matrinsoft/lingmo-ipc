#include "private/DBusConnection_p.h"

#include <QDBusConnection>
#include <QDBusError>

namespace Lingmo {

static DBusConnection *s_sessionBus = nullptr;
static DBusConnection *s_systemBus = nullptr;

DBusConnection::DBusConnection(BusType type, QObject *parent)
    : QObject(parent)
    , d(std::make_unique<DBusConnectionPrivate>())
{
    d->busType = type;

    if (type == SessionBus) {
        d->conn = QDBusConnection::sessionBus();
    } else {
        d->conn = QDBusConnection::systemBus();
    }

    d->connected = d->conn->isConnected();

    if (!d->connected) {
        const char *busName = (type == SessionBus) ? "session" : "system";
        qWarning("LingmoIPC: Failed to connect to %s bus: %s",
                 busName,
                 qPrintable(d->conn->lastError().message()));
    }
}

DBusConnection::~DBusConnection() = default;

QDBusConnection DBusConnection::connection() const
{
    return d->conn.value();
}

bool DBusConnection::isConnected() const
{
    return d->connected;
}

bool DBusConnection::registerService(const QString &serviceName)
{
    if (!d->connected || !d->conn.has_value()) return false;

    const auto result = d->conn->registerService(serviceName);
    if (result) {
        emit serviceRegistered(serviceName);
    }
    return result;
}

bool DBusConnection::registerObject(const QString &path, QObject *object,
                                     const QString &interface)
{
    if (!d->connected || !d->conn.has_value() || !object) return false;

    if (interface.isEmpty()) {
        return d->conn->registerObject(path, object);
    }
    return d->conn->registerObject(path, interface, object);
}

bool DBusConnection::unregisterService(const QString &serviceName)
{
    if (!d->connected || !d->conn.has_value()) return false;

    const bool result = d->conn->unregisterService(serviceName);
    if (result) {
        emit serviceUnregistered(serviceName);
    }
    return result;
}

DBusConnection *DBusConnection::sessionBus(QObject *parent)
{
    if (!s_sessionBus) {
        s_sessionBus = new DBusConnection(SessionBus, parent);
    }
    return s_sessionBus;
}

DBusConnection *DBusConnection::systemBus(QObject *parent)
{
    if (!s_systemBus) {
        s_systemBus = new DBusConnection(SystemBus, parent);
    }
    return s_systemBus;
}

} // namespace Lingmo
