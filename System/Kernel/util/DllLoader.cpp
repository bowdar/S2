/*-------------------------------------------------------------------------------
 * DllLoader.cpp
 *
 *      @author UNKNOWN       @time ----/--/-- --:--
 *      @modified Millhaus.Chen       @time 2014/06/24 17:55
 -------------------------------------------------------------------------------*/

#include "DllLoader.h"

#if defined(__MINGW32__) || defined(_WIN32)
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

using namespace S2::Kernel;



//-----------------------------------------------------------------------
DllLoader::DllLoader( const std::string & name ):_name(name),_hInst(NULL)
{
    _hInst = NULL;
}


DllLoader::DllLoader(const DllLoader & dl):_name(dl._name),_hInst(dl._hInst)
{
    
}
DllLoader& DllLoader::operator = (const DllLoader& dl)
{
    _name = dl._name;
    _hInst = dl._hInst;
    return (*this);
}



//-----------------------------------------------------------------------
DllLoader::~DllLoader()
{
}

//-----------------------------------------------------------------------
void DllLoader::load()
{
    std::string name;
#if defined(__MINGW32__)
    name = "lib" + _name + ".dll";
#elif defined(_WIN32)
	name = _name + ".dll";
#else
    name = "./lib" + _name + ".so";
#endif

    _hInst = (DYNLIB_HANDLE)DYNLIB_LOAD( name.c_str() );

    if(!_hInst)
    {
#if defined(__MINGW32__) || defined(_WIN32)
        printf("LoadLibraryEx error - %d\n", (int)GetLastError());
#else
        printf("dlopen error - %s\n", dlerror());
#endif
        throw 0;
    }
    
}

//-----------------------------------------------------------------------
void DllLoader::unload()
{
   
    if(DYNLIB_UNLOAD(_hInst))
    {
        throw NULL;
    }

}

//-----------------------------------------------------------------------
void* DllLoader::getSymbol(const std::string& name) const throw()
{
    return (void*)DYNLIB_GETSYM(_hInst, name.c_str());
}

