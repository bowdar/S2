//-------------------------------------------------------------------------------
// Tools.h
//
// @brief
//     一些公共函数
//
// @author
//     Millhaus.Chen @time 2015/10/29 14:34
//-------------------------------------------------------------------------------
#pragma once

#include <vector>
#include <random>
#include <cstring>

namespace mtl {

//-------------------------------------------------------------------------------
// @description
//     随机访问迭代器
//
// @added
//     Millhaus.Chen @time 2016/04/19 11:48
//-------------------------------------------------------------------------------
template<class _Ty>
struct random_iterator
{
    random_iterator(const _Ty& Ty) : m_Ty(Ty), nSize(Ty.size()) {}
    typename _Ty::const_iterator next()
    {
        return std::next(m_Ty.begin(), std::uniform_int_distribution<int>(0, nSize)(rd));
    }
    const _Ty& m_Ty;
    size_t nSize;
    std::random_device rd{ std::random_device() };
};


//-------------------------------------------------------------------------------
// @description
//     根据当前时间和每天生效时刻计算上次时间
//
// @added
//     Millhaus.Chen @time 2015/08/14 14:33
//-------------------------------------------------------------------------------
time_t makeLastTime(time_t tCurTime, int nActionHour = 0, int nActionMinute = 0, int nActionSecond = 0);


//-------------------------------------------------------------------------------
// @description
//     检查上一次的时间,是否超过当前时间
//-------------------------------------------------------------------------------
bool checkOverDay(time_t tCurTime = 0, time_t tLasTime = 0);


//-------------------------------------------------------------------------------
// @description
//     判断当前时间,是否在当天的一个指定时间段之内
//-------------------------------------------------------------------------------
bool checkBetweenTime(time_t tCurTime, int nBeginHour = 0, int nBeginMinute = 0, int nBeginSecond = 0, int nEndHour = 0, int nEndMinute = 0, int nEndSecond = 0);


//-------------------------------------------------------------------------------
// @description
//     根据配置天、周、月参数检查是否逾期
//-------------------------------------------------------------------------------
bool checkOverSeveralDays(int nDays, time_t tCurTime = 0, time_t tLasTime = 0);

//-------------------------------------------------------------------------------
// @description
//     万能的生效时间类
//
// @added
//     Millhaus.Chen @time 2015/11/09 21:20
//-------------------------------------------------------------------------------
struct ST_Valid_Time
{
    long long nMask[6];
};

class ValidTime
{
    using TimeList = std::vector<ST_Valid_Time>;
public:
    void init(std::string& str);

    // 可以开始了吗？
    bool shallWe(time_t tTime) const;
    bool shallWeNow() const;

private:
    // 将对应时刻点添加到时刻掩码中去，比如：周一到周五，二进制掩码就是 00111110 ，最后一位为0是表示星期0不存在，但是当作为时分秒的时候0是有作用的
    void addToMask();

private:
    void parse(std::string& str);

private:
    int m_nStackDepth;			// 递归深度
    int m_nNumber;				// 解析过程中产生的数字
    int m_nSectionNumberBegin;	// 临时保存连续段的开始值
    bool m_bValidNumber;		// 是否完成一个数字的解析
    bool m_bNumberSection;		// 是否进入连续段计算
    bool m_bExclude;			// 是否排除后面的数
    int m_nIndex;

    std::string m_str;
    TimeList m_vecValidTime;
};

//-------------------------------------------------------------------------------
// @description
//     拆分字符串为数字, pstrTag用来记录日志
//
// @added
//     Millhaus.Chen @time 2016/06/02 11:53
//-------------------------------------------------------------------------------
template<class T>
bool stringToNumbers(std::string& strIn, T& listOut, const char* pstrTag = "", const char* pstrTok = ",")
{
    static const int strLength = 1024;
    static char pstrBuffer[strLength];
    char* pstrDividing = pstrBuffer;
    if(strIn.length() <= 0 || strIn.length() > strLength)
    {
        //LogSystem[Err]("stringToNumbers() %s string length error !", pstrTag);
        return false;
    }
    memset(pstrDividing, 0, strLength);
    std::strncpy(pstrDividing, strIn.c_str(), strIn.length());
    pstrDividing = strtok(pstrDividing, pstrTok);
    while(pstrDividing != NULL)
    {
        listOut.insert(atoi(pstrDividing));
        pstrDividing = strtok(NULL, pstrTok);
    }
    return true;
}

} // namespace mtl