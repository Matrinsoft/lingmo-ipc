#include <LingmoIPC/ServiceManager.h>

#include <QTest>

class tst_ServiceManager : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testInstance()
    {
        auto *mgr = Lingmo::ServiceManager::instance();
        QVERIFY(mgr != nullptr);
        QCOMPARE(Lingmo::ServiceManager::instance(), mgr);
    }

    void testSetReady()
    {
        auto *mgr = Lingmo::ServiceManager::instance();
        // setReady may fail if not running under systemd, but should not crash
        mgr->setReady();
    }

    void testSetStatus()
    {
        auto *mgr = Lingmo::ServiceManager::instance();
        mgr->setStatus(QStringLiteral("Processing requests"));
    }

    void testWatchdogToggle()
    {
        auto *mgr = Lingmo::ServiceManager::instance();
        mgr->setWatchdogEnabled(true);
        QVERIFY(mgr->isWatchdogEnabled());
        mgr->setWatchdogEnabled(false);
        QVERIFY(!mgr->isWatchdogEnabled());
    }

    void testIsSystemdManaged()
    {
        // Just check it doesn't crash
        const bool managed = Lingmo::ServiceManager::isSystemdManaged();
        Q_UNUSED(managed);
    }
};

QTEST_MAIN(tst_ServiceManager)
#include "tst_ServiceManager.moc"
