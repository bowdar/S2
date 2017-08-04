//-------------------------------------------------------------------------------
// SampleShell.h
//
// @author
//     Millhaus.Chen @time 2016/02/23 16:48
//-------------------------------------------------------------------------------
#pragma once

#include "../../System/Kernel/util/IFactory.h"
#include "../common/BaseShell.h"

#include <string>
#include <memory>

class SampleShell : public BaseShell
{
public:
    typedef std::shared_ptr<SampleShell> Ptr;

    SampleShell(const std::string& prompt);
    const std::string& getModuleName() override;

public:
    bool cmd1(std::string command[]);
    bool cmd2(std::string command[]);
};