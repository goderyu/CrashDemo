cmake_minimum_required(VERSION 3.20)

project(
    CrashDemo
    VERSION 1.0
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(OS_WIN ON)
    set(OS_LINUX OFF)
    add_definitions(-DOS_WIN=1)
    add_definitions(-DOS_LINUX=0)
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(OS_WIN OFF)
    set(OS_LINUX ON)
    add_definitions(-DOS_WIN=0)
    add_definitions(-DOS_LINUX=1)
else()

endif()


if (OS_LINUX)
    message(STATUS "Current os is Linux")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g -Wl,--build-id")

    add_executable(
        ${PROJECT_NAME}
        main.cpp
    )

    target_include_directories(
        ${PROJECT_NAME}
        PRIVATE ThirdParty/Crashpad/include   
        PRIVATE ThirdParty/Crashpad/include/third_party/mini_chromium
    )

    target_link_libraries(
        ${PROJECT_NAME}
        ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/Crashpad/lib/Linux/libcommon.a
        ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/Crashpad/lib/Linux/libclient.a
        ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/Crashpad/lib/Linux/libutil.a
        ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/Crashpad/lib/Linux/libbase.a
    )
endif()