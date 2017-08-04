//-------------------------------------------------------------------------------
// ModuleLibManager.cpp
//
// @author
//     Millhaus.Chen @time 2016/08/31 16:29
//-------------------------------------------------------------------------------
#include "../SystemKernelDeclare.h"
#include "ModuleLibManager.h"

#include <fstream>

typedef void (*DLL_REGISTER_SERVER_FACTORY)(void);
typedef void (*DLL_UNREGISTER_SERVER_FACTORY)(void);

using namespace S2::Kernel;

IMPLEMENT_SINGLE_INSTANCE(ModuleLibManager);
ModuleLibManager::ModuleLibManager(void)
{

}
ModuleLibManager::~ModuleLibManager(void)
{

}

bool ModuleLibManager::init(void)
{
    ModuleFactoryList::iterator i;
    for(i = m_vecModuleLibs.begin(); i != m_vecModuleLibs.end(); ++i)
    {
        DLL_REGISTER_SERVER_FACTORY pFunc = (DLL_REGISTER_SERVER_FACTORY)(*i)->getSymbol("dllRegisterModuleFactory");
        if(pFunc)
        {
            pFunc();
        }

    }
    _isInitialised = true;

    return true;
}

void ModuleLibManager::clear()
{
    ModuleFactoryList::reverse_iterator i;

    for(i = m_vecModuleLibs.rbegin(); i != m_vecModuleLibs.rend(); ++i)
    {
        DLL_UNREGISTER_SERVER_FACTORY pFunc = (DLL_UNREGISTER_SERVER_FACTORY)(*i)->getSymbol("dllUnRegisterModuleFactory");
        if(!pFunc) return;
        pFunc();

        (*i)->unload();
    }

    m_vecModuleLibs.clear();
}

void ModuleLibManager::loadModuleLibFromFile(const std::string & strFileName)
{
    std::ifstream file;
    file.open(strFileName.c_str());
    if(!file.is_open())
    {
        throw NULL;
    }
    std::string strModuleFactoryName;
    while(!file.eof())
    {
        file >> strModuleFactoryName;
        loadModuleLib(strModuleFactoryName);
    }
    file.close();
}

void ModuleLibManager::loadModuleLib(const std::string& strModuleName)
{
    DllLoader::Ptr lib(new DllLoader(strModuleName));
    lib->load();

    m_vecModuleLibs.push_back(lib);    

    DLL_REGISTER_SERVER_FACTORY pFunc = (DLL_REGISTER_SERVER_FACTORY)lib->getSymbol("dllRegisterModuleFactory");

    if(!pFunc) return;

    pFunc();
}


void ModuleLibManager::unloadModuleLib(const std::string & strModuleName)
{
    ModuleFactoryList::iterator i;

    for (i = m_vecModuleLibs.begin(); i != m_vecModuleLibs.end(); ++i)
    {
        if ((*i)->getName() == strModuleName)
        {
            DLL_UNREGISTER_SERVER_FACTORY pFunc = (DLL_UNREGISTER_SERVER_FACTORY)(*i)->getSymbol("dllUnRegisterModuleFactory");

            if(!pFunc) return;
            
            pFunc();
            (*i)->unload();

            m_vecModuleLibs.erase(i);
            return;
        }

    }
}
