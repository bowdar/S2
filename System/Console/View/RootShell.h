//-------------------------------------------------------------------------------
// RootShell.h
//
// @author
//     Millhaus.Chen @time 2016/09/02 10:09
//-------------------------------------------------------------------------------
#pragma once

#include "../../Kernel/Model/Shell.h"

#include <string>

namespace S2 { namespace Console {

class RootShell : public S2::Kernel::Shell
{
public:
    typedef std::shared_ptr<RootShell> Ptr;
    
    RootShell(const std::string& prompt);
    virtual void run();

private:
    bool quit(std::string command[]);
    //bool cdShell(std::string command[]);

public:
    //CmdControlPtr m_pCtl;
};

}}

