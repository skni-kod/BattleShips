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

enum class custom_msg_types : uint32_t {
	server_accept,
	server_deny,
	server_ping,
	server_message,
	vector,
};

#ifdef CLIENT
#include <raylib.h>

class custom_client : public net::client_interface<custom_msg_types>
{
public:
	void ping_server()
	{
		net::message<custom_msg_types> msg;
		msg.header.id = custom_msg_types::server_ping;

		std::chrono::system_clock::time_point time_now = std::chrono::system_clock::now();

		msg << time_now;
		send(msg);
	}

	void send_vector(std::vector<int> vec) {
		net::message<custom_msg_types> msg;
		msg.header.id = custom_msg_types::vector;

		for (auto &v : vec) {
			msg << v;
		}
		msg << static_cast<size_t>(vec.size());

		send(msg);
	}
};

int main()
{
	InitWindow(800, 600, "Battleships");
	SetTargetFPS(60);

	custom_client c;
	c.connect("127.0.0.1", 60000);

	bool quit = false;
	std::vector<int> vec;
	std::string num;
	while (!WindowShouldClose() && !quit) {
		int key = GetCharPressed();

		while ((key >= '0') && (key <= '9')) {
			num += static_cast<char>(key);
			std::cout << "num: " << num << '\n';
			key = GetCharPressed();
		}

		if (IsKeyPressed(KEY_A) && (num.length() > 0)) {
			vec.push_back(std::stoi(num));
			num.clear();
		}
		if (IsKeyPressed(KEY_M)) {
			c.send_vector(vec);
			vec.clear();
		}
		if (IsKeyPressed(KEY_P))
			c.ping_server();
		if (IsKeyPressed(KEY_Q))
			quit = true;

		if (c.is_connected()) {
			if (!c.incoming().empty()) {
				auto msg = c.incoming().pop_front().msg;

				switch (msg.header.id) {
				case custom_msg_types::server_accept: {
					// Server has responded to a ping request
					std::cout << "Server Accepted Connection\n";
				} break;

				case custom_msg_types::server_ping: {
					// Server has responded to a ping request
					std::chrono::system_clock::time_point time_now =
					    std::chrono::system_clock::now();
					std::chrono::system_clock::time_point time_then;
					msg >> time_then;
					std::cout
					    << "Ping: " << std::chrono::duration<double>(time_now - time_then).count()
					    << "\n";
				} break;

				case custom_msg_types::server_message: {
					// Server has responded to a ping request
					uint32_t client_id;
					msg >> client_id;
					std::cout << "Hello from [" << client_id << "]\n";
				} break;

				case custom_msg_types::vector: {
					// Server has responded to a ping request
					std::vector<int> new_vec = {};
					size_t size;

					msg >> size;
					new_vec.resize(size);

					for (size_t i = 0; i < size; i++) {
						msg >> new_vec[i];
					}

					std::cout << "Recived vector of size " << size << " containing: ";
					for (auto &x : new_vec) {
						std::cout << x << ", ";
					}
					std::cout << '\n';

					vec.clear();
					vec.insert(vec.begin(), new_vec.begin(), new_vec.end());
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

		DrawText("new int:", 0, 0, 20, RAYWHITE);
		DrawText(num.c_str(), 80, 0, 20, RAYWHITE);

		DrawText("vec:", 0, 20, 20, RAYWHITE);
		int offset = 50;
		for (auto &v : vec) {
			auto s = std::to_string(v);
			DrawText(s.c_str(), offset, 20, 20, RAYWHITE);
			offset += static_cast<int>(s.length()) * 10 + 10;
		}

		EndDrawing();
	}

	return 0;
}
#endif
#ifdef SERVER
class custom_server : public net::server_interface<custom_msg_types>
{
public:
	custom_server(uint16_t nPort) : net::server_interface<custom_msg_types>(nPort) {}

protected:
	virtual bool on_client_connect(std::shared_ptr<net::connection<custom_msg_types>> client)
	{
		net::message<custom_msg_types> msg;
		msg.header.id = custom_msg_types::server_accept;
		client->send(msg);
		return true;
	}

	// Called when a client appears to have disconnected
	virtual void on_client_disconnect(std::shared_ptr<net::connection<custom_msg_types>> client)
	{
		std::cout << "Removing client [" << client->get_id() << "]\n";
	}

	// Called when a message arrives
	virtual void on_message(std::shared_ptr<net::connection<custom_msg_types>> client,
				net::message<custom_msg_types> &msg)
	{
		switch (msg.header.id) {
		case custom_msg_types::server_ping: {
			std::cout << "[" << client->get_id() << "]: Server Ping\n";

			// Simply bounce message back to client
			client->send(msg);
		} break;

		case custom_msg_types::vector: {
			std::cout << "[" << client->get_id() << "]: Sending Vector\n";

			message_all_clients(msg, client);

		} break;
		default:
			break;
		}
	}
};

int main()
{
	custom_server server(60000);
	server.start();

	while (1) {
		server.update(SIZE_MAX, true);
	}

	return 0;
}
#endif
#endif