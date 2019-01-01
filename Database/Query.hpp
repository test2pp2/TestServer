#pragma once

#include <string>

namespace Database {

  template <typename... Args>
  std::string CreateQuery(const std::string& q, Args... args) {
    auto query = "call " + q + "(";
    Recurse(query, args...);

    return query;
  }

  template <typename Head>
  void Recurse(std::string& q, Head h) {
    BindParameter(q, h);
    q = q + ");";
  }

  template <typename Head, typename... Tail>
  void Recurse(std::string& q, Head h, Tail... t) {
    BindParameter(q, h);
    q = q + ",";
    Recurse(q, t...);
  }

  template <typename T>
  void BindParameter(std::string& q, T arg) {
    q = q + std::to_string(arg);
  }

  template <>
  inline void BindParameter(std::string& q, const char* arg) {
    q = q + "'" + arg + "'";
  }

  template <>
  inline void BindParameter(std::string& q, std::string arg) {
    q = q + "'" + arg + "'";
  }

} // Database