// TestServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Network/Server.h"
#include "Contents/Session.h"
#include "Contents/Packet.h"

int main() {
  const auto address = boost::asio::ip::make_address("127.0.0.1");
  const unsigned short port = 3000;
  const auto thread_count = 2;

  Contents::RegisterCallback();

  boost::asio::io_context ioc{ thread_count };
  auto server = std::make_shared<Network::Server<Contents::Session>>(ioc, boost::asio::ip::tcp::endpoint{ address, port });
  server->Run();

  std::vector<std::thread> v;
  v.reserve(thread_count);
  for (auto i = 0; i < thread_count; ++i)
    v.emplace_back(
      [&ioc] {
    std::cout << "run!" << std::endl;
    ioc.run();
  });

  std::cout << "complete run" << std::endl;
  getchar();
  std::cout << "end run" << std::endl;
  //ioc.run();
  
  getchar();
  return EXIT_SUCCESS;
}
