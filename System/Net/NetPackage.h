//-------------------------------------------------------------------------------
// Netpackage.h
//
// @author
//     Millhaus.Chen @time 2017/03/02 14:51
//-------------------------------------------------------------------------------
#pragma once

#include <cstdint>

namespace S2 { namespace Net {

const int BUFFER_LENGTH = 16384;
const int REQ_TIME_OUT = 0;			/// time out，0 is no time out

/// Net package head
struct SNetHead
{
    u_int32_t   id;
    u_int16_t   size;
    u_int8_t    version;           /// message type
    u_int8_t    cc;                /// checkcode
};

}}