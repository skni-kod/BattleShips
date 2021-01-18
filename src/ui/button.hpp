#pragma once

#include <functional>
#include <raylib.h>
#include <string>

/**
 * \brief Klasa guzika.
 */
class button
{
public:
	Rectangle bounds;
	std::string label;
	std::function<void()> action;
	int font_size = 20;
	Color label_color = BLACK;
	Color bg_color = GREEN;

	button(Rectangle rect, std::string button_label);

	void update();

	void draw();
};
