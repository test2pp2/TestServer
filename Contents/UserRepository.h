#pragma once

namespace Contents {

class Session;

class User {
public:
  static std::shared_ptr<User> Create(std::shared_ptr<Session> session, uid uid);

  User(std::shared_ptr<Session> session, uid uid);
  ~User();

  uid get_uid() const;

private:
  std::shared_ptr<Session> session_;
  uid uid_;
};

class UserRepository {
public:
  UserRepository() = default;
  ~UserRepository() = default;

  static UserRepository& get() {
    static UserRepository instance;
    return instance;
  }

  bool AddUser(std::shared_ptr<User> user);
  bool RemoveUser(std::shared_ptr<User> user);

private:
  std::map<uid, std::shared_ptr<User>> users_;
  std::mutex lock_;
};

}
