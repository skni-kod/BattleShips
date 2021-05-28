#pragma once

#include <raylib.h>
#include <string>
#include <map>

#include "../game/game_board.hpp"
#include "../game/game_ships.hpp"
#include "../game/game_net_interface.hpp"
#include "../ui/button.hpp"
#include "../ui/selection.hpp"
#include "window_type.hpp"

class main_window
{
	friend class selection;
	friend class game_ships;

public:
	main_window();

	~main_window();

	void loop();

private:
	void menu_init();

	void menu_update();

	void menu_draw();

	void game_start();

	void wait_init();

	void wait_update();

	void wait_draw();

	void board_init();

	void board_update();

	void board_draw();

	void game_over();

	bool quit = false, won = false;
	static constexpr int window_width = 800, window_height = 450;
	std::string hostname = "127.0.0.1";
	uint16_t port = 60000;
	std::string turn_message;

	window_type current_window = window_type::MENU_INIT;
	game_board board{{50, 25, 400, 400}, 10};
	game_net_interface game_interface{current_window, turn_message, won, board};

	static inline ship_type selected_ship_type = ship_type::submarine;

	static inline std::map<ship_type, uint8_t> ship_types_count = {
		{ship_type::submarine, 2}, {ship_type::destroyer, 2}, {ship_type::cruiser, 1}, {ship_type::battleship, 1},
	    {ship_type::carrier, 1}
	};

	selection select_submarines{{515, 210}, "Submarines: %d ", ship_type::submarine};
	selection select_destroyers{{515, 240}, "Destroyers: %d ", ship_type::destroyer};
	selection select_cruisers{{515, 270}, "Cruisers: %d   ", ship_type::cruiser};
	selection select_battleships{{515, 300}, "Battleships: %d", ship_type::battleship};
	selection select_carriers{{515, 330}, "Carriers: %d   ", ship_type::carrier};

	button connect_btn{{515, 375, 220, 50}, "CONNECT"};
};
