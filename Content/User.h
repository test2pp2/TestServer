#pragma once

namespace Content {

class Session;

class User {
public:
  static std::shared_ptr<User> Create(std::shared_ptr<Session> session, size_t uid);

  User(std::shared_ptr<Session> session, size_t uid);
  ~User();

  void OnDisconnected() const;
 
  size_t uid() const;

private:
  std::shared_ptr<Session> session_;
  size_t uid_;
};

}