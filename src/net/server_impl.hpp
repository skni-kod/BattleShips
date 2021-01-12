#pragma once

#include "message_type.hpp"
#include "net.hpp"

class net_server : public net::server_interface<message_type>
{
public:
	net_server(uint16_t port) : net::server_interface<message_type>(port) {}

private:
	virtual bool on_client_connect(std::shared_ptr<net::connection<message_type>> client)
	{
		net::message<message_type> msg;
		msg.header.id = message_type::accept;
		client->send(msg);
		return true;
	}

	virtual void on_client_disconnect(std::shared_ptr<net::connection<message_type>> client)
	{
		std::cout << "Removing client [" << client->get_id() << "]\n";
	}

	virtual void on_message(std::shared_ptr<net::connection<message_type>> client, net::message<message_type> &msg)
	{
		switch (msg.header.id) {
		case message_type::send_guess: {
			std::cout << "[" << client->get_id() << "]: Sending Board State\n";
			msg.header.id = message_type::recv_guess;
			message_all_clients(msg, client);
		} break;

		default: break;
		}
	}
};
