//
// ulib - a collection of useful classes
// Copyright (C) 2008-2012,2017,2020,2023 Michael Fink
//
/// \file IoCContainer.hpp Inversion of Control container
//
#pragma once

#include <string>
#include <map>
#include <functional>
#include <any>

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

   /// registers reference for class
   template <typename TClass>
   void Register(std::reference_wrapper<TClass> ref)
   {
      std::string name = typeid(TClass).raw_name();
      m_mapAllInstances[name] = ref;
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
      // nothing to do
   }

   IoCContainer(const IoCContainer&) = delete;              ///< removed copy ctor
   IoCContainer(IoCContainer&&) = delete;                   ///< removed move ctor
   IoCContainer& operator=(const IoCContainer&) = delete;   ///< removed copy assign operator
   IoCContainer& operator=(IoCContainer&&) = delete;        ///< removed move assign operator

private:
   /// instance map type
   typedef std::map<std::string, std::any> T_mapAllInstances;

   /// instance map
   T_mapAllInstances m_mapAllInstances;
};
