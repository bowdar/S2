//-------------------------------------------------------------------------------
// Main.cpp
//
// @note
//     S2.System.Console
//     
// @author
//     Millhaus.Chen @time 2016/08/31 10:13
//-------------------------------------------------------------------------------
#include "../Kernel/IKernel.h"
#include "../Kernel/Framework/SystemBuilder.h"
#include "View/RootShell.h"

#include <memory>
#include <string>

using namespace S2::Kernel;
using namespace S2::Console;

int main(int argc, char **argv)
{
    IKernel::Ptr pKernel(new IKernel());
    
    std::string s("");
    RootShell::Ptr pRootShell(new RootShell(s));

    SystemBuilder::Ptr pSystem(new ArgBuilder(argc, argv));
    SystemBuilder::Ptr pRunByKernel(new CommonBuilder(std::static_pointer_cast<Module>(pRootShell)));

    pSystem->build();
    pRunByKernel->build();

    pKernel->run(pRunByKernel);

    pKernel->exit();
    pSystem->demolish();

    return 0;
}
