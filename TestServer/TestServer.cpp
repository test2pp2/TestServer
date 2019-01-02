// TestServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Network/Server.h"
#include "Contents/Session.h"
#include "Contents/Packet.h"
#include "Database/SqlConnector.h"
#include "Utils/Minidump.h"

std::mutex lock;
std::condition_variable cv;
std::atomic_bool stop = false;

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

//#define UNUSED_REFERENCE_PARAMETER(x) boost::ignore_unused(x)

void SignalCallback(int value) {
  boost::ignore_unused(value);
  stop = true;
  cv.notify_all();
}

class MyObject {
public:
  bool mIsOpened = true;
};

int main() {
  Utils::Minidump dump;
  dump.Start();

  setlocale(LC_ALL, "");
  std::locale::global(std::locale(""));
  std::wcout.imbue(std::locale(""));

  // 서버 종료 ctrl + break
  std::signal(SIGBREAK, SignalCallback);

  const auto address = boost::asio::ip::make_address("127.0.0.1");
  const unsigned short port = 3000;
  const auto thread_count = 2;

  Contents::RegisterCallback();

  boost::asio::io_context io_context{ thread_count };
  auto server = std::make_shared<Network::Server<Contents::Session>>(io_context, boost::asio::ip::tcp::endpoint{ address, port });
  server->Run();

  std::vector<std::thread> thread_pool;
  thread_pool.reserve(thread_count);
  for (auto i = 0; i < thread_count; ++i)
    thread_pool.emplace_back(
      [&io_context] {
    InitalizeThreadLocal();
    std::cout << "run!" << std::endl;
    io_context.run();
  });

  std::unique_lock<std::mutex> lock_guard(lock);
  cv.wait(lock_guard, [] {
    if (!stop) {
      wprintf(L"메인 스레드 대기\n");
      return false;
    }
    wprintf(L"서버 종료 알림\n");
    return true;
  });

  io_context.stop();
  for (auto& worker_thread : thread_pool) {
    worker_thread.join();
  }

  dump.End();
  return EXIT_SUCCESS;
}
