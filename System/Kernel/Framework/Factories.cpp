//-------------------------------------------------------------------------------
// Factories.cpp
//
// @author
//     Millhaus.Chen @time 2016/08/31 16:26
//-------------------------------------------------------------------------------
#include "Factories.h"
#include "../util/IFactory.h"

using namespace S2::Kernel;

//IMPLEMENT_SINGLE_INSTANCE(Factories);
Factories::Factories(void)
{

}
Factories::~Factories(void)
{

}

void Factories::addModuleFactory(ModuleFactory* pFactory)
{
    ModuleFactoryMap::iterator it = m_moduleFactoryMap.find(pFactory->getTypeName());
    if(it == m_moduleFactoryMap.end())
    {
        m_moduleFactoryMap.insert(std::make_pair(pFactory->getTypeName(), pFactory));
    }
    else
    {
        throw NULL;
    }
}

void Factories::removeModuleFactory(ModuleFactory* pFactory)
{
    m_moduleFactoryMap.erase(pFactory->getTypeName());
}

ModulePtr Factories::createModule(const std::string& typeName, const std::string& name)
{
    ModuleFactoryMap::iterator it = m_moduleFactoryMap.find(typeName);
    if(it == m_moduleFactoryMap.end())
    {
        return nullptr;
    }

    return it->second->createInstance(name);
}

ModulePtr Factories::getModule(const std::string& typeName, const std::string& name)
{
    ModuleFactoryMap::iterator it = m_moduleFactoryMap.find(typeName);
    if(it == m_moduleFactoryMap.end())
    {
        return nullptr;
    }

    return it->second->getInstance(name);
}

void Factories::addShellFactory(ShellFactory* pFactory)
{
    ShellFactoryMap::iterator it = m_shellFactoryMap.find(pFactory->getTypeName());
    if(it == m_shellFactoryMap.end())
    {
        m_shellFactoryMap.insert(std::make_pair(pFactory->getTypeName(), pFactory));
    }
    else
    {
        throw NULL;
    }
}

void Factories::removeShellFactory(ShellFactory* pFactory)
{
    m_shellFactoryMap.erase(pFactory->getTypeName());
}

ShellPtr Factories::createShell(const std::string& typeName, const std::string& prompt)
{
    ShellFactoryMap::iterator it = m_shellFactoryMap.find(typeName);
    if(it == m_shellFactoryMap.end())
    {
        throw std::string("Shell " + typeName + "not loaded !");
    }

    std::string promptCopy = prompt;
    if(promptCopy.size() > 0)
    {
        promptCopy.append("/");
    }
    promptCopy.append(typeName);

    return it->second->createInstance(promptCopy);
}

ShellPtr Factories::getShell(const std::string& typeName)
{
    ShellFactoryMap::iterator it = m_shellFactoryMap.find(typeName);
    if(it == m_shellFactoryMap.end())
    {
        throw std::string("Shell " + typeName + "not loaded !");
    }

    return it->second->getInstance();
}