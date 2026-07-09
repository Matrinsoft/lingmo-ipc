#pragma once

#include <LingmoIPC/ServiceManager.h>

#include <QTimer>

namespace Lingmo {

class ServiceManagerPrivate
{
public:
    bool watchdogEnabled = false;
    QTimer *watchdogTimer = nullptr;
    qint64 watchdogIntervalMs = 0;
};

} // namespace Lingmo
