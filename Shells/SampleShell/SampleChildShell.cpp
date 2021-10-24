//-------------------------------------------------------------------------------
// SampleChildShell.cpp
//
// @author
//     Millhaus.Chen @time 2016/02/23 15:22
//-------------------------------------------------------------------------------
#include "SampleChildShell.h"

#include "../../System/Kernel/Framework/Factories.h"
#include "../common/TFactory.h"

#include "../../Moduls/Sample/SampleChild.h"

using namespace std;
using namespace S2::Kernel;

SampleChildShell::SampleChildShell(const std::string& prompt) : BaseShell(prompt)
{
    REGISTER_CMD("cmd1",     "...", SampleChild::cmd2);
	REGISTER_CMD("cmd2",     "...", SampleChild::cmd2);
}

/// Register factory's name, every factory need implement this funciton
template<>
const std::string& TShellFactory<SampleChildShell>::getTypeName()
{
    static const std::string typeName("SampleChild");
    return typeName;
}

const std::string& SampleChildShell::getModuleName()
{
    static const std::string moduleName("SampleChildModule");
    return moduleName;
}

template<>
const std::string& TShellFactory<SampleChildShell>::getFatherName()
{
    static const std::string fatherName("Sample");
    return fatherName;
}

bool SampleChildShell::cmd1(std::string command[])
{
    static_pointer_cast<SampleChild>(m_pCurModule)->cmd1();
    return true;
}

bool SampleChildShell::cmd2(std::string command[])
{
    static_pointer_cast<SampleChild>(m_pCurModule)->cmd2();
    return true;
}