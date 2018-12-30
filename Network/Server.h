#pragma once

namespace Network {

static void Fail(boost::system::error_code ec, char const* what) {
  std::cerr << what << ": " << ec.message() << "\n";
}

template <typename T>
class Server : public std::enable_shared_from_this<Server<T>> {
public:

  Server(boost::asio::io_context& ioc, tcp::endpoint endpoint);
  ~Server();

  void Run();
  void DoAccept();
  void OnAccept(boost::system::error_code ec);

private:
  tcp::acceptor acceptor_;
  tcp::socket socket_;

  //std::map<const Session<T>*, std::shared_ptr<Session<T>>> sessions_;

  std::mutex mutex_;
};

}

#include "Server.hpp"