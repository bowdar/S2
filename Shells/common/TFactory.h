//-------------------------------------------------------------------------------
// TFactory.h
//     factory template, public code for every factory
//
// @author
//     Millhaus.Chen @time 2016/09/21 14:20
//-------------------------------------------------------------------------------
#pragma once

#include "../../System/Kernel/util/IFactory.h"
#include "../../System/Kernel/Framework/Factories.h"
#include "../../System/Kernel/SystemKernelDeclare.h"

#include <algorithm>
#include <string>
#include <memory>

template<class ShellType>
class TShellFactory: public S2::Kernel::IFactory<S2::Kernel::Shell>
{
public:
    virtual const std::string& getTypeName() override;
    virtual const std::string& getFatherName() override
    {
        static const std::string fatherName = "";
        return fatherName;
    }
    ProductPtr createInstance(const std::string& instanceName) override
    {
        if(m_pShell)
        {
            return m_pShell;
        }
        else
        {
            m_pShell = typename ShellType::Ptr(new ShellType(instanceName));
        }
        return m_pShell;
    }
    ProductPtr getInstance(const std::string& instanceName) override
    {
        return createInstance(instanceName);
    }
    void deleteInstance(const std::string& instanceName) override {}

protected:
    typename ShellType::Ptr m_pShell = nullptr;
};

template<class ModuleType>
class TModuleFactory: public S2::Kernel::IFactory<S2::Kernel::Module>
{
    typedef std::map<std::string, typename ModuleType::Ptr> ClientMap;

public:
    const std::string & getTypeName() override;
    ProductPtr createInstance(const std::string& instanceName) override
    {
        typename ModuleType::Ptr ptr = typename ModuleType::Ptr(new ModuleType(instanceName));
        m_mapClient.insert({instanceName, ptr});
        return std::static_pointer_cast<S2::Kernel::Module>(ptr);
    }
    ProductPtr getInstance(const std::string& instanceName) override
    {
        auto iter = m_mapClient.find(instanceName);
        if(iter != m_mapClient.end())
        {
            return iter->second;
        }
        return nullptr;
    }
    void deleteInstance(const std::string& instanceName) override
    {
        m_mapClient.erase(instanceName);
    }

private:
    ClientMap m_mapClient;
};
