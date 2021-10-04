//-------------------------------------------------------------------------------
// RootShell.cpp
//
// @author
//     Millhaus.Chen @time 2016/08/31 10:55
//-------------------------------------------------------------------------------
#include "RootShell.h"
#include "../../Kernel/Framework/Factories.h"
#include "../../Kernel/util/Log.h"

#include <functional>

using namespace S2::Console;
using namespace S2::Kernel;
using namespace std;

RootShell::RootShell(const std::string& prompt) : Shell(prompt)
{
    //m_pCtl->addCommand("quit", bind(&RootShell::quit, this, placeholders::_1),       "              quit system");
    //m_pCtl->addCommand("ls", bind(&RootShell::listShells, this, placeholders::_1),   "              list loaded shells by system");
    //m_pCtl->addCommand("cs", bind(&RootShell::cdShell, this, placeholders::_1),      " <name>       enter shell");
    //m_pCtl->addCommand("lr", bind(&RootShell::listModules, this, _1),  "              Module");
    REGISTER_CMD("quit","quit system",RootShell::quit);
}

void RootShell::run()
{
    m_pCtl->run();
}

bool RootShell::quit(std::string command[])
{
    cout << "Exit console ? (y/n) :";
    char c;
    cin >> c;
    if(c == 'y' || c == 'Y')
    {
        Log::GetInstance().flushLog();
        spdlog::drop_all();
        return false;
    }
    return true;
}

//bool RootShell::cdShell(std::string command[])
//{
//    int nNum = (int)(*command[0].c_str());
//
//    if(nNum > 1)
//    {
//        try
//        {
//            ShellPtr pShell = Factories::GetInstance().createShell(command[2]);
//            pShell->run();
//        }
//        catch(...)
//        {
//            cout << "ShellFactory \"" << command[2] << "Shell\" not loaded!" << endl;
//        }
//    }
//
//    return true;
//}

// bool RootShell::listModules(std::string command[])
// {
//     using namespace boost::interprocess;
// 
//     cout << "Rrunning servers:" << endl;
// 
//     Factories::ShellFactoryMap::const_iterator it = Factories::GetInstance().getShellFactoryMap().begin();
//     for(; it != Factories::GetInstance().getShellFactoryMap().end(); it++)
//     {
//         try
//         {
//             managed_shared_memory segment(open_only, it->first.c_str());
// 
//             SsmSet<int>::Type * pModuleList = segment.find<SsmSet<int>::Type>(SSM_SERVER_LIST).first;
//             if(pModuleList)
//             {
//                 cout << "\t";
//                 for(SsmSet<int>::Type::iterator itModule = pModuleList->begin(); itModule != pModuleList->end(); itModule++)
//                 {
//                     cout << it->first << "(" << *itModule << ") ";
//                 }
//                 cout << endl;
//             }
//         }catch(...) {}
//     }
//     return true;
// }
