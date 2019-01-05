#include "stdafx.h"
#include "Log.h"

namespace Utils {

BOOST_LOG_ATTRIBUTE_KEYWORD(a_channel, "Channel", std::string)

static formatter log_format;

void InitializeLog() {
  add_common_attributes();
  core::get()->add_global_attribute(
    "Scope", 
    attributes::named_scope()
  );
  core::get()->set_filter(
    trivial::severity >= trivial::trace
  );

  auto format_timestamp = boost::log::expressions::
    format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f");
  auto format_thread_id = boost::log::expressions::
    attr<attributes::current_thread_id::value_type>("ThreadID");
  auto format_severity = boost::log::expressions::
    attr<trivial::severity_level>("Severity");
  auto format_scope = expressions::format_named_scope("Scope",
    keywords::format = "%n(%f:%l)",
    keywords::iteration = expressions::reverse,
    keywords::depth = 2);

  log_format =
    expressions::format("[%1%] (%2%) [%3%] [%4%] %5%")
    % format_timestamp % format_thread_id % format_severity % format_scope
    % expressions::smessage;

  auto console_sink = add_console_log();
  console_sink->set_formatter(log_format);
}

bool AddFileLog(const std::string& fileter_name) {
  auto sink = add_file_log(
    keywords::file_name = "Log/" + fileter_name + "/%Y-%m-%d_%H-%M-%S.%N.log",
    keywords::rotation_size = 10 * 1024 * 1024,
    keywords::min_free_space = 30 * 1024 * 1024,
    keywords::open_mode = std::ios_base::app,
    keywords::filter = a_channel == fileter_name
  );
  if (sink == nullptr) return false;

  sink->set_formatter(log_format);
  sink->locked_backend()->auto_flush(true);
  return true;
}

}