#pragma once

#include <LingmoIPC/LingmoIPCExport.h>

#include <QObject>
#include <QString>
#include <QDBusConnection>
#include <memory>

namespace Lingmo {

class DBusConnectionPrivate;

// Manages D-Bus connections for Lingmo Desktop components.
//
// Provides a centralized way to obtain and manage D-Bus connections:
// - Session bus (for desktop services)
// - System bus (for system services like UPower, NetworkManager)
// - Custom named connections
//
// Usage:
//   auto *conn = Lingmo::DBusConnection::sessionBus();
//   conn->registerService("org.lingmo.Test");
class LINGMOIPC_EXPORT DBusConnection : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isConnected READ isConnected NOTIFY connectionChanged)

public:
    enum BusType {
        SessionBus,
        SystemBus
    };
    Q_ENUM(BusType)

    explicit DBusConnection(BusType type, QObject *parent = nullptr);
    ~DBusConnection() override;

    // Get the underlying Qt D-Bus connection
    QDBusConnection connection() const;

    bool isConnected() const;

    // Register a well-known service name
    bool registerService(const QString &serviceName);

    // Register an object at a path
    bool registerObject(const QString &path, QObject *object,
                        const QString &interface = {});

    // Unregister a service
    bool unregisterService(const QString &serviceName);

    // Convenience: get session/system bus singleton
    static DBusConnection *sessionBus(QObject *parent = nullptr);
    static DBusConnection *systemBus(QObject *parent = nullptr);

Q_SIGNALS:
    void connectionChanged(bool connected);
    void serviceRegistered(const QString &serviceName);
    void serviceUnregistered(const QString &serviceName);

private:
    std::unique_ptr<DBusConnectionPrivate> d;
};

} // namespace Lingmo
