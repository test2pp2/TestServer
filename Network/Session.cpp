#include "stdafx.h"
#include "Session.h"
#include "Server.h"

namespace Network {

BaseSession::BaseSession(tcp::socket socket) : ws_(std::move(socket)), strand_(ws_.get_executor()) {
}

BaseSession::~BaseSession() {
  std::cout << "dtor BaseSession called" << std::endl;
}

void BaseSession::Run() {
  ws_.async_accept(
    boost::asio::bind_executor(
      strand_,
      std::bind(
        &BaseSession::OnAccept,
        shared_from_this(),
        std::placeholders::_1)));
}

void BaseSession::OnAccept(boost::system::error_code ec) {
  if (ec) {
    return Network::Fail(ec, "accept");
  }

  OnConnect();
  // Read a message
  DoRead();
}

void BaseSession::DoRead() {
  ws_.async_read(
    buffer_,
    boost::asio::bind_executor(
      strand_,
      std::bind(
        &BaseSession::OnRead,
        shared_from_this(),
        std::placeholders::_1,
        std::placeholders::_2)));
}

void BaseSession::OnRead(boost::system::error_code ec, std::size_t bytes_transferred) {
  std::cout << "called OnRead" << std::endl;
  boost::ignore_unused(bytes_transferred);

  // This indicates that the session was closed
  if (ec == websocket::error::closed) {
    OnDisconnect();
    return;
  }

  if (ec) {
    OnDisconnect();
    Network::Fail(ec, "read");
    ws_.close(websocket::close_code::normal, ec);
    return;
  }

  auto buf = boost::beast::buffers_to_string(buffer_.data());
  
  OnRecv(buf);

  buffer_.consume(buffer_.size());
  DoRead();
  
  /*
  // Echo the message
  ws_.text(ws_.got_text());
  ws_.async_write(
    buffer_.data(),
    boost::asio::bind_executor(
      strand_,
      std::bind(
        &BaseSession::OnWrite,
        shared_from_this(),
        std::placeholders::_1,
        std::placeholders::_2)));
        */
}

void BaseSession::Write(const std::string& buffer) {
  try {
    auto size = ws_.write(boost::asio::buffer(buffer));
    assert(size == buffer.size());

  } catch (boost::system::system_error const& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }  catch (std::exception const& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

void BaseSession::Close() {
  //boost::system::error_code ec;
  ws_.close(websocket::close_code::normal);
}
/*
void BaseSession::OnWrite(boost::system::error_code ec, std::size_t bytes_transferred) {
  boost::ignore_unused(bytes_transferred);

  if (ec) {
    return Fail(ec, "write");
  }

  buffer_.consume(buffer_.size());
  DoRead();
}
*/

/*
template <typename T>
void Session<T>::Write(JsonObject object) {
  BaseSession::Write(object.dump());
}

template <typename T>
void Session<T>::OnClose() const {
  server_->OnDisconnect(this);
  std::cout << "OnClose called" << std::endl;
}
*/

} // Network