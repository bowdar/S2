//-------------------------------------------------------------------------------
// SystemBuilder.cpp
//
// @author Millhaus.Chen       @time 2014/06/25 14:30
//-------------------------------------------------------------------------------
#include "../SystemKernelDeclare.h"
#include "../Model/Content.h"
#include "SystemBuilder.h"

using namespace S2::Kernel;

const std::string DLL("-l");
const std::string SHELL("-s");

ArgBuilder::ArgBuilder(int argc, char **argv):
m_argc(argc),
m_argv(argv)
{
    m_pContent.reset(new Content());
    write();
}

ArgBuilder::~ArgBuilder()
{

}


void ArgBuilder::write()
{
    for(int i=1; i<m_argc; ++i)
    {
        std::string str(m_argv[i]);

        if(str == DLL)
        {
            m_pContent->addModuleLibName(std::string(m_argv[++i]));
        }
    }
}

void ArgBuilder::build()
{
    m_pContent->loadModuleLib();
}

void ArgBuilder::demolish()
{
    m_pContent->unloadModuleLib();
}


CommonBuilder::CommonBuilder(Module::Ptr pModule)
{
    m_pContent.reset(new Content());
    m_pModule = pModule;
    write();
}

CommonBuilder::~CommonBuilder()
{

}


void CommonBuilder::write()
{
    m_pContent->addRunByKernel(m_pModule);
}

void CommonBuilder::build()
{

}

void CommonBuilder::demolish()
{

}
