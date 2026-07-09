#include <LingmoIPC/DBusConnection.h>

#include <QTest>
#include <QSignalSpy>
#include <QDBusConnection>

class tst_DBusConnection : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testSessionBusCreation()
    {
        Lingmo::DBusConnection conn(Lingmo::DBusConnection::SessionBus);
        // May or may not connect depending on environment
        Q_UNUSED(conn);
    }

    void testSessionBusSingleton()
    {
        auto *bus1 = Lingmo::DBusConnection::sessionBus();
        auto *bus2 = Lingmo::DBusConnection::sessionBus();
        QVERIFY(bus1 != nullptr);
        QCOMPARE(bus1, bus2);
    }

    void testSystemBusSingleton()
    {
        auto *bus1 = Lingmo::DBusConnection::systemBus();
        auto *bus2 = Lingmo::DBusConnection::systemBus();
        QVERIFY(bus1 != nullptr);
        QCOMPARE(bus1, bus2);
    }

    void testRegisterService()
    {
        Lingmo::DBusConnection conn(Lingmo::DBusConnection::SessionBus);
        if (!conn.isConnected()) {
            QSKIP("No D-Bus session available");
        }

        // Register a unique service name
        const QString serviceName = QStringLiteral("org.lingmo.test.%1")
            .arg(reinterpret_cast<quintptr>(this), 0, 16);
        const bool result = conn.registerService(serviceName);
        // May fail if name is already taken, but should not crash
        Q_UNUSED(result);
    }

    void testConnectionProperty()
    {
        Lingmo::DBusConnection conn(Lingmo::DBusConnection::SessionBus);
        // isConnected should return a consistent value
        QCOMPARE(conn.isConnected(), conn.connection().isConnected());
    }
};

QTEST_MAIN(tst_DBusConnection)
#include "tst_DBusConnection.moc"
