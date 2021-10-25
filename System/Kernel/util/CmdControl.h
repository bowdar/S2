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
    using FunctionType = std::function<bool (std::string[])>;
    using FunctionMap = std::map<std::string, FunctionType>;
    using CommentMap = std::map<std::string, std::string>;

public:
    CmdControl(const std::string& prompt);

    void addCommand(std::string strCmdName, FunctionType pFunc, std::string strCp = "");

    const FunctionMap& getCmdsMap(){ return m_commandsMap; }
    const CommentMap& getCommentMap() { return m_CommentMap; }

    void run();

    void execute(std::string& strCommand);

    void setPrompt(std::string prompt) { m_prompt = prompt; }
    std::string& getPrompt() { return m_prompt; }
    const char* getCommandString() { return m_command.c_str(); }

private:
    std::shared_ptr<std::string> getCommand(std::string strCommand = "");

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

