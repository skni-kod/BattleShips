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

		case window_type::BOARD_INIT:
			board_init();
			break;

		case window_type::BOARD:
			board_update();
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

		default:
			break;
		}

		EndDrawing();
	}
}

void main_window::menu_init()
{
	connect_btn.action = [&current_window = current_window]() { current_window = window_type::BOARD_INIT; };

	current_window = window_type::MENU;
}

void main_window::menu_update()
{
	select_submarines.update();
	select_destroyers.update();
	select_cruisers.update();
	select_battleships.update();
	select_carriers.update();
	connect_btn.update();
}

void main_window::menu_draw()
{
	select_submarines.draw();
	select_destroyers.draw();
	select_cruisers.draw();
	select_battleships.draw();
	select_carriers.draw();
	connect_btn.draw();
}

void main_window::board_init()
{
	client.connect(hostname, port);
	client.start();
	current_window = window_type::WAIT;
}

void main_window::board_update()
{
	auto key = GetKeyPressed();

	switch (key)
	{
	case KEY_ENTER: {
		if (!board.has_guess) {
			client.send_guess(board.get_guess());
			current_window = window_type::WAIT;
		} else {
			// warn about not guessing
			std::cout << "Guess befor ending your turn!" << std::endl;
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

void main_window::board_draw() { board.draw(); }

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

	board.set_view(view_type::opponent);
	board.draw();
}
