//-------------------------------------------------------------------------------
// IKernel.cpp
//
// @author
//     Millhaus.Chen @time 2014/06/23 14:04
//
// @modified
//     Millhaus.Chen @time 2014/06/25 16:41
//-------------------------------------------------------------------------------

#include "IKernel.h"
#include "Model/Content.h"
#include "Model/Module.h"

#include <thread>

using namespace S2::Kernel;

IKernel::IKernel(void)
{
}

IKernel::~IKernel(void)
{
}

void IKernel::run(SystemBuilder::Ptr pSystem)
{
    //// @note Millhaus.Chen @time 2016/05/21 13:48
    ////    Kernel里面跑的协程单例实例跟模块里面的不同，暂未解决问题
    //std::thread thr([=]
    //{
    //    co_sched.RunLoop();        
    //});
    //thr.detach();

    Content::ModuleList moduleList = pSystem->getContent()->getModuleList();
    for(Content::ModuleList::iterator it = moduleList.begin(); it != moduleList.end(); ++it)
    {
        (*it)->run();
    }
}

void IKernel::exit(void)
{

}
