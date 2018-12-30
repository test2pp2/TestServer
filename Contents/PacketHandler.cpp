#include "stdafx.h"
#include "PacketHandler.h"
#include "Session.h"
#include "Opcode.h"

namespace Contents {

static std::array<HandlerCallback, 1000> registers_ = { nullptr, };

void PacketkHandler::Register(Opcode opcode, HandlerCallback callback) {
  registers_[opcode] = callback;
}

bool PacketkHandler::Call(std::shared_ptr<Session> session, Opcode opcode, JsonObject payload) {
  const auto callback = registers_[opcode];
  if (callback == nullptr) {
    return false;
  }
  callback(session, payload);
  return true;
}

}
