//-------------------------------------------------------------------------------
// SystemBuilder.h
//
// @brief
//     Builderģʽ��ϵͳ������
//
// @author
//     Millhaus.Chen @time 2016/08/31 11:25
//-------------------------------------------------------------------------------
#pragma once

#include "../util/DllExport.h"
#include "../Model/Content.h"

#include <memory>

namespace S2{ namespace Kernel {

class _DLL_EXPORTS SystemBuilder
{
public:
    typedef std::shared_ptr<SystemBuilder> Ptr;
    
    SystemBuilder(void) {}
    virtual ~SystemBuilder() {}

    virtual void write() = 0;
    virtual void build() = 0;
    virtual void demolish() = 0;

    const Content::Ptr getContent() { return m_pContent; }

protected:
    Content::Ptr m_pContent;
};

class _DLL_EXPORTS CommonBuilder: public SystemBuilder
{
public:
    CommonBuilder(Module::Ptr pModule);
    virtual ~CommonBuilder(void);
    virtual void write();
    virtual void build();
    virtual void demolish();

private:
    Module::Ptr m_pModule;
};

class _DLL_EXPORTS ArgBuilder: public SystemBuilder
{
public:
    ArgBuilder(int argc, char **argv);
    virtual ~ArgBuilder(void);
    virtual void write();
    virtual void build();
    virtual void demolish();

private:
    int m_argc;
    char** m_argv;
};

}}
