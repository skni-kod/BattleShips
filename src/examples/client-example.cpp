#include <iostream>
#include <raylib.h>

#include "../net/net.hpp"

enum class CustomMsgTypes : uint32_t {
	ServerAccept,
	ServerDeny,
	ServerPing,
	MessageAll,
	ServerMessage,
};

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
	InitWindow(600, 400, "Client example");
	SetTargetFPS(60);
	SetExitKey(KEY_Q);

	CustomClient c;
	c.connect("127.0.0.1", 60000);

	bool quit = false;
	while (!WindowShouldClose()) {
		if (IsKeyPressed(KEY_P))
			c.PingServer();
		if (IsKeyPressed(KEY_M))
			c.MessageAll();

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

		ClearBackground(LIGHTGRAY);

		EndDrawing();
	}

	return 0;
}