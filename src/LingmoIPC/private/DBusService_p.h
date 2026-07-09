#pragma once

#include <LingmoIPC/DBusService.h>
#include <LingmoIPC/DBusConnection.h>

#include <QHash>
#include <QString>

namespace Lingmo {

struct InterfaceInfo {
    QString path;
    QString interface;
};

class DBusServicePrivate
{
public:
    QString serviceName;
    DBusConnection *connection = nullptr;
    QHash<QString, InterfaceInfo> registeredInterfaces;
};

} // namespace Lingmo
