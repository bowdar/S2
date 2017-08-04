//-------------------------------------------------------------------------------
// NetServer.h
//
// @author
//     Millhaus.Chen @time 2016/01/25 16:06
//
// @modified
//     Millhaus.Chen @time 2017/03/03 11:16
//-------------------------------------------------------------------------------
#pragma once

#include <memory>
#include <functional>

#include <boost/noncopyable.hpp>
#include <boost/any.hpp>
#include <boost/asio.hpp>

#include <libgo/coroutine.h>

#include "NetPackage.h"

namespace S2 { namespace Net {

/// 被NetServer或者NetServer使用，Peer单词来自Photon
/// 一个NetServer可以包含多个Peer，比如一个Client连接了多个remote
class ServerPeer
{
    friend class NetServer;
	using asionet = boost::asio::ip::tcp;

public:
	ServerPeer(boost::asio::io_service& ioService) :
               socket(ioService),
               acceptor(ioService, asionet::endpoint(boost::asio::ip::address::from_string("192.168.159.132"), 1741)){}

    asionet::socket& getSocket() {return socket; }

public:
	std::string strRemote;
	unsigned char bufferReceive[BUFFER_LENGTH];
	unsigned char bufferBackup[BUFFER_LENGTH]; /// 粘包时的备份缓冲
	size_t receiveSize = 0;
	size_t backupSize = 0;
	int reqRunning = 0; /// 发送了req还没等到ack的数量
	int reqWaiting = 0; /// 待判断当前ack是不是自己req的数量

    asionet::endpoint endpoint;
    asionet::socket socket;
    asionet::acceptor acceptor;

protected:
    /// Send and receive buffer of interface
    unsigned char reqBuffer[BUFFER_LENGTH];
    unsigned char rspBuffer[BUFFER_LENGTH];
};

/// 要作为客户端的类可以包含NetServer，或者派生自NetServer
class NetServer
{
public:
	void receiveLoop(S2::Net::ServerPeer*);

	/// 发送消息
	bool send(S2::Net::ServerPeer& peer,
                    const unsigned char* message,
                    size_t msgSize,
                    u_int32_t msgID);

	/// 发送请求获取返回消息
    unsigned char* request(S2::Net::ServerPeer& peer,
                    const unsigned char* reqMsg,
                    size_t reqSize,
                    size_t& rspSize,
                    u_int32_t msgID);

    /// Callback when received non-req message
    void onReceive(const unsigned char* message,
                    size_t msgSize,
                    u_int32_t msgID);

	boost::asio::io_service& getIOService() { return m_ioService; }

protected:
	boost::asio::io_service m_ioService;
};









}}