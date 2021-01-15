#pragma once
#include <cstdint>

enum class message_type : uint32_t { accept, deny, start, send_guess, recv_guess, send_validation, recv_validation };
