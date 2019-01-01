// TestServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Network/Server.h"
#include "Contents/Session.h"
#include "Contents/Packet.h"
#include "Database/SqlConnector.h"

//#pragma comment(lib,"Advapi32")

bool CreateSqlConnection(
  const std::string& hostname, 
  int port, 
  const std::string& username, 
  const std::string& password,
  const std::string& schema
) {
  bool result = true;
  try {
    result = Database::TryConnect(hostname, port, username, password, schema);
    wprintf(L"sql 접속 성공\n");
  } catch (sql::SQLException &e) {
    printf("error: %s\n", e.what());
    result = false;
    //cout << " (MySQL error code: " << e.getErrorCode();
    //cout << ", SQLState: " << e.getSQLState() << " )" << endl;
  }
  return result;
}

void InitalizeThreadLocal() {
  std::cout << "called InitalizeThreadLocal" << std::endl;
  if (!CreateSqlConnection("127.0.0.1", 3306, "root", "aaaa1111", "test")) {
    assert(false);
  }
  std::cout << "successfully connect to sql server" << std::endl;
}

int main() {
  setlocale(LC_ALL, "");

  const auto address = boost::asio::ip::make_address("127.0.0.1");
  const unsigned short port = 3006;
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
    InitalizeThreadLocal();
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
