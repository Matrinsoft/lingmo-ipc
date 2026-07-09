set(CMAKE_AUTOMOC ON)
find_package(Qt6 REQUIRED COMPONENTS Test)

function(add_lingmo_ipc_test name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name}
        PRIVATE
            Lingmo::IPC
            Qt6::Test
    )
    add_test(NAME ${name} COMMAND ${name})
endfunction()

add_lingmo_ipc_test(tst_DBusConnection)
add_lingmo_ipc_test(tst_DBusService)
add_lingmo_ipc_test(tst_ServiceManager)
