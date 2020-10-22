#pragma once

#include "Color.hpp"
#include "Mouse.hpp"
#include "Window.hpp"

#include "../board/board.hpp"

class Game
{
public:
	void loop();

private:
	const int window_width = 600, window_height = 480;
};
