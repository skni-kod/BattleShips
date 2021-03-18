#include "ui.hpp"
#include <raylib.h>
#include <string>
#include <cctype>

namespace ui
{
	static properties ui_properties;

	void set_font_size(const int& font_size) { ui_properties.font_size = font_size; }
	void set_border_line_thickness(const int &border_line_thickness) { ui_properties.border_line_thickness = border_line_thickness; }
	void set_fill_color(const Color &fill_color) { ui_properties.fill_color = fill_color; }
	void set_button_label_color(const Color &label_color) { ui_properties.button_label_color = label_color; }
	void set_border_color(const Color &border_color) { ui_properties.border_color = border_color; }
	void set_selection_label_color(const Color &label_color) { ui_properties.selection_label_color = label_color; }
	void set_selection_highlight_width(const int &selection_highlight_width) { ui_properties.selection_highlight_width = selection_highlight_width; }
	void set_blink_time(const double &blink_time) { ui_properties.blink_time = blink_time; }
	void set_text_input_label_color(const Color &text_input_label_color) { ui_properties.text_input_label_color = text_input_label_color; }
	void set_text_input_color(const Color &text_input_color) { ui_properties.text_input_color = text_input_color; }
	void set_focus_required(const bool focus_required) { ui_properties.focus_required = focus_required; }

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

	short selection(const Vector2 &vec, const std::string &selection_label, const bool &highlight)
	{
		Rectangle bounds;
		bounds.x = vec.x;
		bounds.y = vec.y;
		bounds.width = static_cast<float>(MeasureText(selection_label.c_str(), ui_properties.font_size));
		bounds.height = static_cast<float>(ui_properties.font_size);

		if (highlight)
			DrawRectangleLinesEx({bounds.x - 1, bounds.y - 1-static_cast<float>(ui_properties.border_line_thickness)/2, 
								static_cast<float>(ui_properties.selection_highlight_width), bounds.height + 5 + static_cast<float>(ui_properties.border_line_thickness)},
								ui_properties.border_line_thickness, ui_properties.border_color);

		DrawText(selection_label.c_str(), static_cast<int>(bounds.x)+1, static_cast<int>(bounds.y)+1, ui_properties.font_size, ui_properties.selection_label_color);
		
		bounds.x = vec.x +static_cast<float>(ui_properties.selection_highlight_width)+ static_cast<float>(ui_properties.font_size);
		bounds.y = vec.y;
		bounds.width = static_cast<float>(ui_properties.font_size);
		bounds.height = bounds.height;

		short pressed = 0;

		if (button(bounds, "-"))
			pressed = -1;

		bounds.x = bounds.x + bounds.width + static_cast<float>(ui_properties.font_size) / 2;
		bounds.y = vec.y;
		bounds.width = static_cast<float>(ui_properties.font_size);
		bounds.height = bounds.height;

		if (button(bounds, "+"))
			pressed = 1;

		bounds.x = vec.x;
		bounds.y = vec.y;
		bounds.width = static_cast<float>(ui_properties.selection_highlight_width);
		bounds.height = static_cast<float>(ui_properties.font_size);
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
			if (CheckCollisionPointRec(GetMousePosition(), bounds))
				pressed = 2;

		return pressed;
	}

	std::string text_input(const Vector2 &vec, const std::string &ti_label, const std::string &text, const int &max_char_lenghth)
	{
		static double next_blink_time = GetTime() + ui_properties.blink_time;
		static bool blink = false;

		std::string new_text = text;
		char pressed_char = static_cast<char>(GetCharPressed());
		char blink_char = blink ? '_' : ' ';

		if ( !ui_properties.focus_required
		 	|| CheckCollisionPointRec(GetMousePosition(), 
			 	{vec.x, vec.y, 
				 (static_cast<float>(ti_label.size())+static_cast<float>(max_char_lenghth))*static_cast<float>(MeasureText("A", ui_properties.font_size)), 
				 static_cast<float>(ui_properties.font_size)})) {

			if (std::isalnum(pressed_char) && new_text.size()<static_cast<unsigned long>(max_char_lenghth))
				new_text += pressed_char;
			else if (IsKeyPressed(KEY_BACKSPACE) && new_text.size()>0)
				new_text.pop_back();

			if (next_blink_time < GetTime()) {
				blink = !blink;
				next_blink_time = GetTime() + ui_properties.blink_time;
			}
		}

		DrawText(ti_label.c_str(), static_cast<int>(vec.x), static_cast<int>(vec.y), ui_properties.font_size, ui_properties.text_input_label_color);
		DrawText((new_text+blink_char).c_str(), static_cast<int>(vec.x+static_cast<float>(MeasureText(ti_label.c_str(), ui_properties.font_size))), 
				static_cast<int>(vec.y), ui_properties.font_size, ui_properties.text_input_color);
		return new_text;
	}
}
