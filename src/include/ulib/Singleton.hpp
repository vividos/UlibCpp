//
// ulib - a collection of useful classes
// Copyright (C) 2008-2014,2017 Michael Fink
//
/// \file Singleton.hpp Singleton template class
//
#pragma once

// includes
#include <atomic>
#include <mutex>

/// \brief thread-safe Double Check Singleton Pattern implementation
/// \note use the IMPLEMENT_SINGLETON macro to implement singleton instance in a .cpp file
/// \see http://www.chaoticmind.net/~hcb/projects/boost.atomic/doc/atomic/usage_examples.html#boost_atomic.usage_examples.singleton
template <typename T>
class Singleton
{
public:
   /// \brief returns instance of singleton
   /// ensures that singleton is only created once
   static T& Instance()
   {
      T* tmp = m_instance.load(std::memory_order_consume);
      if (!tmp)
      {
         std::mutex::scoped_lock l(m_instantiationMutex);
         tmp = m_instance.load(std::memory_order_consume);
         if (!tmp)
         {
            tmp = new T;
            m_instance.store(tmp, std::memory_order_release);
         }
      }
      return *tmp;
   }

private:
   /// instance of object
   static std::atomic<T*> m_instance;

   /// mutex to protect singleton creation
   static std::recursive_mutex m_instantiationMutex;
};

/// use this macro to implement a singleton instance in a .cpp file
#define IMPLEMENT_SINGLETON(T) \
   std::atomic<T*> Singleton<T>::m_instance(0); \
   std::recursive_mutex Singleton<T>::m_instantiationMutex;
