//-------------------------------------------------------------------------------
// SaturnSDKExport.h
//
// @author
//     Millhaus.Chen @time 2015/01/21 15:05
//-------------------------------------------------------------------------------
#pragma once

#if defined( DLL_EXPORTS )
#	if defined( __MINGW32__ )
#		define _DLL_EXPORTS __declspec( dllexport )
#	elif defined( _WIN32 )
#		define _DLL_EXPORTS __declspec( dllexport )
#   else
#       define _DLL_EXPORTS
#	endif
#else
#	if defined( __MINGW32__ )
#		define _DLL_EXPORTS
#	elif defined( _WIN32 )
#		define _DLL_EXPORTS __declspec( dllimport )
#   else
#       define _DLL_EXPORTS
#	endif
#endif
#define _DLL_PRIVATE