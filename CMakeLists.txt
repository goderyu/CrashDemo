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

if (OS_WIN)
endif()

add_executable(
    ${PROJECT_NAME}
    main.cpp
)

target_compile_options(
    ${PROJECT_NAME}
    PUBLIC "/Zi"
    PUBLIC "/MT"
)

target_link_options(
    ${PROJECT_NAME}
    PUBLIC "/DEBUG"
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE ThirdParty/Crashpad/include   
    PRIVATE ThirdParty/Crashpad/include/third_party/mini_chromium
)

if (OS_LINUX)
    message(STATUS "Current os is Linux")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g -Wl,--build-id")


    target_link_libraries(
        ${PROJECT_NAME}
        ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/Crashpad/lib/Linux/libcommon.a
        ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/Crashpad/lib/Linux/libclient.a
        ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/Crashpad/lib/Linux/libutil.a
        ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/Crashpad/lib/Linux/libbase.a
    )
elseif(OS_WIN)
    message(STATUS "Current os is Windows")
    # set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
    # set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
    # set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /NODEFAULTLIB:LIBCMT")
    target_link_libraries(
        ${PROJECT_NAME}
        ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/Crashpad/lib/Windows/common.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/Crashpad/lib/Windows/client.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/Crashpad/lib/Windows/util.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/Crashpad/lib/Windows/base.lib
    )
endif()