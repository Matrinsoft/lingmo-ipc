#pragma once

#include <LingmoIPC/DBusConnection.h>
#include <QDBusConnection>

namespace Lingmo {

class DBusConnectionPrivate
{
public:
    DBusConnection::BusType busType;
    QDBusConnection conn;
    bool connected = false;
};

} // namespace Lingmo
