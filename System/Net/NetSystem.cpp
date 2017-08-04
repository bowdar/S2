//-------------------------------------------------------------------------------
// NetSystem.cpp
//
// @author
//     Millhaus.Chen @time 2016/01/25 18:04
//-------------------------------------------------------------------------------
#include "NetSystem.h"

#include <thread>

using namespace S2::Net;
using namespace boost::asio;
using boost::system::error_code;

//io_service NetSystem::_ioService;

void NetSystem::run()
{
	/// @note Millhaus.Chen @time 2016/05/21 13:48
	///    Kernel里面跑的协程单例实例跟模块里面的不同，暂未解决问题
    ///    会非法退出
	static std::thread thr([=]
	{
		co_sched.RunLoop();		
	});
}