#pragma once

namespace Network {

template<typename T>
class Server;

class BaseSession : public std::enable_shared_from_this<BaseSession> {
public:
explicit BaseSession(tcp::socket socket);
virtual ~BaseSession();
    
void Run();
void OnAccept(boost::system::error_code ec);

void DoRead();
virtual void OnRead(boost::system::error_code ec, std::size_t bytes_transferred);

void Write(const std::string& buffer);
void Close();
//void OnWrite(boost::system::error_code ec, std::size_t bytes_transferred);

virtual void OnConnect() = 0;
virtual void OnDisconnect() = 0;
virtual void OnRecv(const std::string& buffer) = 0;

private:
websocket::stream<tcp::socket> ws_;
boost::asio::strand<
    boost::asio::io_context::executor_type> strand_;
boost::beast::multi_buffer buffer_;
};

/*
template <typename T>
class Session : public BaseSession {
public:
  Session(Server<T>* server, tcp::socket socket) : BaseSession(std::move(socket)), server_(server) {  
  }
  virtual ~Session() {}

  void OnClose() const override;

  void Write(JsonObject object);

private:
  Server<T>* server_;
};
*/

}
