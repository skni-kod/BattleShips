#ifndef EXAMPLE
#ifdef CLIENT
#include "main_window/main_window.hpp"

int main()
{
	main_window game;
	game.loop();
	return 0;
}
#endif
#ifdef SERVER
#include "net/server_impl.hpp"
int main()
{
	net_server s(60000);
	s.start();
	while (1) {
		s.update(SIZE_MAX, true);
	}
	return 0;
}
#endif
#endif
#ifdef EXAMPLE
#include "net/net.hpp"

enum class CustomMsgTypes : uint32_t {
	ServerAccept,
	ServerDeny,
	ServerPing,
	MessageAll,
	ServerMessage,
};

#ifdef CLIENT
#include <raylib.h>

class CustomClient : public net::client_interface<CustomMsgTypes>
{
public:
	void PingServer()
	{
		net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::ServerPing;

		// Caution with this...
		std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();

		msg << timeNow;
		send(msg);
	}

	void MessageAll()
	{
		net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::MessageAll;
		send(msg);
	}
};

int main()
{
	InitWindow(800, 600, "Battleships");
	SetTargetFPS(60);

	CustomClient c;
	c.connect("127.0.0.1", 60000);

	bool quit = false;
	while (!WindowShouldClose() && !quit) {
		if (IsKeyPressed(KEY_P))
			c.PingServer();
		if (IsKeyPressed(KEY_M))
			c.MessageAll();
		if (IsKeyPressed(KEY_Q))
			quit = true;

		if (c.is_connected()) {
			if (!c.incoming().empty()) {
				auto msg = c.incoming().pop_front().msg;

				switch (msg.header.id) {
				case CustomMsgTypes::ServerAccept: {
					// Server has responded to a ping request
					std::cout << "Server Accepted Connection\n";
				} break;

				case CustomMsgTypes::ServerPing: {
					// Server has responded to a ping request
					std::chrono::system_clock::time_point timeNow =
					    std::chrono::system_clock::now();
					std::chrono::system_clock::time_point timeThen;
					msg >> timeThen;
					std::cout
					    << "Ping: " << std::chrono::duration<double>(timeNow - timeThen).count()
					    << "\n";
				} break;

				case CustomMsgTypes::ServerMessage: {
					// Server has responded to a ping request
					uint32_t clientID;
					msg >> clientID;
					std::cout << "Hello from [" << clientID << "]\n";
				} break;
				default:
					break;
				}
			}
		} else {
			std::cout << "Server Down\n";
			quit = true;
		}

		BeginDrawing();

		ClearBackground(BLACK);

		EndDrawing();
	}

	return 0;
}
#endif
#ifdef SERVER
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
#endif
#endif