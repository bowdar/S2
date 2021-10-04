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
#include "Framework/Factories.h"

#include <memory>

namespace S2{ namespace Kernel {

class _DLL_EXPORTS Kernel : boost::noncopyable
{
public:
    typedef std::shared_ptr<Kernel> Ptr;
    
    Kernel(void);
    virtual ~Kernel(void);

    void run(SystemBuilder::Ptr pSystem);
    void loop();
    void exit(void);
};

}}

#include "util/SystemTools.h"
