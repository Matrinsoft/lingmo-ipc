# lingmo-ipc

Lingmo Desktop - IPC and D-Bus integration library.

## Overview

`lingmo-ipc` provides D-Bus connection management, service registration,
and systemd integration for Lingmo Desktop components:

- **DBusConnection**: Session/system bus connection management
- **DBusService**: D-Bus method handling, signal emission, property management
- **ServiceManager**: systemd integration (sd_notify, watchdog, status)

## Dependencies

- Qt 6 (Core, DBus)

## Building

```bash
cmake -B build -DCMAKE_INSTALL_PREFIX=/usr
cmake --build build
ctest --test-dir build
```

## License

GPL-2.0-or-later
