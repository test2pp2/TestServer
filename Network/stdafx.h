// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <mutex>

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/bind_executor.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>

#include "Utils/json11.hpp"

#include "ConstNetwork.h"

namespace websocket = boost::beast::websocket;

using tcp = boost::asio::ip::tcp;
using JsonObject = json11::Json;
using namespace boost::log;
using namespace boost::log::trivial;

namespace Network {

static sources::severity_channel_logger_mt<trivial::severity_level, std::string> log_network(
  keywords::channel = LogNetwork
);

} // Network

#define LOG_NETWORK Network::log_network
