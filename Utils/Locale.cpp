#include "stdafx.h"
#include <codecvt>

namespace Locale {

static thread_local std::wstring_convert<std::codecvt_utf8<wchar_t>> cvt;

std::wstring Utf8ToWstring(const std::string& utf8_string)
{
  return cvt.from_bytes(utf8_string);
}

std::string WstringToUtf8(const std::wstring& wstring)
{
  return cvt.to_bytes(wstring);
}

std::wstring StringToWstring(const std::string& string)
{
  try
  {
    return cvt.from_bytes(string);
  }
  catch (std::range_error& e)
  {
    thread_local wchar_t buf[1024] = { 0 };
    if (string.length() >= sizeof(buf) - 1)
    {
      return{};
    }

    thread_local size_t converted = 0;
    mbstowcs_s(&converted, buf, string.c_str(), sizeof(buf));
    return buf;
  }
  catch (std::exception& e)
  {
    return{};
  }
}

std::string WstringToString(const std::wstring& wstring)
{
  try
  {
    return cvt.to_bytes(wstring);
  }
  catch (std::exception& e)
  {

    return{};
  }
}

}