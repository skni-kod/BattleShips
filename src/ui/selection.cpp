#include "selection.hpp"
#include "../game/game_ships.hpp"
#include "../main_window/main_window.hpp"

const int MIN_COUNTER = 0;
const int MAX_COUNTER = 10;

selection::selection(Vector2 vec, std::string selection_label_format, ship_type type)
    : label_format(selection_label_format), sel_type(type), sel_btn({}, ""), btn1({}, "-"), btn2({}, "+"), counter(main_window::ship_types_count[type])
{
	bounds.x = vec.x;
	bounds.y = vec.y;
	bounds.width = static_cast<float>(label_format.size() * static_cast<size_t>(font_size/2));
	bounds.height = static_cast<float>(font_size);

	sel_btn.bounds = bounds;
	sel_btn.bg_color = BLANK;
	sel_btn.action = [type]() { main_window::selected_ship_type = type; };

	btn1.bounds.x = vec.x + bounds.width + static_cast<float>(font_size);
	btn1.bounds.y = vec.y;
	btn1.bounds.width = static_cast<float>(btn1.label.size() * static_cast<size_t>(font_size));
	btn1.bounds.height = bounds.height;
	btn1.font_size = font_size;
	btn1.action = [this]() {
		if(counter > MIN_COUNTER) counter--;
	};

	btn2.bounds.x = btn1.bounds.x + btn1.bounds.width + static_cast<float>(font_size)/2;
	btn2.bounds.y = vec.y;
	btn2.bounds.width = static_cast<float>(btn2.label.size() * static_cast<size_t>(font_size));
	btn2.bounds.height = bounds.height;
	btn2.font_size = font_size;
	btn2.action = [this]() {
		if(counter < MAX_COUNTER) counter++;
	};
}

void selection::update()
{
	sel_btn.update();
	if (main_window::selected_ship_type == sel_type)
		highlight = true;
	else
		highlight = false; 


	btn1.update();
	btn2.update();
}

void selection::draw()
{
	if (highlight)
		DrawRectangleLinesEx({bounds.x - 5, bounds.y - 5, bounds.width, bounds.height + 10}, 1, highlight_color);

	DrawText(TextFormat(label_format.c_str(), counter), static_cast<int>(bounds.x), static_cast<int>(bounds.y), font_size, label_color);
	
	sel_btn.draw();
	btn1.draw();
	btn2.draw();
}
