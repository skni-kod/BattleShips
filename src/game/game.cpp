#include "game.hpp"

void Game::loop() {
	raylib::Window window(window_width, window_height, "Battleships");

	SetTargetFPS(60);

	while (!window.ShouldClose()) {
		BeginDrawing();

		raylib::Color::RayWhite.ClearBackground();

		DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

		EndDrawing();
	}
}
