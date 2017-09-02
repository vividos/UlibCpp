//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2017 Michael Fink
//
/// \file BoostAsio.hpp Boost.Asio include
//
#pragma once

// don't link Regex and DateTime
#define BOOST_DATE_TIME_NO_LIB
#define BOOST_REGEX_NO_LIB

// asio but no winsock init
#define BOOST_ASIO_DETAIL_IMPL_WINSOCK_INIT_IPP

// ignore /analyze warnings in Boost.Asio header files
#pragma warning(push)
#pragma warning(disable: 6001 6031 6255 6258 6386 6387)

// includes
#include <boost/asio.hpp>

#pragma warning(pop)

// asio but no winsock init

/// \cond false
inline void boost::asio::detail::winsock_init_base::startup(boost::asio::detail::winsock_init_base::data &, unsigned char, unsigned char) {}
inline void boost::asio::detail::winsock_init_base::cleanup(boost::asio::detail::winsock_init_base::data &) {}
inline void boost::asio::detail::winsock_init_base::throw_on_error(boost::asio::detail::winsock_init_base::data &) {}
/// \endcond
