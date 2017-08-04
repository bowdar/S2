//-------------------------------------------------------------------------------
// BaseShell.h
//
// @author
//     Millhaus.Chen @time 2016/09/01 16:33
//-------------------------------------------------------------------------------
#pragma once

#include "../../System/Kernel/Model/Shell.h"

#include <string>
#include <memory>

class BaseShell : public S2::Kernel::Shell
{
public:
	BaseShell(const std::string& prompt);
	virtual void run();

    virtual const std::string& getModuleName() = 0;

protected:
	bool cdModule(std::string command[]);
	bool startup(std::string command[]); /// [disused]
	bool selectShell(std::string command[]); /// [disused]
	bool setLogLevel(std::string command[]);
    bool specifyModule(std::string command[]);
    bool createModule(std::string command[]);

private:
    void setModuleObject(const std::string& moduleName, const std::string& objName);
    void setModuleObject(std::shared_ptr<S2::Kernel::Module> pModule);

protected:
	int m_moduleID;
    std::string m_shellName;
};
