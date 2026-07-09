#pragma once

#include <LingmoIPC/LingmoIPCExport.h>

#include <QObject>
#include <QString>
#include <memory>

namespace Lingmo {

class ServiceManagerPrivate;

// Systemd service lifecycle management.
//
// Provides integration with systemd for:
// - Readiness notification (sd_notify READY=1)
// - Watchdog keep-alive (sd_notify WATCHDOG=1)
// - Status updates (sd_notify STATUS=...)
// - Service restart on failure
//
// Usage:
//   auto *mgr = Lingmo::ServiceManager::instance();
//   mgr->setReady();
//   // ... later ...
//   mgr->setWatchdog();  // call periodically
class LINGMOIPC_EXPORT ServiceManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool watchdogEnabled READ isWatchdogEnabled WRITE setWatchdogEnabled)

public:
    explicit ServiceManager(QObject *parent = nullptr);
    ~ServiceManager() override;

    static ServiceManager *instance();

    // Signal that the service is ready
    bool setReady();

    // Send a watchdog keep-alive notification
    bool setWatchdog();

    // Update status message
    bool setStatus(const QString &status);

    // Set the main PID (for systemd to track)
    bool setMainPid(qint64 pid = -1);

    // Send a stopping notification
    bool setStopping();

    // Check if we're running under systemd
    static bool isSystemdManaged();

    bool isWatchdogEnabled() const;
    void setWatchdogEnabled(bool enabled);

Q_SIGNALS:
    void watchdogExpired();

private:
    std::unique_ptr<ServiceManagerPrivate> d;
};

} // namespace Lingmo
