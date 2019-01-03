#pragma once

#include "Network/Session.h"
#include "Opcode.h"

namespace Content {

class User;

class Session : public ::Network::BaseSession {
public:
  Session(tcp::socket socket);
  virtual ~Session();

  //void OnAccept(boost::system::error_code ec) override;
  void Send(Opcode opcode, const JsonObject& payload);

  void OnConnect() override;
  void OnDisconnect() override;
  void OnRecv(const std::string& buffer) override;

  void set_user(User* user);
private:
  void Send(const JsonObject& buffer);

  User* user_;
};

}