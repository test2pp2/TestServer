#pragma once

LONG WINAPI my_top_level_filter(__in PEXCEPTION_POINTERS pExceptionPointer);

namespace Utils {

class Minidump {

public:
  std::wstring get_dump_filename();
  Minidump();
  ~Minidump();

  static void Start();
  static void End();
private:
  std::string format_arg_list(const char *fmt, va_list args);
  std::string format_string(const char *fmt, ...);
  std::wstring s2ws(const std::string& s);

};

}