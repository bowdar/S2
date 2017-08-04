//-------------------------------------------------------------------------------
// NetSystem.h
//
// @brief
//     
//
// @author
//     Millhaus.Chen @time 2016/01/25 18:01
//-------------------------------------------------------------------------------
#pragma once
#include <boost/noncopyable.hpp>

#include "NetServer.h"
#include "NetClient.h"

namespace S2 { namespace Net {

const int SOCKET_TCP_BUFFER = 16384;

class NetSystem : public boost::noncopyable
{
public:
//static boost::asio::io_service& getIoService()
//{
//	return _ioService;
//}

static void run();

private:
//static boost::asio::io_service _ioService;
};

}}