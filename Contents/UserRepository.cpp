#include "stdafx.h"
#include "UserRepository.h"
#include "Session.h"

namespace Contents {

std::shared_ptr<User> User::Create(std::shared_ptr<Session> session, uid uid) {
  return std::make_unique<User>(session, uid);
}

User::User(std::shared_ptr<Session> session, uid uid) : session_(session), uid_(uid) {
}

User::~User() {
}

uid User::get_uid() const {
  return uid_;
}

bool UserRepository::AddUser(std::shared_ptr<User> user) {
  std::lock_guard<std::mutex> lock(lock_);
  const auto key = user->get_uid();
  const auto it = users_.find(key);
  if (it != users_.end()) {
    return false;
  }

  users_[key] = user;

  return true;
}

bool UserRepository::RemoveUser(std::shared_ptr<User> user) {
  std::lock_guard<std::mutex> lock(lock_);
  const auto key = user->get_uid();
  const auto it = users_.find(key);
  if (it == users_.end()) {
    return false;
  }
  users_.erase(key);
  return true;
}

}
