#pragma once

#include <memory>
#include <thread>

#include "../network_common.hpp"
#include "../connection/connection.hpp"

namespace network
{
template <typename MessageType> class Client
{
public:
	bool connect(const std::string &host, const uint16_t port)
	{
		try {
			asio::ip::tcp::resolver resolver(context);
			asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));

			connection = std::make_unique<Connection<MessageType>>(
			    Connection<MessageType>::owner::client, context, asio::ip::tcp::socket(context), input);

			connection->connect_server(endpoints);

			thread = std::thread([this]() { context.run(); });
		} catch (std::exception &e) {
			std::cerr << "Client Exception: " << e.what() << "\n";
			return false;
		}
		return true;
	}

	void disconnect()
	{
		if (is_connected()) {
			connection->disconnect();
		}

		context.stop();
		if (thread.joinable())
			thread.join();

		connection.release();
	}

	bool is_connected()
	{
		if (connection)
			return connection->is_connected();
		else
			return false;
	}

	void send(const Message<MessageType> &msg)
	{
		if (is_connected())
			connection->send(msg);
	}

	Queue<OwnedMessage<MessageType>> &incoming() { return input; }

	Client(){};

	virtual ~Client() { disconnect(); };

protected:
	asio::io_context context;
	std::thread thread;
	std::unique_ptr<Connection<MessageType>> connection;

private:
	Queue<OwnedMessage<MessageType>> input;
};
} // namespace network
