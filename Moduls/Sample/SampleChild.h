//-------------------------------------------------------------------------------
// SampleChild.h
//
// @author
//     Millhaus.Chen @time 2016/02/23 16:48
//-------------------------------------------------------------------------------
#pragma once

#include "../../System/Kernel/Model/Module.h"

class _DLL_EXPORTS SampleChild : public Modlue
{
public:
    typedef std::shared_ptr<SampleChild> Ptr;

    Sample();

public:
    bool cmd1();
    bool cmd2();
};