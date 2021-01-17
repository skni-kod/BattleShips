#pragma once

#include "button.hpp"

enum class ship_type;

class selection
{
public:
	std::string label_format;
	Color label_color = DARKGREEN;

	selection(Vector2 vec, std::string selection_label_format, ship_type type);

	void update();

	void draw();

private:
	Rectangle bounds;
	const int font_size = 20;
	bool outline = false;

	ship_type sel_type;
	button sel_btn;
	button btn1;
	button btn2;
	uint8_t &counter;
};
