#pragma once

#include <functional>
#include <iostream>

#include "message_type.hpp"
#include "net.hpp"
#include "../game/guess_type.hpp"

/// \brief Implementacja klasy klient.
class net_client : public net::client_interface<message_type>
{
public:
	/// \brief Konstruktor implementacji klasy klient.
	net_client(std::function<void()> on_start_func, std::function<void(uint32_t guess_index)> on_recieve_guess_func,
		   std::function<void(guess_type type)> on_recieve_validation_func, std::function<void()> on_end_func)
	    : on_start(on_start_func), on_recieve_guess(on_recieve_guess_func), on_recieve_validation(on_recieve_validation_func), on_end(on_end_func){};

	/// \brief Metoda wysyłająca start.
	void start()
	{
		net::message<message_type> msg;
		msg.header.id = message_type::start;
		send(msg);
	}

	/// \brief Metoda wysyłająca próbę zgadnięcia.
	void send_guess(uint32_t guess)
	{
		net::message<message_type> msg;
		msg.header.id = message_type::send_guess;

		msg << guess;
		send(msg);
	}

	/// \brief Metoda wysyłająca walidację próby zgadnięcia.
	void send_validation(guess_type type)
	{
		net::message<message_type> msg;
		msg.header.id = message_type::send_validation;

		msg << type;
		send(msg);
	}

	/// \brief Metoda wysyłająca koniec.
	void end()
	{
		net::message<message_type> msg;
		msg.header.id = message_type::end;
		send(msg);
	}

	/// \brief Metoda reagująca na przychodzące wiadomości.
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
					guess_type type;
					msg >> type;
					std::cout << "guess " << (type == guess_type::hit || type == guess_type::hit_and_sunk ? "good" : "bad") << std::endl;
					on_recieve_validation(type);
				} break;

				case message_type::end: {
					std::cout << "Ending Game" << std::endl;
					on_end();
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
	/// \brief Wskaźnik na funkcje wywoływaną na starcie połączenia.
	std::function<void()> on_start;
	/// \brief Wskaźnik na funkcje wywoływaną przy otrzymaniu próby zgadnięcia.
	std::function<void(uint32_t guess_index)> on_recieve_guess;
	/// \brief Wskaźnik na funkcje wywoływaną przy otrzymaniu walidacji próby zgadnięcia.
	std::function<void(guess_type type)> on_recieve_validation;
	/// \brief Wskaźnik na funkcje wywoływaną przy końcu rozgrywki.
	std::function<void()> on_end;
};
