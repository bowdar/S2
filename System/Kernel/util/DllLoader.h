//-------------------------------------------------------------------------------
// DllLoader.h
//
// @author
//     UNKNOWN @time ----/--/-- --:--
//
// @modified
//     Millhaus.Chen @time 2014/06/24 17:54
//     Millhaus.Chen @time 2016/08/31 14:10 cross platform
//-------------------------------------------------------------------------------
#pragma once

#include <string>
#include <memory>
#include <boost/noncopyable.hpp>

#if defined(__MINGW32__) || defined(_WIN32)
    #define DYNLIB_HANDLE hInstance
    #define DYNLIB_LOAD( a ) LoadLibraryEx( a, NULL, LOAD_WITH_ALTERED_SEARCH_PATH )
    #define DYNLIB_GETSYM( a, b ) GetProcAddress( a, b )
    #define DYNLIB_UNLOAD( a ) !FreeLibrary( a )
    struct HINSTANCE__;
    typedef struct HINSTANCE__* hInstance;
#else
    #define DYNLIB_HANDLE void*
    #define DYNLIB_LOAD( a ) dlopen( a, RTLD_LAZY )
    #define DYNLIB_GETSYM( a, b ) dlsym( a, b )
    #define DYNLIB_UNLOAD( a ) dlclose( a )
#endif


namespace S2 { namespace Kernel {

class DllLoader : public boost::noncopyable
{
public:
    typedef std::shared_ptr<DllLoader> Ptr;

    /** Default constructor - used by DLLManager.
    @warning
        Do not call directly
    */
    DllLoader( const std::string & name );

    DllLoader(const DllLoader & dl);
    DllLoader& operator = (const DllLoader& dl);
    void swap(DllLoader& dl) throw();

    ~DllLoader(void);

    void load(void);

    void unload(void);

    const std::string & getName(void) const { return _name; }

    /** Returns the address of the given symbol from the loaded library.
    @param
        name The name of the symbol to search for
    @returns
        If the function succeeds, the returned value is a handle to
        the symbol.
    @par
        If the function fails, the returned value is <b>NULL</b>.
    */
    void* getSymbol( const std::string & name ) const throw();

private:
    std::string _name;

    /** Handle to the loaded library. */
    DYNLIB_HANDLE _hInst;

};

}}
