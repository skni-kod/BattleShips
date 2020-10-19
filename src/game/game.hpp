#pragma once

#include "../../include/Window.hpp"
#include "../../include/Image.hpp"
#include "../../include/Texture2D.hpp"
#include "../../include/Color.hpp"

class Game {
public:
	void loop();

private:
	const int window_width = 800, window_height = 450;
};
