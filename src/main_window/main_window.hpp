#pragma once

#include <raylib.h>
#include <string>

#include "../game/game_board.hpp"
#include "../net/client_impl.hpp"
#include "../ui/button.hpp"
#include "../ui/selection.hpp"

static const uint8_t NUM_CELLS = 10;

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
	game_board board{{100, 40, 400, 400}, NUM_CELLS};
	net_client client{board.selected_cells};

	bool quit = false;
	static constexpr int window_width = 600, window_height = 480;
	std::string hostname = "127.0.0.1";
	uint16_t port = 60000;

	uint8_t submarines_count = 2;
	selection select_submarines{ {200, 150}, "Submarines: %d ", submarines_count};
	uint8_t destroyer_count = 2;
	selection select_destroyers{ {200, 180}, "Destroyers: %d ", destroyer_count};
	uint8_t cruiser_count = 1;
	selection select_cruisers{   {200, 210}, "Cruisers: %d   ", cruiser_count};
	uint8_t battleship_count = 1;
	selection select_battleships{{200, 240}, "Battleships: %d", battleship_count};
	uint8_t carrier_count = 1;
	selection select_carriers{   {200, 270}, "Carriers: %d   ", carrier_count};
	button connect_btn{{200, 330, 200, 50}, "CONNECT"};
};
