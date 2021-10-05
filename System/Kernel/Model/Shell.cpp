//-------------------------------------------------------------------------------
// Shell.cpp
//
// @author
//     Millhaus.Chen @time 2014/09/02 10:09
//
// @modified
//     Millhaus.Chen @time 2014/11/20 09:45
//-------------------------------------------------------------------------------
#include "Shell.h"

#include "../Framework/Factories.h"
#include "../util/IFactory.h"
#include "../util/Log.h"

#include <algorithm>
#include <cfloat>

using namespace S2::Kernel;
using namespace std;
using namespace boost;

Shell::Shell(const std::string& prompt)
{
	m_pCtl.reset(new CmdControl(prompt));

	REGISTER_CMD("?",    "", Shell::help);
	REGISTER_CMD("help", "", Shell::help);
    REGISTER_CMD("quit", "", Shell::quit);
    REGISTER_CMD("ls",   "list loaded shells by system", Shell::listShells);
    REGISTER_CMD("cs",   "<name>       enter shell", Shell::cdShell);
    REGISTER_CMD("exit", "", Shell::exitSys);
    REGISTER_CMD("flushlog", "", Shell::flushLog);
//    REGISTER_CMD("test", "", Shell::test);

    m_isExit = false;
}

bool Shell::help(string command[])
{
	int nNum = (int)(*command[0].c_str());

	if(nNum > 1 && command[2] != "-a")
	{
		CmdControl::CommentMap::const_iterator it = m_pCtl->getCommentMap().find(command[2]);
		if(it != m_pCtl->getCommentMap().end())
		{
			cout << "\t" << it->first << "\t" << it->second << endl;
		}
		return true;
	}

	CmdControl::FunctionMap::const_iterator it = m_pCtl->getCmdsMap().begin();
	for(; it != m_pCtl->getCmdsMap().end(); it++)
	{
		if(it->first == "?")
		{
			continue;
		}
		cout << "\t" << it->first;
		if(nNum > 1 && command[2] == "-a")
		{
			CmdControl::CommentMap::const_iterator itComment = m_pCtl->getCommentMap().find(it->first);
			if(itComment != m_pCtl->getCommentMap().end())
			{
                if(strlen(it->first.c_str()) <= 4) cout << "\t\t";
                else if(strlen(it->first.c_str()) <= 8) cout << "\t";
				cout << "\t" << itComment->second << endl;
			}
		}
	}
	cout << endl;
	return true;
}

bool Shell::quit(string command[])
{
    return false;
}

bool Shell::doNothing(string command[])
{
	return true;
}

bool Shell::CheckAll(string command[], int n)
{
    return CheckPrameter(command, n) && CheckModule();
}

bool Shell::CheckPrameter(string command[], int n)
{
    int nNum = (int)(*command[0].c_str());
    if(nNum < n + 1)
    {
        CmdControl::CommentMap::const_iterator itComment = m_pCtl->getCommentMap().find(command[1]);
        if(itComment != m_pCtl->getCommentMap().end())
        {
            printf("Command error, ref: %s %s\n", command[1].c_str(), itComment->second.c_str());
            return false;
        }
    }
    return true;
}

bool Shell::CheckModule()
{
    if(!m_pCurModule)
    {
        cerr << "Module was not specify !" << endl;
        return false;
    }
    return true;
}

bool Shell::listShells(std::string command[])
{
    const auto& shellFactoryMap = Factories::GetInstance().getShellFactoryMap();
    for_each(shellFactoryMap.begin(), shellFactoryMap.end(),
             [this](Factories::ShellFactoryMap::const_reference ref)
    {
        auto& prompt = m_pCtl->getPrompt();
        size_t begin = m_pCtl->getPrompt().rfind('/');
        begin = begin == string::npos ? 0 : begin + 1;
        auto father = prompt.substr(begin, prompt.length() - prompt.rfind('@'));
        if(ref.second->getFatherName().compare(father) == 0)
        {
            printf("\t%s\n", ref.first.c_str());
        }
    });
    return true;
}

bool Shell::cdShell(std::string command[])
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
                const auto& shellFactoryMap = Factories::GetInstance().getShellFactoryMap();
                auto iter = shellFactoryMap.find(command[2]);
                if(iter != shellFactoryMap.end())
                {
                    if((iter->second->getFatherName().length() == 0 && m_pCtl->getPrompt().length() == 0)
                       || m_pCtl->getPrompt().compare(iter->second->getFatherName()) == 0)
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
                }
            } catch(...) { }

            printf("Can not find \"%s Shell\" in this commands directory!\n", command[2].c_str());
            return true;
        }
    }
    return true;
}

//-------------------------------------------------------------------------------
// @description
//      exit system
// @added
//      tao.chen  @time 2016/12/23 13:08
//-------------------------------------------------------------------------------
bool Shell::exitSys(std::string command[])
{
//    Exit(0);
    cout << "Exit console ? (y/n) :";
    char c;
    cin >> c;
    if(c == 'y' || c == 'Y')
    {
        Log::GetInstance().flushLog();
        spdlog::drop_all();
        m_isExit = true;
        return false;
    }
    return true;
}

//-------------------------------------------------------------------------------
// @description
//      flush global log(m_globalLog)
// @added
//      tao.chen  @time 2016/12/30 10:44
//-------------------------------------------------------------------------------
bool Shell::flushLog(std::string command[])
{
    Log::GetInstance().flushLog();
    return true;
}


bool Shell::test(std::string command[])
{
    LOGCONSOLE_WARN("{}",DBL_MAX);
    LOGCONSOLE_WARN("{}",DBL_MIN);
    return true;
}