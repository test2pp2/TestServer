#pragma once

namespace Content {

class Session;
class User;

class UserRepository {
public:
  UserRepository() = default;
  ~UserRepository() = default;

  static UserRepository& Instance() {
    static UserRepository instance;
    return instance;
  }

  bool AddUser(size_t uid, std::shared_ptr<User> user);
  bool RemoveUser(size_t uid);

private:
  std::map<size_t, std::shared_ptr<User>> users_;
  std::mutex lock_;
};

}
