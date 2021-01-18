#include "button.hpp"

/**
 * \brief Konstruktor guzika.
 * \param rect Obszar guzika
 * \param btn_label Etykieta guzika.
 */
button::button(Rectangle rect, std::string btn_label) : bounds(rect), label(btn_label)
{
}

/**
 * \brief Metoda aktualizacyjna guzika.
 */
void button::update()
{
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
		if (CheckCollisionPointRec(GetMousePosition(), bounds))
			action();
}

/**
 * \brief Metoda rysująca guzik jako prostokąt o danym kolorze oraz wyśrodkowana etykieta.
 */
void button::draw()
{
	DrawRectangleRec(bounds, bg_color);

	int text_width = MeasureText(label.c_str(), font_size);
	int x = static_cast<int>(bounds.x + (bounds.width - static_cast<float>(text_width))/2);
	int y = static_cast<int>(bounds.y + (bounds.height - static_cast<float>(font_size))/2);

	DrawText(label.c_str(), x, y, font_size, label_color);
}
