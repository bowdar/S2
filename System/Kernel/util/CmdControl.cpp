//-------------------------------------------------------------------------------
// CmdControl.h
//
// @author
//     Millhaus.Chen @time 2016/08/31 11:57
//-------------------------------------------------------------------------------
#include "CmdControl.h"


S2::Kernel::CmdControl::CmdControl(const std::string &prompt)
{
    m_prompt = prompt;
    m_isRunning = false;
    m_cmdNotExist = "Invalid command !";
}

void S2::Kernel::CmdControl::addCommand(std::string strCmdName, S2::Kernel::CmdControl::FunctionType pFunc,
                                        std::string strCp)
{
//        m_commandsMap.insert(make_pair(strCmdName, pFunc));
//        m_CommentMap.insert(make_pair(strCmdName, strCp));
    if(m_commandsMap.find(strCmdName) == m_commandsMap.end())
    {
        m_commandsMap.insert(make_pair(strCmdName, pFunc));
    }
    else
    {
        m_commandsMap[strCmdName] = pFunc;
    }
    if(m_CommentMap.find(strCmdName) == m_CommentMap.end())
    {
        m_CommentMap.insert(make_pair(strCmdName, strCp));
    }
    else
    {
        m_CommentMap[strCmdName] = strCp;
    }
}

void S2::Kernel::CmdControl::run()
{
    m_isRunning = true;
    while(m_isRunning)
    {
        //std::cout << m_prompt << ">";
        printf("%s>", m_prompt.c_str());
        auto pCmdInputStrings = getCommand();
        if(pCmdInputStrings == nullptr) continue;
        if(pCmdInputStrings.get()[1].length() == 0) // only inputed "enter"
        {
            continue;
        }
        else if(pCmdInputStrings.get()[1] == "p")
        {
            while(1)
            {
                std::string command;
                getline(std::cin,command);
                if(command == "g")
                {
                    break;
                }
                else
                {
                    g_strList.push_back(command);
                }
            }
            continue;
        }
        else if(pCmdInputStrings.get()[1] == "g")
        {
            continue;
        }
        FunctionMap::iterator it = m_commandsMap.find(pCmdInputStrings.get()[1]);
        if(it != m_commandsMap.end())
        {
            m_isRunning  = it->second(pCmdInputStrings.get());
        }
        else
        {
            //std::cout << m_cmdNotExist << std::endl;
            std::system(m_command.c_str());
        }
    }
}

void S2::Kernel::CmdControl::execute(std::string &strCommand)
{
    auto pCmdInputStrings = getCommand(strCommand);
    FunctionMap::iterator it = m_commandsMap.find(pCmdInputStrings.get()[1]);
    if(it != m_commandsMap.end())
    {
        m_isRunning  = it->second(pCmdInputStrings.get());
    }
}

std::shared_ptr<std::string> S2::Kernel::CmdControl::getCommand(std::string strCommand) // return inputed command, and splided by string array
{
    if(g_strList.empty())
    {
        if(strCommand.length() == 0)
        {
            getline(std::cin, m_command);
        }
        else
        {
            m_command = strCommand;
        }
    }
    else
    {
        m_command = g_strList.front();
        g_strList.pop_front();
    }
//        if(m_command[0] == '#') return nullptr;
    int nNum = 1;
    std::deque<std::string> commands;
    int from = 0;
    int len = 1;
    for(unsigned int i = 0; i < m_command.size(); i++)
    {
        if(m_command[i] == ' ')
        {
            commands.push_back(m_command.substr(from, len-1));
            from = i+1;
            len = 1;
            nNum++;
        }
        else if(m_command[i] == '#')
        {
            commands.push_back(m_command.substr(from, len-1));
            from = i+1;
            len = 1;
            nNum++;
            break;
        }
        else
        {
            len++;
        }
    }
    commands.push_back(m_command.substr(from));
    std::shared_ptr<std::string> pResultStrings(new std::string[nNum + 1], std::default_delete<std::string[]>());
    pResultStrings.get()[0].append(1, (char)nNum); // pos 0 stored the number of commands
    std::deque<std::string>::iterator it = commands.begin();
    for(int i = 1; i <= nNum; i++)
    {
        pResultStrings.get()[i] = *it;
        it++;
    }
    return pResultStrings;
}
