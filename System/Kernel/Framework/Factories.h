//-------------------------------------------------------------------------------
// Factories.h
//
// @brief
//     This class store all of the abstract factories
//
// @author
//     Millhaus.Chen @time 2016/09/06 15:37
//-------------------------------------------------------------------------------
#pragma once

#include "../util/SingletonMacro.h"
#include "../SystemKernelDeclare.h"
#include "../util/DllExport.h"
#include "../util/Singleton.h"

#include <string>
#include <map>
#include <memory>
#include <boost/mpl/inherit_linearly.hpp>

namespace S2{ namespace Kernel {

class _DLL_EXPORTS Factories : public mtl::Singleton<Factories> //private boost::noncopyable
{
public:
    typedef std::map<std::string, ModuleFactory*> ModuleFactoryMap;
    typedef std::map<std::string, ShellFactory*> ShellFactoryMap;

public:
    Factories(void);
    virtual ~Factories(void);
    //DECLARE_SINGLE_INSTANCE(Factories);

    void addModuleFactory(ModuleFactory* factory);
    void removeModuleFactory(ModuleFactory* factory);
    void addShellFactory(ShellFactory* factory);
    void removeShellFactory(ShellFactory* factory);

    /// @brief Overloading these two functions, use to add a different type of the factory
    inline void addFactory(ModuleFactory* factory){ addModuleFactory(factory); }
    inline void removeFactory(ModuleFactory* factory){ removeModuleFactory(factory); }
    inline void addFactory(ShellFactory* factory){ addShellFactory(factory); }
    inline void removeFactory(ShellFactory* factory){ removeShellFactory(factory); }

    ModulePtr createModule(const std::string& typeName, const std::string& name);
    ModulePtr getModule(const std::string& typeName, const std::string& name);

    ShellPtr createShell(const std::string& typeName, const std::string& prompt = "");
    ShellPtr getShell(const std::string& typeName);

    const ModuleFactoryMap& getModuleFactoryMap(){ return m_moduleFactoryMap; }
    const ShellFactoryMap& getShellFactoryMap(){ return m_shellFactoryMap; }

private:
    ModuleFactoryMap m_moduleFactoryMap;
    ShellFactoryMap m_shellFactoryMap;
};

template<class T>
inline void AddFactory(T* factory)
{
    Factories::GetInstance().addFactory(factory);
}

template<class T>
inline void RemoveFactory(T* factory)
{
    Factories::GetInstance().removeFactory(factory);
}

class FactoryRegister
{
public:
    typedef std::shared_ptr<FactoryRegister> Ptr;
    
    FactoryRegister(void)
    {}
    virtual ~FactoryRegister(void)
    {}
};

template<class Base, class T>
class ModeFactory: public Base
{
public:
    ModeFactory(void)
    {
        AddFactory(&m_factory);
    }
    virtual ~ModeFactory(void)
    {
        RemoveFactory(&m_factory);
    }
private:
    T m_factory;
};

template<class TList>
class FactoryRegisterList :public boost::mpl::inherit_linearly<TList, ModeFactory<boost::mpl::placeholders::_, boost::mpl::placeholders::_>, FactoryRegister>::type {};


}}
