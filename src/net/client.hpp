#pragma once

#include "common.hpp"
#include "connection.hpp"
#include "message.hpp"

namespace net
{
/**
 * \brief Klasa klienta.
 */
template <typename T> class client_interface
{
public:
	/**
	 * \brief Konstruktor klasy klienta.
	 */
	client_interface() {}

	/**
	 * \brief Destruktor klasy klienta.
	 */
	virtual ~client_interface() { disconnect(); }

public:
	/**
	 * \brief Metoda łącząca klienta do serwera.
	 * Łączy z serwerem oraz rozpoczyna nowy wątek dla asio.
	 */
	bool connect(const std::string &host, const uint16_t port)
	{
		try {
			asio::ip::tcp::resolver resolver(context);
			asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));

			server_connection = std::make_unique<connection<T>>(
			    connection<T>::owner::client, context, asio::ip::tcp::socket(context), input_messages);

			server_connection->connect_to_server(endpoints);

			context_thread = std::thread([this]() { context.run(); });
		} catch (std::exception &e) {
			std::cerr << "Client Exception: " << e.what() << "\n";
			return false;
		}
		return true;
	}

	/**
	 * \brief Metoda rozłączająca z serwerem.
	 */
	void disconnect()
	{
		if (is_connected()) {
			server_connection->disconnect();
		}

		context.stop();
		if (context_thread.joinable())
			context_thread.join();

		server_connection.release();
	}

	/**
	 * \brief Metoda zwracająca true jeśli klient jest połączony do serwera.
	 */
	bool is_connected()
	{
		if (server_connection)
			return server_connection->is_connected();
		else
			return false;
	}

public:
	/**
	 * \brief Metoda wysyłająca wiadomość do serwera przez połączenie.
	 */
	void send(const message<T> &msg)
	{
		if (is_connected())
			server_connection->send(msg);
	}

	/**
	 * \brief Metoda zwracająca referencje do przychodzących wiadomości.
	 */
	tsqueue<owned_message<T>> &incoming() { return input_messages; }

protected:
	asio::io_context context;
	std::thread context_thread;
	std::unique_ptr<connection<T>> server_connection;

private:
	tsqueue<owned_message<T>> input_messages;
};
} // namespace net