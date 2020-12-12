#include <iostream>

#include "net/net.hpp"

enum class CustomMsgTypes : uint32_t {
	ServerAccept,
	ServerDeny,
	ServerPing,
	MessageAll,
	ServerMessage,
};

class CustomServer : public net::server_interface<CustomMsgTypes>
{
public:
	CustomServer(uint16_t nPort) : net::server_interface<CustomMsgTypes>(nPort) {}

protected:
	virtual bool on_client_connect(std::shared_ptr<net::connection<CustomMsgTypes>> client)
	{
		net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::ServerAccept;
		client->send(msg);
		return true;
	}

	// Called when a client appears to have disconnected
	virtual void on_client_disconnect(std::shared_ptr<net::connection<CustomMsgTypes>> client)
	{
		std::cout << "Removing client [" << client->get_id() << "]\n";
	}

	// Called when a message arrives
	virtual void on_message(std::shared_ptr<net::connection<CustomMsgTypes>> client,
				net::message<CustomMsgTypes> &msg)
	{
		switch (msg.header.id) {
		case CustomMsgTypes::ServerPing: {
			std::cout << "[" << client->get_id() << "]: Server Ping\n";

			// Simply bounce message back to client
			client->send(msg);
		} break;

		case CustomMsgTypes::MessageAll: {
			std::cout << "[" << client->get_id() << "]: Message All\n";

			// Construct a new message and send it to all clients
			net::message<CustomMsgTypes> newmsg;
			newmsg.header.id = CustomMsgTypes::ServerMessage;
			newmsg << client->get_id();
			message_all_clients(newmsg, client);

		} break;
		default:
			break;
		}
	}
};

int main()
{
	CustomServer server(60000);
	server.start();

	while (1) {
		server.update(SIZE_MAX, true);
	}

	return 0;
}