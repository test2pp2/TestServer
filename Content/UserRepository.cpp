#include "stdafx.h"
#include "UserRepository.h"
#include "Session.h"

namespace Content {

bool UserRepository::AddUser(size_t key, std::shared_ptr<User> user) {
  std::lock_guard<std::mutex> lock(lock_);
  const auto it = users_.find(key);
  if (it != users_.end()) {
    return false;
  }

  users_[key] = user;
  BOOST_LOG_SEV(LOG_CONTENT, debug) << "Add User size: " << users_.size();
  return true;
}

bool UserRepository::RemoveUser(size_t key) {
  std::lock_guard<std::mutex> lock(lock_);
  const auto it = users_.find(key);
  if (it == users_.end()) {
    return false;
  }
  users_.erase(key);
  BOOST_LOG_SEV(LOG_CONTENT, debug) << "Remove User size: " << users_.size();
  return true;
}

}
