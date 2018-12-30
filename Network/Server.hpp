#include "stdafx.h"
#include "Session.h"
#include "Server.h"

namespace Network {
  
template<typename T>
Server<T>::Server(boost::asio::io_context& ioc, tcp::endpoint endpoint) :
  acceptor_(ioc),
  socket_(ioc) {

  boost::system::error_code ec;
  acceptor_.open(endpoint.protocol(), ec);
  if (ec) {
    Fail(ec, "open");
    return;
  }

  acceptor_.bind(endpoint, ec);
  if (ec) {
    Fail(ec, "bind");
    return;
  }

  acceptor_.listen(
    boost::asio::socket_base::max_listen_connections, ec);
  if (ec) {
    Fail(ec, "listen");
    return;
  }
}

template<typename T>
Server<T>::~Server() {
}

template<typename T>
void Server<T>::Run() {
  if (!acceptor_.is_open()) {
    assert(false);
    return;
  }
  DoAccept();
}

template<typename T>
void Server<T>::DoAccept() {
  acceptor_.async_accept(
    socket_,
    std::bind(
      &Server::OnAccept,
      shared_from_this(),
      std::placeholders::_1));
}

template<typename T>
void Server<T>::OnAccept(boost::system::error_code ec) {
  if (ec) {
    //fail(ec, "accept");
  } else {
    std::lock_guard<std::mutex> lock(mutex_);
    auto session = std::make_shared<T>(std::move(socket_));
    //sessions_.insert(std::make_pair(session.get(), session));
    session->Run();
  }

  DoAccept();
}

} // Network