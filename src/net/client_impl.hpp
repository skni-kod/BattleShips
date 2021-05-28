#pragma once

#include <iostream>

#include "message_type.hpp"
#include "net.hpp"

enum class guess_type;

class base_game_net_interface
{
public:
	virtual ~base_game_net_interface() {}
	virtual void on_start() {}
	virtual void on_recieve_guess(uint32_t) {}
	virtual void on_recieve_validation(guess_type) {}
	virtual void on_end() {}
};

class net_client : public net::client_interface<message_type>
{
public:
	net_client(base_game_net_interface &game_interface) : game_interface(game_interface) {}

	void start()
	{
		net::message<message_type> msg;
		msg.header.id = message_type::start;
		send(msg);
	}

	void send_guess(uint32_t guess)
	{
		net::message<message_type> msg;
		msg.header.id = message_type::send_guess;

		msg << guess;
		send(msg);
	}

	void send_validation(guess_type type)
	{
		net::message<message_type> msg;
		msg.header.id = message_type::send_validation;

		msg << type;
		send(msg);
	}

	void end()
	{
		net::message<message_type> msg;
		msg.header.id = message_type::end;
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

				case message_type::start: {
					std::cout << "Starting Game" << std::endl;
					game_interface.on_start();
				} break;

				case message_type::recv_guess: {
					std::cout << "Recived Guess" << std::endl;
					uint32_t guess;
					msg >> guess;
					game_interface.on_recieve_guess(guess);
				} break;

				case message_type::recv_validation: {
					std::cout << "Recived Validation: ";
					guess_type type;
					msg >> type;
					game_interface.on_recieve_validation(type);
				} break;

				case message_type::end: {
					std::cout << "Ending Game" << std::endl;
					game_interface.on_end();
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
	base_game_net_interface &game_interface;
};
