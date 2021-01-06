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
		case window_type::GAME_INIT:
			game_init();
			break;
		case window_type::GAME:
			game_update();
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
		case window_type::GAME:
			game_draw();
			break;
		default:
			break;
		}

		EndDrawing();
	}
}

void main_window::menu_init()
{
	connect_btn.label = "CONNECT";
	connect_btn.action = [&current_window = current_window]() { current_window = window_type::GAME_INIT; };
	current_window = window_type::MENU;
}

void main_window::menu_update()
{
	// update input
	// if clicked fill respective fields
	connect_btn.update();
}

void main_window::menu_draw()
{
	// draw input
	connect_btn.draw();
}

void main_window::game_init()
{
	client.connect(hostname, port);
	current_window = window_type::GAME;
}

void main_window::game_update()
{

	if (IsKeyPressed(KEY_ENTER))
		client.send_board_state();

	// loop returns true on connection error
	if (client.loop())
		quit = true;

	board.update_highlight();

	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
		board.update_selected();
}

void main_window::game_draw() { board.draw(); }
