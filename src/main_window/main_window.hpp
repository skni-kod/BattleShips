#pragma once

#include <raylib.h>

#include "../game/game_board.hpp"
#include "../net/client_impl.hpp"
#include "../ui/button.hpp"

enum class window_type { MENU_INIT, MENU, GAME_INIT, GAME, GAMEOVER };

class main_window
{
public:
	main_window();

	~main_window();

	void loop();

private:
	void menu_init();

	void menu_update();

	void menu_draw();

	void game_init();

	void game_update();

	void game_draw();

	window_type current_window = window_type::MENU_INIT;
	game_board<10> board{400, 400, 100, 40};
	net_client<10> client{board.selected_cells};

	bool quit = false;
	static constexpr int window_width = 600, window_height = 480;
	std::string hostname = "127.0.0.1";
	uint16_t port = 60000;

	button connect_btn{{(window_width - 100)/2, (window_height - 50)/2, 100, 50}};
};
