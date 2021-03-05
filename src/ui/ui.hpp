#pragma once

#include <raylib.h>
#include <string>

namespace ui
{
	namespace
	{
		struct properties 
		{
			int font_size;
			int border_line_thickness;
			Color border_color;
			Color fill_color;
			Color button_label_color;
			Color selection_label_color;
			int selection_highlight_width;
		};
	}

	void set_font_size(const int& font_size);
	void set_border_line_thickness(const int &border_line_thickness);
	void set_fill_color(const Color &fill_color);
	void set_border_color(const Color &border_color);
	void set_button_label_color(const Color &label_color);
	void set_selection_label_color(const Color &label_color);
	void set_selection_highlight_width(const int &selection_highlight_width);

	bool button(const Rectangle &rect, const std::string &button_label);
	bool button(const float &x, const float &y, const float &width, const float &height,
			   const std::string &button_label);
	short selection(const Vector2 &vec, const std::string &selection_label, const bool &highlight);
}