#include "stdafx.h"
#include "Session.h"
#include "PacketHandler.h"

namespace Contents {

Session::Session(tcp::socket socket) : BaseSession(std::move(socket)), user_(nullptr) {
}

Session::~Session() {
  std::cout << "dtor Session called" << std::endl;
}

/*
void Session::OnAccept(boost::system::error_code ec) {
  BaseSession::OnAccept(ec);
}
*/

void Session::Send(Opcode opcode, const JsonObject& payload) {
  const Json res = Json::object({
    { "opcode", opcode },
    { "payload", payload }
  });
  Send(res);
}

void Session::OnConnect() {
  std::cout << "called OnConnect: " << std::endl;
}
 
void Session::OnDisconnect() {
  std::cout << "called OnDisconnect: " << std::endl;
}

void Session::OnRecv(const std::string& buffer) {
  std::cout << "called OnRecv: " << buffer << std::endl;

  std::string error;
  const auto json = Json::parse(buffer, error);
  if (!error.empty()) {
    return;
  }

  auto self = std::dynamic_pointer_cast<Session>(shared_from_this());
  const int32_t opcode = json["opcode"].int_value();
  const auto payload = json["payload"];
  //auto test_age = payload["age"].int_value();
  bool result = PacketkHandler::Call(self, static_cast<Opcode>(opcode), payload);
  if (!result) {
    Close();
  }
}

void Session::set_user(User* user) {
  user_ = user;
}

void Session::Send(const JsonObject& json) {
  Write(json.dump());
}

}
