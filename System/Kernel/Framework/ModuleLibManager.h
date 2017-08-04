//-------------------------------------------------------------------------------
// ModuleLibManager.h
//
// @author
//    Millhaus.Chen @time 2016/08/31 11:29
//-------------------------------------------------------------------------------
#pragma once

#include "../util/SingletonMacro.h"
#include "../util/DllLoader.h"

#include <vector>

namespace S2 { namespace Kernel {

class ModuleLibManager
{
    typedef std::vector<DllLoader::Ptr> ModuleFactoryList;

public:
    ModuleLibManager(void);
    virtual ~ModuleLibManager(void);
    DECLARE_SINGLE_INSTANCE(ModuleLibManager);

public:
    bool init(void);

    void shutdown(void);

public:
    void clear(void);

    void loadModuleLibFromFile(const std::string & strFileName);
    void loadModuleLib(const std::string & strModuleName);
    void unloadModuleLib(const std::string & strModuleName);

private:

    ModuleFactoryList m_vecModuleLibs;
    bool _isInitialised;
};



}}
