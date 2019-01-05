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

  void set_user(std::shared_ptr<User> user);
  bool IsUser() const {
    auto user = user_.lock();
    if (user == nullptr) return false;
    return true;
  }

private:
  void Send(const JsonObject& buffer);

  std::weak_ptr<User> user_;
};

}