//-------------------------------------------------------------------------------
// Singleton.h
//
// @brief
//     这是一个最简洁比较完美的线程安全的单例模板（缺陷是没有好办法delete构造函数）
//
// @author
//     Millhaus.Chen @time 2015/08/26 10:58
//-------------------------------------------------------------------------------

#pragma once

namespace mtl {

// 模板形势
template <typename T>
class Singleton
{
public:
	template<typename... Args>
	static T& GetInstance(Args&&... args)
	{
		static T instance(std::forward<Args>(args)...);
		return instance;
	}
};

// 宏形势
#define  DECLARE_SINGLETON(ClassName)						\
public:													\
	static ClassName& GetInstance()						\
	{														\
		static ClassName instance;							\
		return instance;									\
	}														\
private:


} //namespace mtl