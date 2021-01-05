#include "main_window.hpp"

void main_window::loop()
{
	InitWindow(window_width, window_height, "Battleships");
	SetTargetFPS(60);

	game_board<10> board(400, 400, 100, 40);

	net_client<10> c(board.selected_cells);
	c.connect("127.0.0.1", 60000);

	bool quit = false;
	while (!WindowShouldClose() & !quit) {
		if (IsKeyPressed(KEY_ENTER))
			c.send_board_state();

		// loop returns true on connection error
		if (c.loop()) quit = true;

		board.update_highlight();

		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
			board.update_selected();

		BeginDrawing();

		ClearBackground(LIGHTGRAY);

		board.draw();

		EndDrawing();
	}

	CloseWindow();
}
