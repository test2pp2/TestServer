#include "stdafx.h"
#include "SqlConnector.h"

namespace Database {

static sql::Driver* g_driver = nullptr;

thread_local std::shared_ptr<sql::Connection> local_connection;
thread_local std::shared_ptr<sql::Statement>  local_statement;
thread_local std::unique_ptr<sql::ResultSet>  local_res;

static std::mutex driver_lock;

bool CreateStatement() {
  local_statement.reset(local_connection->createStatement());
  if (local_statement == nullptr) {
    return false;
  }
  return true;
}

bool TryConnect(
  const std::string& hostname, 
  int port,
  const std::string& username, 
  const std::string& password, 
  const std::string& schema) {
  std::lock_guard<std::mutex> lock(driver_lock);

  if (!g_driver) {
    g_driver = get_driver_instance();
  }

  /*
  sql::ConnectOptionsMap connection_properties;
  connection_properties["hostname"] = hostname.c_str();
  connection_properties["port"] = port;
  connection_properties["userName"] = username.c_str();
  connection_properties["password"] = password.c_str();
  connection_properties["schema"] = schema.c_str();
  connection_properties["client_multi_statements"] = true;
  connection_properties["client_multi_results"] = true;
  local_connection.reset(g_driver->connect(connection_properties));
  */  
  
  const std::string url = hostname + ":" + std::to_string(port);
  local_connection.reset(
    g_driver->connect(url.c_str(), username.c_str(), password.c_str())
  );

  if(local_connection == nullptr) {
    return false;
  }

  local_connection->setSchema(schema.c_str());
  //local_connection->setClientOption("client_multi_statements", reinterpret_cast<void*>(true));
  //local_connection->setClientOption("client_multi_results", reinterpret_cast<void*>(true));
  
  return CreateStatement();
}

std::shared_ptr<sql::Connection> connection() {
  return local_connection;
}

std::shared_ptr<sql::Statement> statement() {
  return local_statement;
}

std::unique_ptr<sql::ResultSet> ExecuteQuery(const std::string& query) {
  try {
    local_res.reset(local_statement->executeQuery(query));
    // result가 2개가 있는 쿼리는 따로 처리해야할듯
    while (local_statement->getMoreResults()) {}
    return std::move(local_res);
  } catch (sql::SQLException &e) {
    printf("%s\n", e.what());
    return nullptr;
  }
}

void Execute(const std::string& query) {
  try {
    local_statement->execute(query);
  } catch (sql::SQLException &e) {
    printf("%s\n", e.what());
  }
}

} // Database

