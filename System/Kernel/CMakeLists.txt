ADD_DEFINITIONS(-DDLL_EXPORTS)

add_library(Kernel STATIC SystemKernelDeclare.h
                          Kernel.h
                          Kernel.cpp
                          Task.hpp
                          util/log.h
                          util/Singleton.h
                          util/SingletonMacro.h
                          util/SystemTools.h
                          util/IFactory.h
                          util/CmdControl.h
                          util/DllExport.h
                          util/DllLoader.h
                          util/DllLoader.cpp
                          Model/Content.h
                          Model/Module.h
                          Model/Module.cpp
                          Model/Shell.h
                          Model/Shell.cpp
                          Framework/Factories.h
                          Framework/Factories.cpp
                          Framework/ModuleLibManager.h
                          Framework/ModuleLibManager.cpp
                          Framework/SystemBuilder.h
                          Framework/SystemBuilder.cpp)
