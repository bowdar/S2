ADD_DEFINITIONS(-DDLL_EXPORTS)

add_library(Kernel STATIC IKernel.cpp util/DllLoader.cpp Model/Module.cpp Model/Shell.cpp Framework/Factories.cpp Framework/ModuleLibManager.cpp Framework/SystemBuilder.cpp )
