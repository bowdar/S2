//-------------------------------------------------------------------------------
// CmdControl.h
//
// @author
//     Millhaus.Chen @time 2016/08/31 11:57
//-------------------------------------------------------------------------------
#pragma once

#include <iostream>
#include <deque>
#include <map>
#include <functional>
#include <memory>
#include <string>
#include <list>

static std::list<std::string> g_strList;

namespace S2 { namespace Kernel {

class CmdControl
{
public:
    typedef std::shared_ptr<CmdControl> Ptr;
    typedef std::function<bool (std::string[])> FunctionType;
    typedef std::map<std::string, FunctionType> FunctionMap;
    typedef std::map<std::string, std::string> CommentMap;

public:
    CmdControl(const std::string& prompt)
    {
        m_prompt = prompt;
        m_isRunning = false;
        m_cmdNotExist = "Invalid command !";
    }

    void addCommand(std::string strCmdName, FunctionType pFunc, std::string strCp = "")
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

    const FunctionMap& getCmdsMap(){ return m_commandsMap; }
    const CommentMap& getCommentMap() { return m_CommentMap; }

    void run()
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

    void execute(std::string& strCommand)
    {
        auto pCmdInputStrings = getCommand(strCommand);
        FunctionMap::iterator it = m_commandsMap.find(pCmdInputStrings.get()[1]);
        if(it != m_commandsMap.end())
        {
            m_isRunning  = it->second(pCmdInputStrings.get());
        }
    }

    void setPrompt(std::string prompt) { m_prompt = prompt; }
    std::string& getPrompt() { return m_prompt; }
    const char* getCommandString() { return m_command.c_str(); }

private:
    std::shared_ptr<std::string> getCommand(std::string strCommand = "") // return inputed command, and splided by string array
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

private:
    std::string m_command;
    std::string m_prompt;
    std::string m_cmdNotExist;
    bool m_isRunning;

public:
    FunctionMap m_commandsMap;
    CommentMap m_CommentMap;
};

}}

#define REGISTER_CMD(cmd, comment, function) \
    m_pCtl->addCommand((cmd), std::bind(&function, this, std::placeholders::_1), (comment));

