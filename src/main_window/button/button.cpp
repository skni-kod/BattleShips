#include "button.hpp"

button::button(Rectangle rect) : bounds(rect)
{
}

void button::update()
{
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
		if (CheckCollisionPointRec(GetMousePosition(), bounds))
			action();
}

void button::draw()
{
	DrawRectangleRec(bounds, bg_color);

	int text_width = MeasureText(label.c_str(), font_size);
	int x = static_cast<int>(bounds.x + (bounds.width - static_cast<float>(text_width))/2);
	int y = static_cast<int>(bounds.y + (bounds.height - static_cast<float>(font_size))/2);

	DrawText(label.c_str(), x, y, font_size, label_color);
}
