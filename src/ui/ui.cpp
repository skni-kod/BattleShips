#include "ui.hpp"

namespace ui
{
	static properties ui_properties;

	void set_font_size(const int& font_size) { ui_properties.font_size = font_size; }
	void set_border_line_thick(const int &border_line_thickness) { ui_properties.border_line_thickness = border_line_thickness; }
	void set_fill_color(const Color &fill_color) { ui_properties.fill_color = fill_color; }
	void set_button_label_color(const Color &label_color) { ui_properties.button_label_color = label_color; }
	void set_border_color(const Color &border_color) { ui_properties.border_color = border_color; }

	bool button(const float &x, const float &y, const float &width, const float &height,
			   const std::string &button_label)
	{
		return button(Rectangle{x, y, width, height}, button_label);
	}
	
	bool button(const Rectangle &rect, const std::string &button_label)
	{
		int text_width = MeasureText(button_label.c_str(), ui_properties.font_size);

		DrawRectangleRec(rect, ui_properties.fill_color);

		int x = static_cast<int>(rect.x + (rect.width - static_cast<float>(text_width)) / 2);
		int y = static_cast<int>(rect.y + (rect.height - static_cast<float>(ui_properties.font_size)) / 2);

		DrawText(button_label.c_str(), x, y, ui_properties.font_size, ui_properties.button_label_color);

		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
			if (CheckCollisionPointRec(GetMousePosition(), rect))
				return true;

		return false;
	}
	
}