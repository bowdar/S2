//-------------------------------------------------------------------------------
// Main.cpp
//
// @note
//     S2.System.Console
//     
// @author
//     Millhaus.Chen @time 2016/08/31 10:13
//-------------------------------------------------------------------------------
#include "../Kernel/Kernel.h"
#include "../Kernel/Framework/SystemBuilder.h"
#include "View/RootShell.h"

#include <memory>

using namespace S2::Kernel;
using namespace S2::Console;

int main(int argc, char **argv)
{
    Kernel::Ptr pKernel(new Kernel());
    
    const RootShell::Ptr pRootShell(new RootShell(""));

    SystemBuilder::Ptr pSystem(new ArgBuilder(argc, argv));
    SystemBuilder::Ptr pRunByKernel(new CommonBuilder(
            std::static_pointer_cast<Module>(pRootShell)));

    pSystem->build();
    pRunByKernel->build();

    pKernel->Run(pRunByKernel);

    pKernel->Exit();
    pRunByKernel->demolish();
    pSystem->demolish();

    return 0;
}
