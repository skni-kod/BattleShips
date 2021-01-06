#include "selection.hpp"

const int MIN_COUNTER = 0;
const int MAX_COUNTER = 10;

selection::selection(Vector2 vec, std::string selection_label_format, uint8_t &selection_counter)
    : label_format(selection_label_format), btn1({}, "-"), btn2({}, "+"), counter(selection_counter)
{
	bounds.x = vec.x;
	bounds.y = vec.y;
	bounds.width = static_cast<float>(label_format.size() * static_cast<size_t>(font_size/2));
	bounds.height = static_cast<float>(font_size);

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
	btn1.update();
	btn2.update();
}

void selection::draw()
{
	DrawText(TextFormat(label_format.c_str(), counter), static_cast<int>(bounds.x), static_cast<int>(bounds.y), font_size, label_color);

	btn1.draw();
	btn2.draw();
}
