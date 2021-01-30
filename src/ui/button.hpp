#pragma once

#include <functional>
#include <raylib.h>
#include <string>

/// \brief Klasa guzika.
class button
{
public:
	/// \brief Granice obszaru.
	Rectangle bounds;
	/// \brief Tekst etykiety.
	std::string label;
	/// \brief Wskaźnik na funkcje wykonywaną po kliknięciu.
	std::function<void()> action;
	/// \brief Rozmiar czcionki.
	int font_size = 20;
	/// \brief Kolor etykiety.
	Color label_color = BLACK;
	/// \brief Kolor guzika.
	Color bg_color = GREEN;

	/**
	 * \brief Konstruktor guzika.
	 * \param rect Obszar guzika
	 * \param btn_label Etykieta guzika.
	 */
	button(Rectangle rect, std::string button_label);

	/// \brief Metoda aktualizacyjna guzika.
	void update();

	/// \brief Metoda rysująca guzik jako prostokąt o danym kolorze oraz wyśrodkowaną etykietę.
	void draw();
};
