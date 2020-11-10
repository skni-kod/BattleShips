#pragma once

#include <iostream>

#include <asio.hpp>

#include "../message/message.hpp"
#include "../queue/queue.hpp"

namespace network
{

template <typename MessageType> class Connection : public std::enable_shared_from_this<Connection<MessageType>>
{
public:
	enum class owner { server, client };

	Connection(owner parent, asio::io_context &ctx, asio::ip::tcp::socket sock,
		   Queue<OwnedMessage<MessageType>> &in)
	    : socket(std::move(sock)), context(ctx), input(in)
	{
		owner_type = parent;
	}

	virtual ~Connection() {};

	uint32_t get_id() const { return id; }

	void connect_client(uint32_t uid = 0)
	{
		if (owner_type == owner::server) {
			if (socket.is_open()) {
				id = uid;
				read_header();
			}
		}
	}

	void connect_server(const asio::ip::tcp::resolver::results_type &endpoints)
	{
		if (owner_type == owner::client) {
			asio::async_connect(
			    socket, endpoints,
			    [this](std::error_code ec, asio::ip::tcp::endpoint endpoint [[maybe_unused]]) {
				    if (!ec) {
					    read_header();
				    }
			    });
		}
	}

	void disconnect()
	{
		if (is_connected())
			asio::post(context, [this]() { socket.close(); });
	}

	bool is_connected() const { return socket.is_open(); }

	void send(const Message<MessageType> &msg)
	{
		asio::post(context, [this, msg]() {
			bool writing = !output.empty();
			output.push_back(msg);
			if (writing) {
				write_header();
			}
		});
	}

private:
	void write_header()
	{
		asio::async_write(socket, asio::buffer(&output.front().header, sizeof(MessageHeader<MessageType>)),
				  [this](std::error_code ec, size_t length [[maybe_unused]]) {
					  if (!ec) {
						  if (output.front().body.size() > 0) {
							  write_body();
						  } else {
							  output.pop_front();

							  if (!output.empty()) {
								  write_header();
							  }
						  }
					  } else {
						  std::cout << "[" << id << "] Writing header failed.\n";
						  socket.close();
					  }
				  });
	}

	void write_body()
	{
		asio::async_write(socket, asio::buffer(output.front().body.data(), output.front().body.size()),
				  [this](std::error_code ec, size_t length [[maybe_unused]]) {
					  if (!ec) {
						  output.pop_front();

						  if (!output.empty()) {
							  write_header();
						  }
					  } else {
						  std::cout << "[" << id << "] Writing body failed.\n";
						  socket.close();
					  }
				  });
	}

	void read_header()
	{
		asio::async_read(socket, asio::buffer(&tmp_input.header, sizeof(MessageHeader<MessageType>)),
				 [this](std::error_code ec, size_t length [[maybe_unused]]) {
					 if (!ec) {
						 if (tmp_input.header.size > 0) {
							 tmp_input.body.resize(tmp_input.header.size);
							 read_body();
						 } else {
							 add_incoming_message();
						 }
					 } else {
						 std::cout << "[" << id << "] Reading header failed.\n";
						 socket.close();
					 }
				 });
	}

	void read_body()
	{
		asio::async_read(socket, asio::buffer(tmp_input.body.data(), tmp_input.body.size()),
				 [this](std::error_code ec, size_t length [[maybe_unused]]) {
					 if (!ec) {
						 add_incoming_message();
					 } else {
						 std::cout << "[" << id << "] Read Body Fail.\n";
						 socket.close();
					 }
				 });
	}

	void add_incoming_message()
	{
		if (owner_type == owner::server)
			input.push_back({this->shared_from_this(), tmp_input});
		else
			input.push_back({nullptr, tmp_input});

		read_header();
	}

private:
	asio::ip::tcp::socket socket;
	asio::io_context &context;
	Queue<OwnedMessage<MessageType>> &input;
	Queue<Message<MessageType>> output;
	Message<MessageType> tmp_input;
	owner owner_type = owner::server;

	uint32_t id = 0;
};

} // namespace network
