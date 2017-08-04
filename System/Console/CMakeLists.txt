CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0 FATAL_ERROR)

add_executable(Console main.cpp View/RootShell.cpp)

TARGET_LINK_LIBRARIES(Console Kernel)

IF(UNIX)
    TARGET_LINK_LIBRARIES(Console dl)
ENDIF()
