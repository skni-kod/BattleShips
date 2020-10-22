#include "game.hpp"

void Game::loop()
{
	raylib::Window window(window_width, window_height, "Battleships");

	raylib::Mouse mouse;

	window.SetTargetFPS(60);

	Board board(10, 10, 40, 40);

	while (!window.ShouldClose()) {
		auto mouse_pos = mouse.GetPosition();

		board.update_highlight(mouse_pos);

		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
			board.update_selected(mouse_pos);

		window.BeginDrawing();

		raylib::Color::LightGray.ClearBackground();

		board.draw();

		window.EndDrawing();
	}
}
