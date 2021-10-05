//-------------------------------------------------------------------------------
// Kernel.h
//
// @author
//     Millhaus.Chen @time 2014/06/23 14:04 
//
// @modified
//     Millhaus.Chen @time 2014/06/25 15:52
//-------------------------------------------------------------------------------
#pragma once

#include "util/DllExport.h"
#include "Framework/SystemBuilder.h"
#include "util/ThreadPool.h"
#include "Task.hpp"

#include <memory>

namespace S2{ namespace Kernel {

class _DLL_EXPORTS Kernel final : public boost::noncopyable
{
public:
    typedef std::shared_ptr<Kernel> Ptr;
    typedef Task<void()> KernelLoopType;
    
    Kernel(void);

    void Run(SystemBuilder::Ptr pSystem);
    void Loop();
    void Exit(void);

private:
    KernelLoopType::Ptr m_pKernelLoop;
};

}}

#include "util/SystemTools.h"
