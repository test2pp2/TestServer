#pragma once

#include "Opcode.h"

namespace Contents {

class Session;

using HandlerCallback = std::function<void(std::shared_ptr<Session>, JsonObject)>;

class PacketkHandler {
public:
  static void Register(Opcode opcode, HandlerCallback callback);
  static bool Call(std::shared_ptr<Session> session, Opcode opcode, JsonObject payload);
};

}
