ADD_DEFINITIONS(-DDLL_EXPORTS)

INCLUDE_DIRECTORIES(../../third/libgo/)

IF(UNIX)
    INCLUDE_DIRECTORIES("include" ${Saturn_DependenciesInclude} ../../third/libgo/linux)
ELSE()
    INCLUDE_DIRECTORIES("include" ${Saturn_DependenciesInclude} ../../third/libgo/windows)
ENDIF()

add_library(Net STATIC NetClient.cpp NetServer.cpp NetSystem.cpp)

TARGET_LINK_LIBRARIES(Net Kernel)

#FIND_LIBRARY(libgopath libgo ../../third/libgo/lib)
#IF(libgopath)
#    MESSAGE("Founded ${libgopath}")
#    TARGET_LINK_LIBRARIES (Net ${libgopath})
#ENDIF(libgopath)