//-------------------------------------------------------------------------------
// IFactory.h
//
// @author
//     Millhaus.Chen @time 2016/08/31 16:29
//-------------------------------------------------------------------------------
#pragma once

#include <memory>
#include <boost/noncopyable.hpp>
#include <string>
#include <vector>

namespace S2 { namespace Kernel {

class Shell;

template <class T>
class IFactory: private boost::noncopyable
{
public:
    typedef std::shared_ptr<T>  ProductPtr;

public:
    IFactory(void){}
    virtual ~IFactory(void){}

    /// Overload this function to register factory's name
    virtual const std::string& getTypeName() = 0;

    /// Overload this function to appoint father
    virtual const std::string& getFatherName()
    {
        static const std::string fatherName = "";
        return fatherName;
    }

    virtual ProductPtr createInstance(const std::string& instanceName = "") = 0;

    virtual ProductPtr getInstance(const std::string& instanceName = "") = 0;

    virtual void deleteInstance(const std::string& instanceName = "") = 0;
};

template <class InterfaceType, class ClassType, char const* TypeName>//extern char const typename[]= "typename";
class Factory: public IFactory<InterfaceType>
{
public:
    typedef std::shared_ptr<InterfaceType>  ProductPtr;
    
    Factory(void)
    {}
    virtual ~Factory(void){}

    virtual const std::string & getTypeName() const
    {
        const static std::string name(TypeName);
        return name;
    }
    virtual ProductPtr createInstance(const std::string& instanceName)
    {
        return ProductPtr(new ClassType(instanceName));
    }
};

}}