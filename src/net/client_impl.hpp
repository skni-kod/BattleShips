#pragma once

#include <functional>
#include <iostream>

#include "message_type.hpp"
#include "net.hpp"

class net_client : public net::client_interface<message_type>
{
public:
	net_client(std::function<void()> on_start_func, std::function<void(uint32_t guess)> on_recieve_guess_func,
		   std::function<void(bool good)> on_recieve_validation_func)
	    : on_start(on_start_func), on_recieve_guess(on_recieve_guess_func), on_recieve_validation(on_recieve_validation_func){};

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

	void send_validation(bool good)
	{
		net::message<message_type> msg;
		msg.header.id = message_type::send_validation;

		msg << good;
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
					on_start();
				} break;

				case message_type::recv_guess: {
					std::cout << "Recived Guess" << std::endl;
					uint32_t guess;
					msg >> guess;
					on_recieve_guess(guess);
				} break;

				case message_type::recv_validation: {
					std::cout << "Recived Validation: ";
					bool good;
					msg >> good;
					std::cout << "guess " << (good ? "good" : "bad") << std::endl;
					on_recieve_validation(good);
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
	std::function<void()> on_start;
	std::function<void(uint32_t guess)> on_recieve_guess;
	std::function<void(bool guess)> on_recieve_validation;
};
