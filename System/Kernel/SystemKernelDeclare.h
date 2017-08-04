//-------------------------------------------------------------------------------
// SystemKernelDeclare.h
//
// @author
//     Millhaus.Chen @time 2016/08/31 11:40
//-------------------------------------------------------------------------------
#pragma once

#include <memory>

namespace S2{ namespace Kernel {

class Module;
typedef std::shared_ptr<Module> ModulePtr;
class Shell;
typedef std::shared_ptr<Shell> ShellPtr;

template <class T> class IFactory;
typedef IFactory<Module> ModuleFactory;
typedef std::shared_ptr<ModuleFactory> ModuleFactoryPtr;
typedef IFactory<Shell> ShellFactory;
typedef std::shared_ptr<ShellFactory> ShellFactoryPtr;

class Factories;
typedef std::shared_ptr<Factories> FactoriesPtr;

}}
