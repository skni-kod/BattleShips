#pragma once
#include <cstdint>

enum class message_type : uint32_t { accept, deny, send_guess, recv_guess };
