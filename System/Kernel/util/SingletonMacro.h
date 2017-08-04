//-------------------------------------------------------------------------------
// DllLoader.h
//
// @author
//     UNKNOWN @time ----/--/-- --:--
//
// @modified
//     Millhaus.Chen @time 2014/10/23 19:40
//-------------------------------------------------------------------------------
#pragma once


// @example
//   @file Sample.h
//     class CSample
//     {
//	       CSample();
//         ~CSample();
//         DECLARE_SINGLE_INSTANCE(CSample);
//     };
//
//   @file Sample.cpp
//     IMPLEMENT_SINGLE_INSTANCE(CSample);


#define DECLARE_SINGLE_INSTANCE(classname)						\
protected:														\
	static classname* m_pInstance;								\
public:															\
	static bool CreateInstance()								\
	{															\
		if(NULL == m_pInstance)									\
		m_pInstance = new classname();							\
		return m_pInstance != NULL;								\
	}															\
	static classname* GetInstancePtr()							\
	{															\
		if(m_pInstance == NULL)									\
		{														\
			CreateInstance();									\
		}														\
		return m_pInstance;										\
	}															\
	static classname& GetInstance()								\
	{															\
		if(m_pInstance == NULL)									\
		{														\
			CreateInstance();									\
		}														\
		return *m_pInstance;									\
	}															\
	static void DestroyInstance()								\
	{															\
		if(m_pInstance != NULL)									\
		{														\
			delete m_pInstance;									\
			m_pInstance = NULL;									\
		}														\
	}

#define IMPLEMENT_SINGLE_INSTANCE(classname)					\
	classname* classname::m_pInstance = NULL;
