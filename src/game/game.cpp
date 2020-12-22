#include "game.hpp"

void Game::loop()
{
	InitWindow(window_width, window_height, "Battleships");
	SetTargetFPS(60);

	Board<10> board(400, 400, 100, 40);

	std::array<bool, 100> a {
		0,0,0,0,0,0,0,0,0,1,
		0,0,0,0,0,0,0,0,1,0,
		0,0,0,0,0,0,0,1,0,0,
		0,0,0,0,0,0,1,0,0,0,
		0,0,0,0,0,1,0,0,0,0,
		0,0,0,0,1,0,0,0,0,0,
		0,0,0,1,0,0,0,0,0,0,
		0,0,1,0,0,0,0,0,0,0,
		0,1,0,0,0,0,0,0,0,0,
		1,0,0,0,0,0,0,0,0,0,
	};

	board.selected_cells = a;

	while (!WindowShouldClose()) {
		board.update_highlight();

		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
			board.update_selected();

		BeginDrawing();

		ClearBackground(LIGHTGRAY);

		board.draw();

		EndDrawing();
	}
}
