#pragma once

#include <LingmoIPC/LingmoIPCExport.h>

#include <QObject>
#include <QString>
#include <QVariantMap>
#include <QDBusMessage>
#include <memory>

namespace Lingmo {

class DBusConnection;
class DBusServicePrivate;

// D-Bus service helper for registering and handling D-Bus interfaces.
//
// Provides a high-level API for:
// - Implementing D-Bus methods (request handlers)
// - Emitting D-Bus signals
// - Managing D-Bus properties (get/set/notify)
// - Introspection support
//
// Usage:
//   auto *service = new Lingmo::DBusService("org.lingmo.Test", this);
//   service->setConnection(Lingmo::DBusConnection::sessionBus());
//   service->registerInterface("/org/lingmo/Test", "org.lingmo.Test.Interface");
//   connect(service, &DBusService::methodCalled, this, &MyClass::handleMethod);
class LINGMOIPC_EXPORT DBusService : public QObject
{
    Q_OBJECT

public:
    explicit DBusService(const QString &serviceName, QObject *parent = nullptr);
    ~DBusService() override;

    QString serviceName() const;

    void setConnection(DBusConnection *connection);
    DBusConnection *connection() const;

    // Register a D-Bus interface at a path
    bool registerInterface(const QString &path, const QString &interface);

    // Unregister a D-Bus interface
    bool unregisterInterface(const QString &path, const QString &interface);

    // Emit a D-Bus signal
    void emitSignal(const QString &path, const QString &interface,
                    const QString &signalName, const QVariantList &args = {});

    // Set a D-Bus property value (emits PropertiesChanged)
    void setProperty(const QString &path, const QString &interface,
                     const QString &propertyName, const QVariant &value);

    // Send a D-Bus method call
    QDBusMessage call(const QString &destination,
                      const QString &path,
                      const QString &interface,
                      const QString &method,
                      const QVariantList &args = {});

    // Send an async D-Bus method call
    void callAsync(const QString &destination,
                   const QString &path,
                   const QString &interface,
                   const QString &method,
                   const QVariantList &args = {});

Q_SIGNALS:
    void methodCalled(const QString &path, const QString &interface,
                      const QString &method, const QVariantList &args);
    void propertyChanged(const QString &path, const QString &interface,
                         const QString &propertyName, const QVariant &value);

private:
    std::unique_ptr<DBusServicePrivate> d;
};

} // namespace Lingmo
