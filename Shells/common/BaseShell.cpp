//-------------------------------------------------------------------------------
// BaseShell.cpp
//
// @author
//     Millhaus.Chen @time 2016/09/01 16:33
//-------------------------------------------------------------------------------
#include "BaseShell.h"
#include "../../System/Kernel/Framework/Factories.h"
#include "../../System/Kernel/util/Log.h"

#include <thread>

using namespace std;
using namespace S2::Kernel;

BaseShell::BaseShell(const std::string& prompt) : Shell(prompt)
{
	REGISTER_CMD("startup", "[path]        statup current shell module", BaseShell::startup);
	REGISTER_CMD("cs",      "..            switch shell", BaseShell::cdModule);
	REGISTER_CMD("ss",      "[ModuleID]    choise module of shell, none is broadcast", BaseShell::selectShell);
    REGISTER_CMD("log",     "<LEVEL>       contain trace,debug,info,warn,error,critical,off",BaseShell::setLogLevel);
    REGISTER_CMD("spec",    "<OBJECT_NAME> specific a module object of a shell for current use", BaseShell::specifyModule);
    REGISTER_CMD("create",  "<MODULE_NAME>", BaseShell::createModule);
	//REGISTER_CMD(crush,   "", BaseShell);

	m_shellName = prompt;
	m_moduleID = 0;
}

void BaseShell::run()
{
	m_pCtl->run();
}

//-------------------------------------------------------------------------------
// @description
//     call run in kernel module
//
// @added
//     Millhaus.Chen @time 2016/09/27 15:50
//-------------------------------------------------------------------------------
bool BaseShell::startup(std::string command[])
{
	int nNum = (int)(*command[0].c_str());

	std::string alias = "";
	if(nNum > 2)
	{
		alias = command[3];
	}

	//try
	//{
	//	ModulePtr pModule = Factories::GetInstance().createModule("S1." + m_pCtl->getPrompt() + "Factory", alias);
	//	if(nNum > 1)
	//	{
	//		pModule->setParam(command[2]);
	//	}
	//	pModule->run();
	//}
	//catch(std::string e)
	//{
	//	cout << "Module: " << e.c_str() << " startup failure !" << endl;
	//}
	//catch(...)
	//{
	//	cout << "Illegal commond !" << endl;
	//}

	// startup other process in thread
	thread threadStartup([this]()
	{
#if defined(__MINGW32__) || defined(_WIN32)
		string strStartup = "S1.Module." + m_pCtl->getPrompt();
#else
		string strStartup = "./S1.Module." + m_pCtl->getPrompt();
#endif
		system(strStartup.c_str());
	});
	//threadStartup.detach();


	return true;
}

//-------------------------------------------------------------------------------
// @description
//     switch module shell
//
// @added
//     Millhaus.Chen @time 2016/09/27 15:51
//-------------------------------------------------------------------------------
bool BaseShell::cdModule(std::string command[])
{
    int nNum = (int)(*command[0].c_str());
    if(nNum > 1)
    {
        if(command[2].compare("..") == 0)
        {
            return false;
        }
        else
        {
            try
            {
                ShellPtr pShell = Factories::GetInstance().createShell(command[2], m_pCtl->getPrompt());
                pShell->run();
                if(pShell->getIsExit())
                {
                    m_isExit = true;
                    return false;
                }
                else
                {
                    return true;
                }
            }
            catch(...)
            {
                cout << "ShellFactory \"" << command[2] << "Shell\" not loaded!" << endl;
            }
            return true;
        }
    }
    return true;
}


//-------------------------------------------------------------------------------
// @description
//     choise module of shell, none is broadcast
//
// @added
//     Millhaus.Chen @time 2016/09/27 15:52
//-------------------------------------------------------------------------------
bool BaseShell::selectShell(std::string command[])
{
	int nNum = (int)(*command[0].c_str());
	if(nNum > 1)
	{
		m_pCtl->setPrompt(m_shellName + ":" + command[2]);
		m_moduleID = atoi(command[2].c_str());
	}
	else
	{
		//cout << m_pCtl->getCommentMap().find(command[1])->second << endl;
		m_pCtl->setPrompt(m_shellName);
		m_moduleID = 0;
	}
	return true;
}

//-------------------------------------------------------------------------------
// @description
//      setting log level
//
// @added
//      tao.chen  @time 2016/11/28 17:43
//-------------------------------------------------------------------------------
bool BaseShell::setLogLevel(std::string command[])
{
    int nNum = (int)(*command[0].c_str());
    if(nNum > 1)
    {
        if("trace" == command[2])
        {
            Log::GetInstance().setLogLevel(spdlog::level::trace);
        }
        else if("debug" == command[2])
        {
            Log::GetInstance().setLogLevel(spdlog::level::debug);
        }
        else if("info" == command[2])
        {
            Log::GetInstance().setLogLevel(spdlog::level::info);
        }
        else if("warn" == command[2])
        {
            Log::GetInstance().setLogLevel(spdlog::level::warn);
        }
        else if("error" == command[2])
        {
            Log::GetInstance().setLogLevel(spdlog::level::err);
        }
        else if("critical" == command[2])
        {
            Log::GetInstance().setLogLevel(spdlog::level::critical);
        }else if("off" == command[2])
        {
            Log::GetInstance().setLogLevel(spdlog::level::off);
        }
        else
        {
            cerr <<  "No this level,please re-enter" << endl;
        }
    }
    else
    {
        cerr << "setting failed!" << endl;
        return true;
    }
    return true;
}

bool BaseShell::createModule(std::string command[])
{
    if(!CheckPrameter(command, 1)) return true;

    auto pModule = Factories::GetInstance().createModule(getModuleName(), command[2]);
    if(pModule == nullptr)
    {
        cerr << "Factory " << getModuleName() << " was not exist!" << endl;
        return true;
    }
    pModule->run();

    return true;
}

/// specific a module object for current use
bool BaseShell::specifyModule(std::string command[])
{
    if(!CheckPrameter(command, 1)) return true;

    setModuleObject(getModuleName(), command[2]);
    return true;
}

void BaseShell::setModuleObject(const std::string& moduleName, const std::string& objName)
{
    auto pModule = static_pointer_cast<Module>(Factories::GetInstance().getModule(moduleName, objName));
    if(pModule)
    {
        setModuleObject(pModule);
    }
    else
    {
        cerr << "specify failed!" << endl;
    }
}

void BaseShell::setModuleObject(std::shared_ptr<Module> pModule)
{
    m_pCurModule = pModule;
    m_pCtl->setPrompt(m_shellName + "@" + m_pCurModule->getName());
}