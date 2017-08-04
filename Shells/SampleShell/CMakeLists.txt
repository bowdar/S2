CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0 FATAL_ERROR)

INCLUDE_DIRECTORIES(../../)
INCLUDE_DIRECTORIES(../../third/)

ADD_LIBRARY(HistoryAnalyze SHARED
        SampleShell.cpp
        ../common/BaseShell.cpp
        )

IF(UNIX)
    SET(BOOST_ROOT "/usr/local")
ENDIF()
set(Boost_USE_STATIC_LIBS        OFF) # only find static libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost 1.61.0 COMPONENTS system filesystem)
IF(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(HistoryAnalyze ${Boost_LIBRARIES})
ENDIF()