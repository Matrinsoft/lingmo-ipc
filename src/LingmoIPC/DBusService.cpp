#include "private/DBusService_p.h"
#include <LingmoIPC/DBusConnection.h>

#include <QDBusMessage>
#include <QDBusConnection>
#include <QDBusInterface>

namespace Lingmo {

DBusService::DBusService(const QString &serviceName, QObject *parent)
    : QObject(parent)
    , d(std::make_unique<DBusServicePrivate>())
{
    d->serviceName = serviceName;
}

DBusService::~DBusService() = default;

QString DBusService::serviceName() const
{
    return d->serviceName;
}

void DBusService::setConnection(DBusConnection *connection)
{
    d->connection = connection;
}

DBusConnection *DBusService::connection() const
{
    return d->connection;
}

bool DBusService::registerInterface(const QString &path, const QString &interface)
{
    if (!d->connection) return false;

    // Register the service name
    if (!d->connection->registerService(d->serviceName))
        return false;

    // Store the interface info
    d->registeredInterfaces.insert(path + interface, {path, interface});

    return true;
}

bool DBusService::unregisterInterface(const QString &path, const QString &interface)
{
    const QString key = path + interface;
    return d->registeredInterfaces.remove(key) > 0;
}

void DBusService::emitSignal(const QString &path, const QString &interface,
                              const QString &signalName, const QVariantList &args)
{
    if (!d->connection || !d->connection->isConnected())
        return;

    QDBusMessage msg = QDBusMessage::createSignal(path, interface, signalName);
    msg.setArguments(args);
    d->connection->connection().send(msg);
}

void DBusService::setProperty(const QString &path, const QString &interface,
                               const QString &propertyName, const QVariant &value)
{
    Q_UNUSED(path);
    Q_UNUSED(interface);
    Q_UNUSED(propertyName);
    Q_UNUSED(value);
    // PropertiesChanged signal emission would go here
    emit propertyChanged(path, interface, propertyName, value);
}

QDBusMessage DBusService::call(const QString &destination,
                                const QString &path,
                                const QString &interface,
                                const QString &method,
                                const QVariantList &args)
{
    if (!d->connection || !d->connection->isConnected())
        return QDBusMessage();

    QDBusMessage msg = QDBusMessage::createMethodCall(
        destination, path, interface, method);
    msg.setArguments(args);

    return d->connection->connection().call(msg);
}

void DBusService::callAsync(const QString &destination,
                             const QString &path,
                             const QString &interface,
                             const QString &method,
                             const QVariantList &args)
{
    if (!d->connection || !d->connection->isConnected())
        return;

    QDBusMessage msg = QDBusMessage::createMethodCall(
        destination, path, interface, method);
    msg.setArguments(args);

    d->connection->connection().asyncCall(msg);
}

} // namespace Lingmo
