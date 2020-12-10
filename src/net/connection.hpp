#pragma once

#include "common.hpp"
#include "tsqueue.hpp"
#include "message.hpp"


namespace net
{
	template<typename T>
	class connection : public std::enable_shared_from_this<connection<T>>
	{
	public:
		// A connection is "owned" by either a server or a client, and its
		// behaviour is slightly different bewteen the two.
		enum class owner
		{
			server,
			client
		};

	public:
		// Specify Owner, connect to context, transfer the socket, Provide reference to incoming message queue
		connection(owner parent, asio::io_context& ctx, asio::ip::tcp::socket sock, tsqueue<owned_message<T>>& input)
			: context(ctx), socket(std::move(sock)), input_messages(input)
		{
			owner_type = parent;
		}

		virtual ~connection()
		{}

		// This ID is used system wide - its how clients will understand other clients exist across the whole system.
		uint32_t get_id() const
		{
			return id;
		}

	public:
		void connect_to_client(uint32_t uid = 0)
		{
			if (owner_type == owner::server)
			{
				if (socket.is_open())
				{
					id = uid;
					read_header();
				}
			}
		}

		void connect_to_server(const asio::ip::tcp::resolver::results_type& endpoints)
		{
			// Only clients can connect to servers
			if (owner_type == owner::client)
			{
				asio::async_connect(socket, endpoints,
					[this](std::error_code ec, asio::ip::tcp::endpoint endpoint [[maybe_unused]])
					{
						if (!ec)
						{
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

		bool is_connected() const
		{
			return socket.is_open();
		}

		// Prime the connection to wait for incoming messages
		void start_listening()
		{

		}

	public:
		// Asynchronously sends a message, connections are one-to-one so no need to specifiy
		// the target, for a client, the target is the server and vice versa
		void send(const message<T>& msg)
		{
			asio::post(context,
				[this, msg]()
				{
					bool currently_writing = !output_messages.empty();
					output_messages.push_back(msg);
					if (!currently_writing)
					{
						write_header();
					}
				});
		}



	private:
		// Asynchronously write the message header
		void write_header()
		{
			asio::async_write(socket, asio::buffer(&output_messages.front().header, sizeof(message_header<T>)),
				[this](std::error_code ec, std::size_t length [[maybe_unused]])
				{
					if (!ec)
					{
						// check if message has body
						if (output_messages.front().body.size() > 0)
						{
							write_body();
						}
						else
						{
							output_messages.pop_front();

							// issue antoher write if queue is not empty
							if (!output_messages.empty())
							{
								write_header();
							}
						}
					}
					else
					{
						std::cout << "[" << id << "] Write Header Fail.\n";
						socket.close();
					}
				});
		}

		// Asynchronously write a message body
		void write_body()
		{
			asio::async_write(socket, asio::buffer(output_messages.front().body.data(), output_messages.front().body.size()),
				[this](std::error_code ec, std::size_t length [[maybe_unused]])
				{
					if (!ec)
					{
						output_messages.pop_front();

						// issue antoher write if queue is not empty
						if (!output_messages.empty())
						{
							write_header();
						}
					}
					else
					{
						std::cout << "[" << id << "] Write Body Fail.\n";
						socket.close();
					}
				});
		}

		// Asynchronously read a message header
		void read_header()
		{
			asio::async_read(socket, asio::buffer(&temporary_input.header, sizeof(message_header<T>)),
				[this](std::error_code ec, std::size_t length [[maybe_unused]])
				{
					if (!ec)
					{
						// check if message has body
						if (temporary_input.header.size > 0)
						{
							temporary_input.body.resize(temporary_input.header.size);
							read_body();
						}
						else
						{
							add_to_incoming();
						}
					}
					else
					{
						std::cout << "[" << id << "] Read Header Fail.\n";
						socket.close();
					}
				});
		}

		// Asynchronously read a message body
		void read_body()
		{
			asio::async_read(socket, asio::buffer(temporary_input.body.data(), temporary_input.body.size()),
				[this](std::error_code ec, std::size_t length [[maybe_unused]])
				{
					if (!ec)
					{
						add_to_incoming();
					}
					else
					{
						std::cout << "[" << id << "] Read Body Fail.\n";
						socket.close();
					}
				});
		}

		// Once a full message is received, add it to the incoming queue
		void add_to_incoming()
		{
			// save to incoming queue converting message to owned_message
			if(owner_type == owner::server)
				input_messages.push_back({ this->shared_from_this(), temporary_input });
			else
				input_messages.push_back({ nullptr, temporary_input });

			// now issue next read task
			read_header();
		}

	protected:
		asio::io_context& context;
		asio::ip::tcp::socket socket;
		tsqueue<message<T>> output_messages;
		tsqueue<owned_message<T>>& input_messages;
		message<T> temporary_input;
		owner owner_type = owner::server;
		uint32_t id = 0;
	};
}