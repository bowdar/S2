//-------------------------------------------------------------------------------
// Shell.h
//
// @author
//     Millhaus.Chen @time 2016/08/31 11:54
//
// @modified
//     Millhaus.Chen @time 2016/09/21 17:22 Implement command tree
//-------------------------------------------------------------------------------
#pragma once

#include "Module.h"

#include "../util/DllExport.h"
#include "../util/CmdControl.h"

#include <map>
#include <string>

namespace S2 { namespace Kernel {

class _DLL_EXPORTS Shell : public Module
{
public:
    Shell(const std::string& prompt);
    bool getIsExit(){ return m_isExit;}

private:
    virtual bool help(std::string command[]);
    virtual bool quit(std::string command[]);
    bool flushLog(std::string command[]);
    bool listShells(std::string command[]);
    bool cdShell(std::string command[]);
    bool doNothing(std::string command[]);
    bool exitSys(std::string command[]);
    bool test(std::string command[]);

protected:
    bool CheckAll(std::string command[], int n);
    bool CheckPrameter(std::string command[], int n);
    bool CheckModule();

public:
    CmdControl::Ptr m_pCtl;

protected:
    bool m_isExit = false;
    std::shared_ptr<S2::Kernel::Module> m_pCurModule = nullptr;
};

}}
