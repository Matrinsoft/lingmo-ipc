#pragma once

#include <LingmoIPC/DBusConnection.h>
#include <QDBusConnection>
#include <optional>

namespace Lingmo {

class DBusConnectionPrivate
{
public:
    DBusConnection::BusType busType;
    std::optional<QDBusConnection> conn;
    bool connected = false;
};

} // namespace Lingmo
