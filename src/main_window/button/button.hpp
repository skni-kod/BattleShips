#pragma once

#include <raylib.h>
#include <functional>
#include <string>

class button {
public:
	std::function<void()> action;
	std::string label;
	int font_size = 20;
	Color label_color = BLACK;
	Color bg_color = GREEN;

	button(Rectangle rect);

	void update();

	void draw();

private:
	Rectangle bounds;
};
