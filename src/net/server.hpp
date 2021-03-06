#pragma once

#include "common.hpp"
#include "connection.hpp"
#include "message.hpp"
#include "tsqueue.hpp"

namespace net
{
template <typename T> class server_interface
{
public:
	server_interface(uint16_t port) : acceptor(context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) {}

	virtual ~server_interface() { stop(); }

	bool start()
	{
		try {
			wait_for_connection();

			context_thread = std::thread([this]() { context.run(); });
		} catch (std::exception &e) {
			std::cerr << "[SERVER] Exception: " << e.what() << "\n";
			return false;
		}

		std::cout << "[SERVER] Started!\n";
		return true;
	}

	void stop()
	{
		context.stop();

		if (context_thread.joinable())
			context_thread.join();

		std::cout << "[SERVER] Stopped!\n";
	}

	void wait_for_connection()
	{
		acceptor.async_accept([this](std::error_code ec, asio::ip::tcp::socket socket) {
			if (!ec) {
				std::cout << "[SERVER] New Connection: " << socket.remote_endpoint() << "\n";

				std::shared_ptr<connection<T>> newconn = std::make_shared<connection<T>>(
				    connection<T>::owner::server, context, std::move(socket), input_messages);

				if (on_client_connect(newconn)) {
					connections.push_back(std::move(newconn));

					connections.back()->connect_to_client(id_counter++);

					std::cout << "[" << connections.back()->get_id() << "] Connection Approved\n";
				} else {
					std::cout << "[-----] Connection Denied\n";
				}
			} else {
				std::cout << "[SERVER] New Connection Error: " << ec.message() << "\n";
			}

			// wait for another connection
			wait_for_connection();
		});
	}

	void message_client(std::shared_ptr<connection<T>> client, const message<T> &msg)
	{
		if (client && client->is_connected()) {
			client->send(msg);
		} else {
			// remove the client
			on_client_disconnect(client);

			client.reset();

			connections.erase(std::remove(connections.begin(), connections.end(), client),
					  connections.end());
		}
	}

	void message_all_clients(const message<T> &msg, std::shared_ptr<connection<T>> ignored_client = nullptr)
	{
		bool invalid_client_exists = false;

		for (auto &client : connections) {
			if (client && client->is_connected()) {
				if (client != ignored_client)
					client->send(msg);
			} else {
				// assume it disconnected
				on_client_disconnect(client);
				client.reset();

				invalid_client_exists = true;
			}
		}

		// remove dead clients
		if (invalid_client_exists)
			connections.erase(std::remove(connections.begin(), connections.end(), nullptr),
					  connections.end());
	}

	void update(size_t max_msgs_count = SIZE_MAX, bool wait = false)
	{
		if (wait)
			input_messages.wait();

		size_t msgs_count = 0;
		while (msgs_count < max_msgs_count && !input_messages.empty()) {
			auto msg = input_messages.pop_front();

			on_message(msg.remote, msg.msg);

			msgs_count++;
		}
	}

protected:
	virtual bool on_client_connect(std::shared_ptr<connection<T>> client [[maybe_unused]]) { return false; }

	virtual void on_client_disconnect(std::shared_ptr<connection<T>> client [[maybe_unused]]) {}

	virtual void on_message(std::shared_ptr<connection<T>> client [[maybe_unused]],
				message<T> &msg [[maybe_unused]]) {}

protected:
	tsqueue<owned_message<T>> input_messages;
	std::deque<std::shared_ptr<connection<T>>> connections;
	asio::io_context context;
	std::thread context_thread;
	asio::ip::tcp::acceptor acceptor;
	uint32_t id_counter = 10000;
};
} // namespace net