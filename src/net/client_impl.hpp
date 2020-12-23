#pragma once
#include <iostream>

#include "net.hpp"
#include "message_type.hpp"

template <uint32_t num_cells> class net_client : public net::client_interface<message_type>
{
public:
	net_client(std::array<bool, num_cells * num_cells> &selected_cells) : board_state(selected_cells){};
	void send_board_state()
	{
		net::message<message_type> msg;
		msg.header.id = message_type::send_board_state;

		msg << board_state;
		send(msg);
	}

	bool loop()
	{
		if (is_connected()) {
			if (!incoming().empty()) {
				auto msg = incoming().pop_front().msg;

				switch (msg.header.id) {
				case message_type::accept: {
					std::cout << "Server Accepted Connection" << std::endl;
				} break;

				case message_type::deny: {
					std::cout << "Server Denied Connection" << std::endl;
				} break;

				case message_type::recv_board_state: {
					std::cout << "Recived board state" << std::endl;
					msg >> board_state;
				} break;

				default: break;
				}
			}
		} else {
			std::cout << "Server Down" << std::endl;
			return true;
		}
		return false;
	}

private:
	std::array<bool, num_cells * num_cells> &board_state;
};
