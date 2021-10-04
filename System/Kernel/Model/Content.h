//-------------------------------------------------------------------------------
// Content.h
//
// @brief
//     ��¼Kernel���صĸ������͡���̬���Ϳ������
//
// @author
//     Millhaus.Chen     @time 2016/08/31 11:28
//-------------------------------------------------------------------------------
#pragma once

#include "../Framework/ModuleLibManager.h"
#include "Module.h"

#include <string>

namespace S2 { namespace Kernel {

class Content
{
public:
    typedef std::shared_ptr<Content> Ptr;
    typedef std::vector<std::string> ModuleLibNamesList;
    typedef std::vector<Module::Ptr> ModuleList;

public:
    Content(void) {}
    virtual ~Content(void) {}

    void addRunByKernel(Module::Ptr pModule)
    {
        m_runByKernelList.push_back(pModule);
    }
    void addModuleLibName(const std::string& name)
    {
        m_moduleLibNames.push_back(name);
    }
    void loadModuleLib(void)
    {
        for(ModuleLibNamesList::iterator it = m_moduleLibNames.begin(); it != m_moduleLibNames.end(); ++it)
        {
            ModuleLibManager::GetInstance().loadModuleLib(*it);
        }
    }
    void unloadModuleLib(void)
    {
        for(ModuleLibNamesList::iterator it = m_moduleLibNames.begin(); it != m_moduleLibNames.end(); ++it)
        {
            ModuleLibManager::GetInstance().unloadModuleLib(*it);
        }
    }

    const ModuleLibNamesList& getModuleLibNamesList() const { return m_moduleLibNames; }
    const ModuleList& getModuleList() const { return m_runByKernelList; }

private:
    ModuleLibNamesList m_moduleLibNames;
    ModuleLibManager m_moduleLibManager;
    ModuleList m_runByKernelList;
};

}}
