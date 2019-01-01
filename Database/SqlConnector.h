#pragma once

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

namespace Database {

  bool TryConnect(const std::string& hostname, int port,
    const std::string& username, const std::string& password, const std::string& schema);

  std::shared_ptr<sql::Connection> connection();
  std::shared_ptr<sql::Statement>  statement();

  std::unique_ptr<sql::ResultSet> ExecuteQuery(const std::string& query);
  void Execute(const std::string& query);

} // Database
