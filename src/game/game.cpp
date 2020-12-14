#include "game.hpp"

void Game::loop()
{
	InitWindow(window_width, window_height, "Battleships");
	SetTargetFPS(60);

	Board board(10, 10, 40, 40, 2.5f*40, 40);

	while (!WindowShouldClose()) {
		auto mouse_pos = GetMousePosition();

		board.update_highlight(mouse_pos);

		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
			board.update_selected(mouse_pos);

		BeginDrawing();

		ClearBackground(LIGHTGRAY);

		board.draw();

		EndDrawing();
	}
}
