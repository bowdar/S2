#pragma once

#include "TupleTool.hpp"

#include <string>
#include <tuple>

namespace mtl
{

//-------------------------------------------------------------------------------
// @description
//     字符串切割
//
// @added
//     Millhaus.Chen @time 2017/03/08 15:14
//-------------------------------------------------------------------------------
inline std::vector<std::string> split(std::string stringIn, char toker)
{
    std::vector<std::string> stringOut;

    std::string::size_type pos1, pos2;
    pos2 = stringIn.find(toker);
    pos1 = 0;
    while (std::string::npos != pos2)
    {
        stringOut.push_back(stringIn.substr(pos1, pos2 - pos1));

        pos1 = pos2 + 1;
        pos2 = stringIn.find(toker, pos1);
    }
    stringOut.push_back(stringIn.substr(pos1));
    return stringOut;
}

//-------------------------------------------------------------------------------
// @description
//     字符串剔除
//
// @added
//     Millhaus.Chen @time 2017/03/09 13:39
//
// @modified
//     修改为支持变参多个char Millhaus.Chen @time 2017/03/16 13:43
//-------------------------------------------------------------------------------
template<typename... Chars>
std::string erase(std::string stringIn, Chars... chars)
{
    std::tuple<Chars...> charsTup = std::make_tuple(chars...);
    for_each(charsTup, [&stringIn](const char &c) mutable
    {
        std::string::iterator new_end = std::remove_if(stringIn.begin(), stringIn.end(),
                                                       bind2nd(std::equal_to<char>(), c));
        stringIn.erase(new_end, stringIn.end());
    });
    return stringIn;
}

}