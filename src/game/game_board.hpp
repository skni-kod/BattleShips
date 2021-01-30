#pragma once

#include <algorithm>
#include <cstdint>
#include <unordered_map>
#include <raylib.h>
#include <vector>

#include "guess_type.hpp"
#include "game_ships.hpp"

/// \brief Enum zawierający typy widoku planszy.
enum class view_type { placement, player, opponent };

/// \brief Klasa planszy.
class game_board
{
	friend class ship;
	friend class game_ships;

public:
	bool has_guess = false;
	bool game_over = false;

	/**
	 * \brief Konstruktor planszy.
	 * \param rect Obszar komórek planszy.
	 * \param cells_per_slice Rozmiar planszy.
	 */
	game_board(Rectangle rect, uint32_t cells_per_slice);

	/// \brief Metoda przypisująca rodzaj wyświetlanego widoku.
	inline void set_view(view_type desired_view) { view = desired_view; }

	/// \brief Metoda przełączająca widok.
	void toggle_view();

	/// \brief Metoda zwracająca wyświetlany widok jako stałą znakową.
	const char *get_view_cstr() const;

	/// \brief Metoda dodająca/usuwająca staki.
	bool update_ships(bool vertical_placement);

	/// \brief Metoda sprawdzająca i zwracająca skończenie rozkładania statków.
	bool is_placement_done();


	/// \brief Metoda dodająca próbę odgadnięcia.
	guess_type add_guess(uint32_t guess_index);

	/// \brief Metoda przypisująca rodzaj próby zgadnięcia.
	inline void validate_last_guess(guess_type type) { guesses[selected_cell] = type; }

	/// \brief Metoda dodająca i zwracająca próbę odgadnięcia na indeksie zaznaczonej komórki.
	uint32_t get_guess_index();

	/// \brief Metoda aktualizująca indeks pod myszka oraz czy zaznaczenie jest aktywne.
	void update_highlight();

	/// \brief Metoda usuwająca próbę odgadnięcia na indeksie zaznaczonej komórki.
	void update_selected();

	/// \brief Metoda rysująca plansze w zależności od widoku.
	void draw() const;

private:
	/// \brief Granice obszaru planszy.
	Rectangle bounds;

	/// \brief Ilość komórek w jednym rzędzie/kolumnie.
	static uint32_t num_cells;
	/// \brief Rozmiar komórki.
	static float cell_size;
	/// \brief Wektor zawierający granicę obszarów wszystkich komórek.
	inline static std::vector<Vector2> cells{};

	/// \brief Typ aktywnego widoku.
	view_type view = view_type::placement;
	/// \brief Jeśli true rozkładanie zostało zakończone.
	static inline bool placement_done = false;

	/// \brief Zbiór indeksów z przypisanymi typami zgadnięcia gracza.
	std::unordered_map<uint32_t, guess_type> guesses;
	/// \brief Zbiór indeksów z przypisanymi typami zgadnięcia oponenta.
	std::unordered_map<uint32_t, guess_type> opponent_guesses;

	/// \brief Obiekt menedżera klas.
	game_ships ships;

	/// \brief Indeks pod kursorem.
	uint32_t mouse_index;
	/// \brief Indeks zaznaczenia.
	uint32_t selected_cell = 0;
	/// \brief Jeśli true to wyróżnienia jest aktywne.
	bool highlight = false;

	/// \brief Kolor komórki.
	Color normal_color = DARKGREEN;
	/// \brief Kolor wyróżnienia.
	Color highlighted_color = LIME;
	/// \brief Kolor zaznaczenia.
	Color selected_color = GREEN;

	/// \brief Metoda zamieniająca Vector2 na indeks.
	uint32_t to_index(const Vector2 &v) const;

	/// \brief Metoda rysująca ukośną linie.
	void draw_line(uint32_t cell_index) const;

	/// \brief Metoda rysująca krzyżyk.
	void draw_cross(uint32_t cell_index) const;

	/// \brief Metoda rysująca kółko.
	void draw_circle(uint32_t cell_index) const;
};
