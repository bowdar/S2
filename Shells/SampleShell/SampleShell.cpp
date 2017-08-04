//-------------------------------------------------------------------------------
// SampleShell.cpp
//
// @author
//     Millhaus.Chen @time 2016/02/23 15:22
//-------------------------------------------------------------------------------
//#include "Sample.h"
#include "SampleShell.h"

#include "../../System/Kernel/Framework/Factories.h"
#include "../common/TFactory.h"

using namespace std;
using namespace S2::Kernel;

SampleShell::SampleShell(const std::string& prompt) : BaseShell(prompt)
{
    //REGISTER_CMD("cmd1",     "...", Sample::cmd2);
	//REGISTER_CMD("cmd2",     "...", Sample::cmd2);
}

/// Register factory's name, every factory need implement this funciton
template<>
const std::string& TShellFactory<SampleShell>::getTypeName()
{
    static const std::string typeName("Sample");
    return typeName;
}

const std::string& SampleShell::getModuleName()
{
    static const std::string moduleName("SampleModule");
    return moduleName;
}

bool SampleShell::cmd1(std::string command[])
{
    //static_pointer_cast<Sample>(m_pCurModule)->cmd1();
    return true;
}

bool SampleShell::cmd2(std::string command[])
{
    //static_pointer_cast<Sample>(m_pCurModule)->cmd2();
    return true;
}