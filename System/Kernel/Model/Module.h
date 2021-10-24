//-------------------------------------------------------------------------------
// Module.h
//
// @author
//     Millhaus.Chen @time 2016/08/31 11:41
//-------------------------------------------------------------------------------
#pragma once

#include "../util/DllExport.h"
#include "../SystemKernelDeclare.h"

#include <memory>
#include <string>
#include <boost/any.hpp>

namespace S2 { namespace Kernel {

class _DLL_EXPORTS Module
{
public:
    typedef std::shared_ptr<Module> Ptr;
    Module(const std::string& name = "noname");
    virtual ~Module(void);

    std::string& getName() { return m_name; }

    virtual void run() {}
    virtual void exit() {}

    virtual void setParam(boost::any anyParam) { m_anyParam = anyParam; }

protected:
    boost::any m_anyParam;

private:
    std::string m_name;
};

}}
