#include <LingmoIPC/DBusService.h>
#include <LingmoIPC/DBusConnection.h>

#include <QTest>
#include <QSignalSpy>

class tst_DBusService : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testCreateService()
    {
        Lingmo::DBusService service(QStringLiteral("org.lingmo.test"));
        QCOMPARE(service.serviceName(), QStringLiteral("org.lingmo.test"));
    }

    void testSetConnection()
    {
        Lingmo::DBusService service(QStringLiteral("org.lingmo.test"));
        Lingmo::DBusConnection conn(Lingmo::DBusConnection::SessionBus);

        service.setConnection(&conn);
        QCOMPARE(service.connection(), &conn);
    }

    void testRegisterInterface()
    {
        Lingmo::DBusService service(QStringLiteral("org.lingmo.test"));
        Lingmo::DBusConnection conn(Lingmo::DBusConnection::SessionBus);

        if (!conn.isConnected()) {
            QSKIP("No D-Bus session available");
        }

        service.setConnection(&conn);
        // May fail if service name is taken, but should not crash
        service.registerInterface(QStringLiteral("/org/lingmo/test"),
                                 QStringLiteral("org.lingmo.test.Interface"));
    }

    void testCallWithoutConnection()
    {
        Lingmo::DBusService service(QStringLiteral("org.lingmo.test"));
        // Should return empty message, not crash
        auto msg = service.call(QStringLiteral("org.lingmo.test"),
                                QStringLiteral("/org/lingmo/test"),
                                QStringLiteral("org.lingmo.test.Interface"),
                                QStringLiteral("TestMethod"));
        QVERIFY(!msg.type() == QDBusMessage::ReplyMessage);
    }
};

QTEST_MAIN(tst_DBusService)
#include "tst_DBusService.moc"
