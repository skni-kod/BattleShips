#pragma once

#include <raylib.h>
#include <string>
#include <map>

#include "../game/game_board.hpp"
#include "../game/game_ships.hpp"
#include "../net/client_impl.hpp"
#include "../ui/button.hpp"
#include "../ui/selection.hpp"

/// \brief Enum zawierający typy okien.
enum class window_type { MENU_INIT, MENU, GAME_START, BOARD_INIT, BOARD, WAIT_INIT, WAIT, GAME_OVER };

/**
 * \brief Klasa głównego okna.
 *
 * Metody z przedrostkami menu, game, wait, board odpowiednio incjalizują, aktualizują oraz rysują
 * poszczególne typy okien.
 */
class main_window
{
	friend class selection;
	friend class game_ships;

public:
	/// \brief Konstruktor klasy głównego okna.
	main_window();

	/// \brief Destruktor klasy głównego okna.
	~main_window();

	/// \brief Główna pętla gry.
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

	/// \brief Typ aktywnego okna.
	window_type current_window = window_type::MENU_INIT;
	/// \brief Obiekt klasy planszy.
	game_board board{{50, 25, 400, 400}, 10};
	/// \brief Obiekt klasy klienta.
	net_client client{
	[this](){
		current_window = window_type::BOARD_INIT;
	},
	[this](uint32_t guess_index){
		client.send_validation(board.add_guess(guess_index));

		if (board.game_over) {
			client.end();
			current_window = window_type::GAME_OVER;
		} else {
			current_window = window_type::BOARD_INIT;
		}
	},
	[this](guess_type type) {
		board.validate_last_guess(type);

		switch (type) {
		case guess_type::miss:
			turn_message = "miss";
			break;

		case guess_type::hit:
			turn_message = "hit";
			break;

		case guess_type::hit_and_sunk:
			turn_message = "hit and sunk";
			break;
		}
	},
	[this]() {
		won = true;
		current_window = window_type::GAME_OVER;
	}
	};

	/// \brief Jeśli true program się zakończy.
	bool quit = false;
	/// \brief Jeśli true wyświetla wygraną.
	bool won = false;
	/// \brief Rozmiar okna.
	static constexpr int window_width = 800, window_height = 450;
	/// \brief Adres serwera.
	std::string hostname = "127.0.0.1";
	/// \brief Port serwera.
	uint16_t port = 60000;

	/// \brief Aktualnie wybrany statek.
	static inline ship_type selected_ship_type = ship_type::submarine;

	/// \brief Zbior zawierający ilości statków danego typu.
	static inline std::map<ship_type, uint8_t> ship_types_count = {
		{ship_type::submarine, 2}, {ship_type::destroyer, 2}, {ship_type::cruiser, 1}, {ship_type::battleship, 1},
	    {ship_type::carrier, 1}
	};

	/// \brief Wybór/ilość 1-masztowców.
	selection select_submarines{{515, 210}, "Submarines: %d ", ship_type::submarine};
	/// \brief Wybór/ilość 2-masztowców.
	selection select_destroyers{{515, 240}, "Destroyers: %d ", ship_type::destroyer};
	/// \brief Wybór/ilość 3-masztowców.
	selection select_cruisers{{515, 270}, "Cruisers: %d   ", ship_type::cruiser};
	/// \brief Wybór/ilość 4-masztowców.
	selection select_battleships{{515, 300}, "Battleships: %d", ship_type::battleship};
	/// \brief Wybór/ilość 5-masztowców.
	selection select_carriers{{515, 330}, "Carriers: %d   ", ship_type::carrier};

	/// \brief Guzik sprawdzenia planszy i połączenia z serwerem.
	button connect_btn{{515, 375, 220, 50}, "CONNECT"};

	/// \brief Status tury wyświetlany graczowi.
	std::string turn_message;
};
