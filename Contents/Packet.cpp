#include "stdafx.h"
#include "Packet.h"
#include "PacketHandler.h"
#include "UserRepository.h"
#include "Session.h"

namespace Contents {

void LogInReq(std::shared_ptr<Session> session, JsonObject req) {
  const std::string req_uid = req["uid"].string_value();
  const std::string name = req["name"].string_value();
  const int32_t age = req["age"].int_value();

  // user_id from db
  const uid user_id = 1087456;

  auto user = User::Create(session, user_id);

  Json res = Json::object({
    { "result", false },
    //{ "k2", 42.0 },
    //{ "k3", Json::array({ "a", 123.0, true, false, nullptr }) },
  });

  if (user == nullptr) {
    session->Send(LOGIN_RES, res);
    return;
  }

  session->set_user(user.get());

  if (!UserRepository::get().AddUser(user)) {
    session->set_user(nullptr);
    session->Send(LOGIN_RES, res);
    return;
  }

  res = Json::object({
    { "result", true },
    //{ "k2", 42.0 },
    //{ "k3", Json::array({ "a", 123.0, true, false, nullptr }) },
  });
  session->Send(LOGIN_RES, res);
}

void LogOutReq(std::shared_ptr<Session> session, JsonObject payload) {
  const int32_t age = payload["age"].int_value();
  int d = 20;
}

void RegisterCallback() {
  PacketkHandler::Register(LOGIN_REQ, &LogInReq);
  PacketkHandler::Register(LOGOUT_REQ, &LogOutReq);
}

}
