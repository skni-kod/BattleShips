/*#include "game/game.hpp"
#include "network/network.hpp"

int main()
{
	Game game;

	game.loop();

	return 0;
}*/

#include "network/network.hpp"
#include <iostream>

enum class CustomMsgTypes : uint32_t {
	ServerAccept,
	ServerDeny,
	ServerPing,
	MessageAll,
	ServerMessage,
};

class CustomServer : public network::Server<CustomMsgTypes>
{
public:
	CustomServer(uint16_t port) : network::Server<CustomMsgTypes>(port) {}

protected:
	virtual bool on_client_connect(std::shared_ptr<network::Connection<CustomMsgTypes>> client)
	{
		network::Message<CustomMsgTypes> msg;
		msg.header.type = CustomMsgTypes::ServerAccept;
		client->send(msg);
		return true;
	}

	virtual void on_client_disconnect(std::shared_ptr<network::Connection<CustomMsgTypes>> client)
	{
		std::cout << "Removing client [" << client->get_id() << "]\n";
	}

	virtual void on_message(std::shared_ptr<network::Connection<CustomMsgTypes>> client,
			       network::Message<CustomMsgTypes> &msg)
	{
		switch (msg.header.type) {
		case CustomMsgTypes::ServerPing: {
			std::cout << "[" << client->get_id() << "]: Server Ping\n";

			client->send(msg);
		} break;

		case CustomMsgTypes::MessageAll: {
			std::cout << "[" << client->get_id() << "]: Message All\n";

			network::Message<CustomMsgTypes> newmsg;
			newmsg.header.type = CustomMsgTypes::ServerMessage;
			newmsg.add(client->get_id());
			message_all_clients(msg, client);

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
		server.update(UINT_MAX, true);
	}

	return 0;
}
