#pragma once

#include <deque>
#include <iostream>
#include <memory>
#include <thread>

#include "../network_common.hpp"
#include "../connection/connection.hpp"
#include "../message/message.hpp"
#include "../queue/queue.hpp"

namespace network
{

template <typename MessageType> class Server
{
public:
	Server(uint16_t port) : acceptor(context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) {}

	virtual ~Server() { stop(); }

	bool start()
	{
		try {
			wait_for_connection();

			thread = std::thread([this]() { context.run(); });
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

		if (thread.joinable())
			thread.join();

		std::cout << "[SERVER] Stopped!\n";
	}

	void wait_for_connection()
	{
		acceptor.async_accept([this](std::error_code ec, asio::ip::tcp::socket socket) {
			if (!ec) {
				std::cout << "[SERVER] New Connection: " << socket.remote_endpoint() << "\n";

				std::shared_ptr<Connection<MessageType>> newconn =
				    std::make_shared<Connection<MessageType>>(Connection<MessageType>::owner::server,
									      context, std::move(socket), input);

				if (on_client_connect(newconn)) {
					connections.push_back(std::move(newconn));

					connections.back()->connect_client(id_counter++);

					std::cout << "[" << connections.back()->get_id() << "] Connection Approved\n";
				} else {
					std::cout << "[-----] Connection Denied\n";
				}
			} else {
				std::cout << "[SERVER] New Connection Error: " << ec.message() << "\n";
			}

			wait_for_connection();
		});
	}

	void message_client(std::shared_ptr<Connection<MessageType>> client, const Message<MessageType> &msg)
	{
		if (client && client->is_connected()) {
			client->send(msg);
		} else {
			on_client_disconnect(client);

			client.reset();

			connections.erase(std::remove(connections.begin(), connections.end(), client),
					  connections.end());
		}
	}

	void message_all_clients(const Message<MessageType> &msg,
				 std::shared_ptr<Connection<MessageType>> ignored_client = nullptr)
	{
		bool invalid = false;

		for (auto &client : connections) {
			if (client && client->is_connected()) {
				if (client != ignored_client)
					client->send(msg);
			} else {
				on_client_disconnect(client);
				client.reset();

				invalid = true;
			}
		}

		if (invalid)
			connections.erase(std::remove(connections.begin(), connections.end(), nullptr),
					  connections.end());
	}

	void update(size_t max_messages = UINT_MAX, bool wait = false)
	{
		if (wait)
			input.wait();

		size_t count = 0;
		while (count < max_messages && !input.empty()) {
			auto msg = input.pop_front();

			on_message(msg.remote, msg.message);

			count++;
		}
	}

protected:
	virtual bool on_client_connect(std::shared_ptr<Connection<MessageType>> client [[maybe_unused]]) { return false; };

	virtual void on_client_disconnect(std::shared_ptr<Connection<MessageType>> client [[maybe_unused]]) {};

	virtual void on_message(std::shared_ptr<Connection<MessageType>> client [[maybe_unused]], Message<MessageType> &msg [[maybe_unused]]) {};

protected:
	Queue<OwnedMessage<MessageType>> input;

	std::deque<std::shared_ptr<Connection<MessageType>>> connections;

	asio::io_context context;
	std::thread thread;

	asio::ip::tcp::acceptor acceptor;

	uint32_t id_counter = 10000;
};
} // namespace network
