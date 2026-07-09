%global majver 1

Name:          lingmo-ipc
Version:       1.0.0
Release:       1%{?dist}
Summary:       Lingmo Desktop - IPC and D-Bus integration

License:       GPL-2.0-or-later
URL:           https://lingmo.org
Source0:       %{name}-%{version}.tar.gz

BuildRequires: cmake >= 3.21
BuildRequires: ninja-build
BuildRequires: qt6-qtbase-devel >= 6.5
BuildRequires: qt6-qtbase-private-devel >= 6.5

%description
Lingmo IPC provides D-Bus connection management, service registration,
and systemd integration for Lingmo Desktop components.

%package libs
Summary: Shared library for %{name}

%description libs
Shared library providing D-Bus and systemd integration.

%package devel
Summary: Development files for %{name}
Requires: %{name}-libs%{?_isa} = %{version}-%{release}
Requires: qt6-qtbase-devel

%description devel
Development headers and CMake integration for Lingmo IPC.

%prep
%setup -q

%build
%cmake -G Ninja \
    -DCMAKE_BUILD_TYPE=RelWithDebInfo \
    -DBUILD_TESTING=OFF
%cmake_build

%install
%cmake_install

%check
# Tests are run in CI

%files libs
%license LICENSE
%{_libdir}/libLingmoIPC.so.%{majver}*

%files devel
%{_includedir}/LingmoIPC/
%{_libdir}/libLingmoIPC.so
%{_libdir}/cmake/LingmoIPC/

%changelog
* Wed Jul 09 2026 Lingmo OS Team <team@lingmo.org> - 1.0.0-1
- Initial release
