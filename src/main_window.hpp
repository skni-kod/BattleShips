#pragma once

#include <raylib.h>

#include "game_board.hpp"
#include "net/client_impl.hpp"

class main_window
{
public:
	void loop();

private:
	const int window_width = 600, window_height = 480;
};
