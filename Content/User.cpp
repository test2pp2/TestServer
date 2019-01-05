#include "stdafx.h"
#include "User.h"
#include "UserRepository.h"
#include "Session.h"

namespace Content {

std::shared_ptr<User> User::Create(std::shared_ptr<Session> session, size_t uid) {
  auto user = std::make_shared<User>(session, uid);
  if (user == nullptr) {
    return nullptr;
  }
  session->set_user(user);
  return user;
}

User::User(std::shared_ptr<Session> session, size_t uid) : session_(session), uid_(uid) {
}

User::~User() {
}

size_t User::uid() const {
  return uid_;
}

void User::OnDisconnected() const {
  UserRepository::Instance().RemoveUser(uid_);
}

}
