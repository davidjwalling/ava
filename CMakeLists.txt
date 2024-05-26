cmake_minimum_required(VERSION 3.10)

project(ava LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 11)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    include(GNUInstallDirs)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    add_compile_options(
        -fvisibility=hidden
        -Wno-deprecated-declarations
    )
    find_package(Threads REQUIRED)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_definitions(
        -D_WIN32_WINNT=0x0601
        -DNOMINMAX
        -DVC_EXTRALEAN
        -DWIN32_LEAN_AND_MEAN
        -D_CRT_SECURE_NO_WARNINGS
        -D_SCL_SECURE_NO_WARNINGS
        -D_WINSOCK_DEPRECATED_NO_WARNINGS
    )
endif()

#
#   libava
#

set (LIB_SRCS
    api.cpp
    driver.cpp
)

set (PUB_HDRS
    iapi.h
    idriver.h
)

add_library(libava SHARED ${LIB_SRCS})

set_target_properties(libava PROPERTIES PREFIX "")
set_target_properties(libava PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
set_target_properties(libava PROPERTIES PUBLIC_HEADER "${PUB_HDRS}")

target_include_directories(libava INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/libava>
)

install(TARGETS libava EXPORT libava
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/libava
    RUNTIME       DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY       DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE       DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(EXPORT libava
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libava
    FILE libava.cmake
)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_link_libraries(libava PRIVATE Threads::Threads)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES MSVC)
    target_link_libraries(libava wsock32 ws2_32)
    target_compile_definitions(libava PUBLIC _WINDLL)
endif()
 
#
#   testava
#

set (TEST_SRCS
    testava.cpp
)

add_executable(testava ${TEST_SRCS})

target_link_libraries(testava libava)

install(TARGETS testava
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

#
#   ava
#

set (AVA_SRCS
    ava.cpp
)

add_executable(ava ${AVA_SRCS})

target_link_libraries(ava libava)

install (TARGETS ava
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
