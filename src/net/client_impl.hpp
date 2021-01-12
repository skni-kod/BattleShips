#pragma once

#include <iostream>

#include "message_type.hpp"
#include "net.hpp"

class net_client : public net::client_interface<message_type>
{
public:
	net_client(std::vector<uint32_t> &selected_cells) : board_state(selected_cells){};

	void send_board_state()
	{
		net::message<message_type> msg;
		msg.header.id = message_type::send_guess;

		msg << board_state[board_state.size() - 1];
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

				case message_type::recv_guess: {
					std::cout << "Recived board state" << std::endl;
					uint32_t guess;
					msg >> guess;
					board_state.push_back(guess);
				} break;

				default:
					break;
				}
			}
		} else {
			std::cout << "Server Down" << std::endl;
			return true;
		}
		return false;
	}

private:
	std::vector<uint32_t> &board_state;
};
