#include "game.hpp"

void Game::loop() {
	raylib::Window window(window_width, window_height, "Battleships");
	window.ToggleFullscreen();

	SetTargetFPS(60);

	Board board(10, 10, 40, 40, 3*40, 40);

	while (!window.ShouldClose()) {
		auto mouse_pos = GetMousePosition();

		board.update_highlight(mouse_pos);

		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
			board.update_selected(mouse_pos);

		BeginDrawing();

		raylib::Color::LightGray.ClearBackground();

		board.draw();

		EndDrawing();
	}
}
