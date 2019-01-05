#pragma once

using namespace boost::log;
using namespace boost::log::trivial;

namespace Utils {

void InitializeLog();
bool AddFileLog(const std::string& fileter_name); 

}