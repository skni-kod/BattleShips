#pragma once

#include "button.hpp"

enum class ship_type;

/// \brief Klasa pola wyboru oraz modyfikacji ilości typu statku.
class selection
{
public:
	/**
	 * \brief Konstruktor.
	 * \param vec Pozycja.
	 * \param btn_label Format etykiety.
	 */
	selection(Vector2 vec, std::string selection_label_format, ship_type type);

	/// \brief Metoda aktualizacyjna.
	void update();

	/// \brief Metoda rysująca.
	void draw();

private:
	/// \brief Granice obszaru.
	Rectangle bounds;
	/// \brief Format etykiety.
	std::string label_format;
	/// \brief Kolor etykiety.
	Color label_color = DARKGREEN;
	/// \brief Kolor zaznaczenia.
	Color highlight_color = GREEN;
	/// \brief Rozmiar czcionki.
	const int font_size = 20;
	/// \brief Jeśli true to pole jest aktualnie wybrane.
	bool highlight = false;

	/// \brief Typ reprezentowanego statku.
	ship_type sel_type;
	/// \brief Guzik wyboru(etykieta).
	button sel_btn;
	/// \brief Guzik "-".
	button btn1;
	/// \brief Guzik "+".
	button btn2;
	/// \brief Licznik statków.
	uint8_t &counter;
};
