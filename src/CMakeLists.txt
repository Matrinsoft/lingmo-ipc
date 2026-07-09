set(PUBLIC_HEADERS
    ${CMAKE_SOURCE_DIR}/include/LingmoIPC/DBusConnection.h
    ${CMAKE_SOURCE_DIR}/include/LingmoIPC/DBusService.h
    ${CMAKE_SOURCE_DIR}/include/LingmoIPC/ServiceManager.h
)

set(PRIVATE_HEADERS
    LingmoIPC/private/DBusConnection_p.h
    LingmoIPC/private/DBusService_p.h
    LingmoIPC/private/ServiceManager_p.h
)

set(SOURCES
    LingmoIPC/DBusConnection.cpp
    LingmoIPC/DBusService.cpp
    LingmoIPC/ServiceManager.cpp
)

set(CMAKE_AUTOMOC ON)

add_library(LingmoIPC SHARED ${SOURCES} ${PUBLIC_HEADERS} ${PRIVATE_HEADERS})
add_library(Lingmo::IPC ALIAS LingmoIPC)

target_include_directories(LingmoIPC
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(LingmoIPC
    PUBLIC
        Qt6::Core
        Qt6::DBus
)

target_compile_definitions(LingmoIPC
    PRIVATE
        LINGMOIPC_LIBRARY
)

set_target_properties(LingmoIPC PROPERTIES
    SOVERSION 1
    VERSION ${PROJECT_VERSION}
    DEFINE_SYMBOL LINGMOIPC_LIBRARY
    OUTPUT_NAME LingmoIPC
)

configure_file(
    "${CMAKE_SOURCE_DIR}/include/LingmoIPC/LingmoIPCExport.h.in"
    "${CMAKE_BINARY_DIR}/include/LingmoIPC/LingmoIPCExport.h"
)

configure_file(
    "${CMAKE_SOURCE_DIR}/include/LingmoIPC/LingmoIPCVersion.h.in"
    "${CMAKE_BINARY_DIR}/include/LingmoIPC/LingmoIPCVersion.h"
)

install(TARGETS LingmoIPC
    EXPORT LingmoIPCTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/LingmoIPC/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/LingmoIPC
    FILES_MATCHING PATTERN "*.h"
    PATTERN "*.h.in" EXCLUDE
)

install(FILES
    "${CMAKE_BINARY_DIR}/include/LingmoIPC/LingmoIPCExport.h"
    "${CMAKE_BINARY_DIR}/include/LingmoIPC/LingmoIPCVersion.h"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/LingmoIPC
)

install(EXPORT LingmoIPCTargets
    FILE LingmoIPCTargets.cmake
    NAMESPACE Lingmo::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LingmoIPC
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
    "${CMAKE_SOURCE_DIR}/cmake/LingmoIPCConfig.cmake.in"
    "${CMAKE_BINARY_DIR}/LingmoIPCConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LingmoIPC
    PATH_VARS CMAKE_INSTALL_INCLUDEDIR CMAKE_INSTALL_LIBDIR
)

write_basic_package_version_file(
    "${CMAKE_BINARY_DIR}/LingmoIPCConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    "${CMAKE_BINARY_DIR}/LingmoIPCConfig.cmake"
    "${CMAKE_BINARY_DIR}/LingmoIPCConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LingmoIPC
)
