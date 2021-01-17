#include "main_window.hpp"

main_window::main_window()
{
	InitWindow(window_width, window_height, "Battleships");
	SetTargetFPS(60);
}

main_window::~main_window() { CloseWindow(); }

void main_window::loop()
{
	while (!WindowShouldClose() & !quit) {
		switch (current_window) {
		case window_type::MENU_INIT:
			menu_init();
			break;

		case window_type::MENU:
			menu_update();
			break;
		
		case window_type::GAME_START:
			game_start();
			break;

		case window_type::BOARD_INIT:
			board_init();
			break;

		case window_type::BOARD:
			board_update();
			break;

		case window_type::WAIT_INIT:
			wait_init();
			break;

		case window_type::WAIT:
			wait_update();
			break;

		default:
			break;
		}

		BeginDrawing();

		ClearBackground(BLACK);
		switch (current_window) {
		case window_type::MENU:
			menu_draw();
			break;

		case window_type::BOARD:
			board_draw();
			break;

		case window_type::WAIT:
			wait_draw();
			break;

		case window_type::GAME_OVER:
			game_over();
			break;

		default:
			break;
		}

		EndDrawing();
	}
}

void main_window::menu_init()
{
	connect_btn.action = [this]() {
		if (board.is_placement_done())
			current_window = window_type::GAME_START;
	};

	current_window = window_type::MENU;
}

void main_window::menu_update()
{
	board.update_highlight();
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
		board.update_ships(false);
	if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
		board.update_ships(true);

	select_submarines.update();
	select_destroyers.update();
	select_cruisers.update();
	select_battleships.update();
	select_carriers.update();
	connect_btn.update();
}

void main_window::menu_draw()
{
	board.draw();
	select_submarines.draw();
	select_destroyers.draw();
	select_cruisers.draw();
	select_battleships.draw();
	select_carriers.draw();
	connect_btn.draw();
}

void main_window::game_start()
{
	client.connect(hostname, port);
	client.start();
	current_window = window_type::WAIT_INIT;
}

void main_window::wait_init()
{
	board.set_view(view_type::opponent);
	current_window = window_type::WAIT;
}

void main_window::wait_update()
{
	// loop returns true on connection error
	if (client.loop())
		quit = true;
}

void main_window::wait_draw()
{
	const char *message = "Opponents turn";
	DrawText(message, (window_width - MeasureText(message, 20)) / 2, 10, 20, DARKGREEN);

	board.draw();
}

void main_window::board_init()
{
	board.has_guess = true;
	board.set_view(view_type::player);
	current_window = window_type::BOARD;
}

void main_window::board_update()
{
	auto key = GetKeyPressed();

	switch (key)
	{
	case KEY_SPACE: {
		if (!board.has_guess) {
			client.send_guess(board.get_guess());
			current_window = window_type::WAIT_INIT;
		}
	} break;

	case KEY_V: {
		board.toggle_view();
	} break;

	default:
		break;
	}

	// loop returns true on connection error
	if (client.loop())
		quit = true;

	board.update_highlight();

	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
		board.update_selected();
}

void main_window::board_draw()
{
	board.draw();
	const char *message = board.get_message().c_str();
	DrawText(message, (window_width - MeasureText(message, 20)) / 2, 10, 20, DARKGREEN);
}


void main_window::game_over()
{
	const char *message = TextFormat("Game over: you %s!", won ? "won" : "lost");
	DrawText(message, (window_width - MeasureText(message, 40)) / 2, (window_height - 40) / 2, 40, DARKGREEN);
};