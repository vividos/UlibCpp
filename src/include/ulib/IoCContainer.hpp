//
// ulib - a collection of useful classes
// Copyright (C) 2008-2012,2017 Michael Fink
//
/// \file IoCContainer.hpp Inversion of Control container
//
#pragma once

// IoCContainer is only available when compiling for C++17
#if _HAS_CXX17

#include <string>
#include <map>
#include <any>
#include <type_traits>

/// inversion of control container
class IoCContainer
{
public:
   /// returns current instance of IoC container
   static IoCContainer& Current()
   {
      static IoCContainer current;
      return current;
   }

   /// registers ref for class
   template <typename TClass>
   void Register(std::reference_wrapper<TClass> ref)
   {
      std::string name = typeid(TClass).raw_name();
      m_mapAllInstances.insert(std::make_pair(name, ref));
   }

   /// resolves class to object
   template <typename TInterface>
   TInterface& Resolve()
   {
      std::string name = typeid(TInterface).raw_name();

      T_mapAllInstances::iterator iter = m_mapAllInstances.find(name);

      if (iter == m_mapAllInstances.end())
         throw std::runtime_error(std::string("class not registered: ") + typeid(TInterface).name());

      std::reference_wrapper<TInterface> ref =
         std::any_cast<std::reference_wrapper<TInterface>>(iter->second);

      return ref.get();
   }

private:
   /// default ctor
   IoCContainer()
   {
   }

   IoCContainer(const IoCContainer&) = delete;              ///< removed copy ctor
   IoCContainer& operator=(const IoCContainer&) = delete;   ///< removed assign op

private:
   /// instance map type
   typedef std::map<std::string, std::any> T_mapAllInstances;

   /// instance map
   T_mapAllInstances m_mapAllInstances;
};

#endif // _HAS_CXX17
