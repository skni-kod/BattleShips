#pragma once
#include <cstdint>

enum class message_type : uint32_t { accept, deny, send_board_state, recv_board_state };
