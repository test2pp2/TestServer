#include "stdafx.h"
#include "Packet.h"
#include "PacketHandler.h"
#include "UserRepository.h"
#include "User.h"
#include "Session.h"
#include "Database/SqlConnector.h"
#include "Database/Query.hpp"
#include "Utils/Locale.h"

using namespace Database;

namespace Content {

void LogInReq(std::shared_ptr<Session> session, JsonObject req) {
  BOOST_LOG_SEV(LOG_CONTENT, debug) << "called LogInReq";
  if (session->IsUser()) return;
  const std::string req_uid = req["uid"].string_value();
  const std::string name = req["name"].string_value();
  const int32_t age = req["age"].int_value();

  // user_id from db
  std::random_device rn;
  std::mt19937_64 random_device(rn());

  //< 2단계. 분포 설정 ( 정수 )
  const std::uniform_int_distribution<size_t> random_range(0, 10000);

  //< 3단계. 값 추출
  auto uid = random_range(random_device);

  auto user = User::Create(session, uid);

  Json res = Json::object({
    { "result", false },
    //{ "k2", 42.0 },
    //{ "k3", Json::array({ "a", 123.0, true, false, nullptr }) },
  });

  if (user == nullptr) {
    session->Send(LOGIN_RES, res);
    return;
  }

  std::string uuid = "asdasdad";
  const auto query = CreateQuery("sp_find_add_account_info", uuid);
  const auto query_result = ExecuteQuery(query);
  if (!query_result) {
    wprintf(L"db query 실패");
    return;
  }

  while (query_result->next()) {
    std::string result  = query_result->getString("username").c_str();
    auto a = Utils::Utf8ToWstring(result);
    auto b = Utils::StringToWstring(result);
    std::string character_id = query_result->getString("character_id").c_str();
    int iq = query_result->getInt("iq");
  }


  //session->set_user(user.get());


  if (!UserRepository::Instance().AddUser(uid, user)) {
    //session->set_user(nullptr);
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
