#pragma once

#include "button.hpp"

class selection
{
public:
	std::string label_format;
	Color label_color = DARKGREEN;

	selection(Vector2 vec, std::string selection_label_format, u_int8_t &selection_counter);

	void update();

	void draw();

private:
	Rectangle bounds;
	const int font_size = 20;
	button btn1;
	button btn2;
	u_int8_t &counter;
};
