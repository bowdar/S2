//-------------------------------------------------------------------------------
// Task.h
//
// @author
//     Millhaus.Chen @time 2016/06/23 14:04 
//-------------------------------------------------------------------------------
#pragma once

#include "util/DllExport.h"

namespace S2{ namespace Kernel {

template<typename T>
class _DLL_EXPORTS Task;

template<typename R, typename...Args>
class _DLL_EXPORTS Task<R(Args...)>
{
	std::function<R(Args...)> m_fn;

public:
    typedef std::shared_ptr<Task<R(Args...)>> Ptr;
	typedef R return_type;

	template<typename F>
	auto Then(F&& f) -> Task<typename std::result_of<F(R)>::type(Args...)>
	{
		typedef typename std::result_of<F(R)>::type ReturnType;
		auto func = std::move(m_fn);
		return Task<ReturnType(Args...)>([func, &f](Args&&... args)
		{
			std::future<R> lastf = std::async(func, std::forward<Args>(args)...);
			return std::async(f, lastf.get()).get();
		});
	}

	Task(std::function<R(Args...)>&& f) :m_fn(std::move(f)){}
	Task(std::function<R(Args...)>& f) :m_fn(f){}

	void Wait()
	{
		std::async(m_fn).wait();
	}

	template<typename... Args>
	R Get(Args&&... args)
	{
		return std::async(m_fn, std::forward<Args>(args)...).get();
	}

	std::shared_future<R> Run()
	{
		return std::async(m_fn);
	}

    std::shared_future<R> RunForever()
	{
		return std::async(std::bind(this, &Task<R(Args...)>::ForeverThread));
	}

    void Terminate()
	{
	    m_terminate = true;
	}

private:
    void ForeverThread()
	{
        while(!m_terminate)
        {
            m_fn();
        }
	}

    std::atomic<bool> m_terminate = false;
};

}}
