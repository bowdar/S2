//-------------------------------------------------------------------------------
// Tools.cpp
//
// @author
//     Millhaus.Chen @time 2015/11/06 10:21
//-------------------------------------------------------------------------------

#include "Tools.h"
#include "CodeChange.h"

#include <ctime>
#include <memory>

#ifndef DEFINE_TIME_24_HOUR
#	define DEFINE_TIME_24_HOUR (86400)
#endif

using namespace std;

namespace mtl {

//-------------------------------------------------------------------------------
// @description
//     根据当前时间和每天生效时刻计算上次时间
//
// @added
//     Millhaus.Chen @time 2015/11/04 18:55
//-------------------------------------------------------------------------------
time_t makeLastTime(time_t tCurTime, int nActionHour, int nActionMinute, int nActionSecond)
{
    struct tm* pCurTime = localtime(&tCurTime);
    time_t tLastTime = tCurTime;

    // 重置的时候检测距离上次是否超过1天，重置时间始终赋值为上一个0点（可能会减去1天）
    if(pCurTime->tm_hour * 10000 + pCurTime->tm_min * 100 + pCurTime->tm_sec < nActionHour * 10000 + nActionMinute * 100 + nActionSecond)
    {
        tLastTime -= DEFINE_TIME_24_HOUR;
    }
    pCurTime = localtime(&tLastTime);
    pCurTime->tm_hour = nActionHour;
    pCurTime->tm_min = nActionMinute;
    pCurTime->tm_sec = nActionSecond;

    tLastTime = mktime(pCurTime);

    return tLastTime;
}

//检查上一次的时间,是否超过当前时间
bool checkOverDay(time_t tCurTime, time_t tLasTime)
{
    if (tLasTime > tCurTime)
    {
        return false;
    }
    // localtime非线程安全，最好不用指针，尤其是有两个的时候绝对不能用指针 @modified Millhaus.Chen @time 2015/12/09 10:00
    tm pCurTime = *localtime(&tCurTime);
    tm pLasTime = *localtime(&tLasTime);

    pLasTime.tm_hour = 23;
    pLasTime.tm_min = 59;
    pLasTime.tm_sec = 59;

    time_t tTime = mktime(&pLasTime);

    if (tCurTime > tTime)
    {
        return true;
    }
    return false;
}


//判断当前时间,是否在当天的一个指定时间段之内
bool checkBetweenTime(time_t tCurTime, int nBeginHour, int nBeginMinute, int nBeginSecond, int nEndHour, int nEndMinute, int nEndSecond)
{
    struct tm* pCurTime = localtime(&tCurTime);
    time_t tLastTime = tCurTime;

    //modified zhangqi
    time_t tNow = pCurTime->tm_hour * 10000 + pCurTime->tm_min * 100 + pCurTime->tm_sec;
    time_t tBegin = nBeginHour * 10000 + nBeginMinute * 100 + nBeginSecond;
    time_t tEnd = nEndHour * 10000 + nEndMinute * 100 + nEndSecond;

    if (tNow >= tBegin && tNow <= tEnd)
    {
        return true;
    }

    return false;
}


// 根据配置天、周、月参数检查是否逾期
bool checkOverSeveralDays(int nDays, time_t tCurTime, time_t tLasTime)
{
    std::tm m_tm = *localtime(&tCurTime);
    long long nDetlaTime = tCurTime - tLasTime;
    if(nDays == 7)
    {
        if(nDetlaTime >= 7 * DEFINE_TIME_24_HOUR && m_tm.tm_wday == 1)
        {
            return true;
        }
    }
    else if(nDays == 30)
    {
        if(nDetlaTime >= 30 * DEFINE_TIME_24_HOUR && m_tm.tm_mday == 1)
        {
            return true;
        }
    }
    else
    {
        if(nDetlaTime >= nDays * DEFINE_TIME_24_HOUR)
        {
            return true;
        }
    }

    return false;
}


void ValidTime::init(std::string& str)
{
    m_str = str;
    m_nStackDepth = 0;
    m_bValidNumber = false;
    m_bNumberSection = false;
    m_nIndex = -1;
    m_nNumber = 0;
    m_nSectionNumberBegin = -1;
    m_bExclude = false;
    m_vecValidTime.clear();

    parse(str);
}

void ValidTime::addToMask()
{
    if(m_nStackDepth - 1 < 0) return;
    if(m_bValidNumber)
    {
        // 处理连续段：0-30 这样的，30多位全部置1
        if(m_nSectionNumberBegin >= 0 && m_bNumberSection)
        {
            while(m_nSectionNumberBegin < m_nNumber)
            {
                m_vecValidTime[m_nIndex].nMask[m_nStackDepth-1] |= ((long long)0x1 << m_nSectionNumberBegin);
                m_nSectionNumberBegin++;
            }
            m_nSectionNumberBegin = -1;
            m_bNumberSection = false;
        }
        if ( !m_bExclude )
        {
            // 此函数核心语句，当前位，置1
            m_vecValidTime[m_nIndex].nMask[m_nStackDepth-1] |= ((long long)0x1 << m_nNumber);
        }
        else
        {
            // 对排除位，置0，排除暂不支持连续段，如果支持代码更别扭
            m_vecValidTime[m_nIndex].nMask[m_nStackDepth-1] &= ~((long long)0x1 << m_nNumber);
            m_bExclude = false;
        }

        m_nNumber = 0;
        m_bValidNumber = false;
    }
}

void ValidTime::parse(std::string& str)
{
    char ch;

    while(str.size() > 0)
    {
        ch = str.at(0);
        switch(ch)
        {
            case '{':
                if(m_nStackDepth == 0)
                {
                    ST_Valid_Time timeMask;
                    memset(&timeMask, 0, sizeof(ST_Valid_Time));
                    m_vecValidTime.push_back(timeMask);
                    m_nIndex++;
                }
                addToMask();
                m_nStackDepth++;
                parse(str.erase(0, 1));
                break;
            case '}':
                addToMask();
                // 如果计算的上层括号栈区没有填写数字，则认为全部满足，置为全1
                if(m_nStackDepth - 1 >= 0)
                {
                    if(m_vecValidTime[m_nIndex].nMask[m_nStackDepth-1] == 0)
                    {
                        m_vecValidTime[m_nIndex].nMask[m_nStackDepth-1] = ~m_vecValidTime[m_nIndex].nMask[m_nStackDepth-1];
                    }
                }
                str.erase(0, 1);
                m_nStackDepth--;
                return;
                break;
            case '|':
                addToMask();
                str.erase(0, 1);
                break;
            case '-':
                m_nSectionNumberBegin = m_nNumber;
                addToMask();
                m_bNumberSection = true;
                str.erase(0, 1);
                break;
            case '!':
                addToMask();
                m_bExclude = true;
                str.erase(0, 1);
                break;
            default:
                if(ch >= '0' && ch <= '9')
                {
                    m_bValidNumber = true;
                    m_nNumber = m_nNumber * 10 + (ch - '0');
                    str.erase( 0, 1 );
                }
                //else
                //{
                //	return;
                //}
                break;
        }
    }
}

bool ValidTime::shallWeNow() const
{
    return shallWe(time(NULL));
}

bool ValidTime::shallWe(time_t tTime) const
{
    struct tm* pCurTime = localtime(&tTime);
    if(!pCurTime) return false;

    bool bValid = true;

    TimeList::const_iterator it = m_vecValidTime.begin();
    for(; it != m_vecValidTime.end(); ++it)
    {
        bValid = true;
        (it->nMask[0] & ((long long)0x01 << (pCurTime->tm_mon + 1))) > 0 ? true : bValid = false; // time.h的tm结构体非要把月从0开始
        (it->nMask[1] & ((long long)0x01 << (pCurTime->tm_mday   ))) > 0 ? true : bValid = false;
        (it->nMask[2] & ((long long)0x01 << (pCurTime->tm_wday   ))) > 0 ? true : bValid = false;
        (it->nMask[3] & ((long long)0x01 << (pCurTime->tm_hour   ))) > 0 ? true : bValid = false;
        (it->nMask[4] & ((long long)0x01 << (pCurTime->tm_min    ))) > 0 ? true : bValid = false;
        (it->nMask[5] & ((long long)0x01 << (pCurTime->tm_sec    ))) > 0 ? true : bValid = false;
        if(bValid) return true;
    }

    return bValid;
}

} // namespace mtl