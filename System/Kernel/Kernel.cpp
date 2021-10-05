//-------------------------------------------------------------------------------
// Kernel.cpp
//
// @author
//     Millhaus.Chen @time 2014/06/23 14:04
//
// @modified
//     Millhaus.Chen @time 2014/06/25 16:41
//-------------------------------------------------------------------------------

#include "Kernel.h"
#include "Model/Content.h"
#include "Model/Module.h"
#include "Task.hpp"

#include <functional>

using namespace S2::Kernel;

Kernel::Kernel(void)
{
}

void Kernel::Run(SystemBuilder::Ptr pSystem)
{
    //// @note Millhaus.Chen @time 2016/05/21 13:48
    ////    Kernel里面跑的co_go单例实例跟模块里面的不同，暂未解决问题
    //std::thread thr([=]
    //{
    //    co_sched.RunLoop();        
    //});
    //thr.detach();

    m_pKernelLoop = std::make_shared<KernelLoopType>(std::bind(this, &Kernel::Loop));
    m_pKernelLoop->RunForever();

    Content::ModuleList moduleList = pSystem->getContent()->getModuleList();
    for(Content::ModuleList::iterator it = moduleList.begin(); it != moduleList.end(); ++it)
    {
        (*it)->run();
    }
}

void Kernel::Loop()
{
}

void Kernel::Exit(void)
{
    m_pKernelLoop->Terminate();
}
