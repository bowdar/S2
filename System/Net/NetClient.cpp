//-------------------------------------------------------------------------------
// NetClient.cpp
//
// @author
//     Millhaus.Chen @time 2016/01/25 16:46
//
// @modified
//     Millhaus.Chen @time 2017/03/03 13:14
//-------------------------------------------------------------------------------
#include "NetSystem.h"
#include "NetClient.h"

#include <string>

using namespace std;
using namespace S2::Net;
using namespace boost::asio;
using asio_error = boost::system::error_code;


///   该接收消息循环每接收到一个消息会等待所有running的请求都判断一次是否是自己的
/// 等待的消息，如果都不是才会继续接收下一个消息。每当接收了新消息都认为当前还在
/// running的所有req都没有判断过这个新消息（这句解释是不是多余:)）。
void NetClient::receiveLoop(ClientPeer* pPeer)
{
	if(pPeer == nullptr) return;

	asio_error ec;
	while (pPeer->getSocket().is_open())
	{
        memset(pPeer->bufferReceive, 0, BUFFER_LENGTH);
        if(pPeer->backupSize > 0) /// 备份缓冲如果还有数据先将备份缓冲投递干净再读取网络数据
        {
            memcpy(pPeer->bufferReceive, pPeer->bufferBackup, pPeer->backupSize);
            pPeer->receiveSize = pPeer->backupSize;
            pPeer->backupSize = 0;
        }
        else
        {
            pPeer->receiveSize = pPeer->getSocket().read_some(buffer(pPeer->bufferReceive, BUFFER_LENGTH), ec);
            if (ec)
            {
                printf("Receice error ! %s remote: %s port:%d\n", ec.message().c_str(), pPeer->strRemote.c_str(), pPeer->endpoint.port());
                break;
            }
        }
        SNetHead* pHead = (SNetHead*)pPeer->bufferReceive;
        if(pHead->size <= 0)
        {
            pPeer->backupSize = 0;
            pPeer->receiveSize = 0;
        }
        /// 粘包处理
        if(pHead->size < pPeer->receiveSize)
        {
            pPeer->backupSize = pPeer->receiveSize - pHead->size;
            pPeer->receiveSize = pHead->size;
            memset(pPeer->bufferBackup, 0, BUFFER_LENGTH);
            memcpy(pPeer->bufferBackup, pPeer->bufferReceive + pHead->size, pPeer->backupSize);
        }
        /// 拆包处理
        while(pHead->size > pPeer->receiveSize)
        {
            pPeer->receiveSize += pPeer->getSocket().read_some(buffer(pPeer->bufferReceive + pPeer->receiveSize, BUFFER_LENGTH - pPeer->receiveSize), ec);
            if(ec)
            {
                printf("receice error ! %s remote: %s\n", ec.message().c_str(), pPeer->strRemote.c_str());
                break;
            }
        }

        auto iter = pPeer->reqChannel.find(pHead->id);
        if(iter != pPeer->reqChannel.end())
        {
            //iter->second << pPeer->receiveSize; /// 向频道中填入值，唤醒等待的协程
            auto channel = iter->second;
            pPeer->reqChannel.erase(iter);
            channel >> pPeer->receiveSize;
            pPeer->receiveSize = 0;
        }
        else
        {
            onReceive(pPeer->bufferReceive + pHead->size, pHead->size, pHead->id);
        }
	}
}

bool NetClient::send(S2::Net::ClientPeer& peer,
                     const unsigned char* message,
                     size_t msgSize,
                     u_int32_t msgID)
{
	try {

	if(!peer.getSocket().is_open())
	{
		return false;
	}

    memset(peer.reqBuffer, 0, BUFFER_LENGTH);
	SNetHead* pSendHead = (SNetHead*)peer.reqBuffer;
	pSendHead->size = sizeof(SNetHead);

    pSendHead->id = msgID;
    pSendHead->size = sizeof(SNetHead) + msgSize;
    memcpy(peer.reqBuffer + sizeof(SNetHead), message, msgSize);

	} catch(error_code ec)
	{
		printf("sendMsg error: %s\n", ec.message().c_str());
	}
	catch(boost::system::system_error ec)
	{
		printf("sendMsg error, code: %d, what: %s\n", ec.code().value(), ec.what());
	}

	return true;
}


unsigned char* NetClient::request(S2::Net::ClientPeer& peer,
                       const unsigned char* reqMsg,
                       size_t reqSize,
                       size_t& rspSize,
                       u_int32_t msgID)
{
    try {

    if(!peer.getSocket().is_open())
    {
        return nullptr;
    }

    memset(peer.reqBuffer, 0, BUFFER_LENGTH);
    memset(peer.rspBuffer, 0, BUFFER_LENGTH);
    memset(peer.bufferReceive, 0, BUFFER_LENGTH);

    SNetHead* pReqHead = (SNetHead*)peer.reqBuffer;
    pReqHead->id = (short)msgID;
    pReqHead->size = sizeof(SNetHead) + reqSize;
    memcpy(peer.reqBuffer + sizeof(SNetHead), reqMsg, reqSize);

    peer.getSocket().write_some(buffer(peer.reqBuffer, pReqHead->size));

    //peer.reqChannel[COMBIN_WHMSG(nAckID, nAckSubID)] >> receiveSize; /// 发送请求消息后，利用频道阻塞协程
    peer.reqChannel[msgID] << peer.receiveSize;

    if(peer.receiveSize > BUFFER_LENGTH) peer.receiveSize = BUFFER_LENGTH;

    memcpy(peer.rspBuffer, peer.bufferReceive + sizeof(SNetHead), peer.receiveSize - sizeof(SNetHead));

    memset(peer.bufferReceive, 0, BUFFER_LENGTH);
    rspSize = peer.receiveSize;
    //peer.receiveSize = 0;

    } catch(error_code ec)
    {
        printf("error: %s port:%d\n", ec.message().c_str(), peer.endpoint.port());
    }
    catch(boost::system::system_error ec)
    {
        printf("error, code: %d, what: %s port:%d\n", ec.code().value(), ec.what(), peer.endpoint.port());
    }

    return peer.rspBuffer;
}

void NetClient::onReceive(const unsigned char* message,
                          size_t msgSize,
                          u_int32_t msgID)
{
    // @test
    printf("Received message id:%d, \"%s\"\n", msgID, message);
}