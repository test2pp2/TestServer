#pragma once

namespace Utils {
  std::wstring Utf8ToWstring(const std::string& utf8_string);
  std::string WstringToUtf8(const std::wstring& wstring);
  std::wstring StringToWstring(const std::string& string);
  std::string WstringToString(const std::wstring& wstring);
}
