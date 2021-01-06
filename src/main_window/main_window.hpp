#pragma once

#include <raylib.h>

#include "../game/game_board.hpp"
#include "../net/client_impl.hpp"

enum class window_type { MENU, GAME, CONNECT, GAMEOVER };

class main_window
{
public:
	main_window();

	~main_window();

	void loop();

private:
	void menu_update();

	void menu_draw();

	void game_update();

	void game_draw();

	window_type current_window = window_type::CONNECT;
	game_board<10> board{400, 400, 100, 40};
	net_client<10> client{board.selected_cells};

	bool quit = false;
	const int window_width = 600, window_height = 480;
	std::string hostname = "127.0.0.1";
	uint16_t port = 60000;
};
