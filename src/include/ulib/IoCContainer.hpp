//
// ulib - a collection of useful classes
// Copyright (C) 2008-2012,2017,2020 Michael Fink
//
/// \file IoCContainer.hpp Inversion of Control container
//
#pragma once

#include <string>
#include <map>
#pragma warning(push)
#pragma warning(disable: 26439 26451 26812)
#include <boost/any.hpp>
#include <boost/ref.hpp>
#pragma warning(pop)

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
   void Register(boost::reference_wrapper<TClass> ref)
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
         boost::any_cast<boost::reference_wrapper<TInterface>>(iter->second);

      return ref.get();
   }

private:
   /// default ctor
   IoCContainer()
   {
      // nothing to do
   }

   IoCContainer(const IoCContainer&) = delete;              ///< removed copy ctor
   IoCContainer& operator=(const IoCContainer&) = delete;   ///< removed assign op

private:
   /// instance map type
   typedef std::map<std::string, boost::any> T_mapAllInstances;

   /// instance map
   T_mapAllInstances m_mapAllInstances;
};
