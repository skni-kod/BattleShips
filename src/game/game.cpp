#include "game.hpp"

void Game::loop()
{
	InitWindow(window_width, window_height, "Battleships");
	SetTargetFPS(60);

	Board<10> board(400, 400, 100, 40);

	while (!WindowShouldClose()) {
		auto mouse_pos = GetMousePosition();

		auto mouse_index = board.update_highlight(mouse_pos);

		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
			board.update_selected(mouse_pos);

		BeginDrawing();

		ClearBackground(LIGHTGRAY);

		board.draw();

		DrawText(TextFormat("x: %f\ty: %f\n", static_cast<double>(mouse_pos.x), static_cast<double>(mouse_pos.y)), 20, 20, 40, BLACK);
		DrawText(TextFormat("i: %u", mouse_index), 20, 60, 40, BLACK);

		EndDrawing();
	}
}
