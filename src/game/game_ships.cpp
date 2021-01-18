#include "game_ships.hpp"
#include "game_board.hpp"
#include "../main_window/main_window.hpp"

/** \memberof ship
 * \brief Konstruktor statku.
 * \param type Typ nowego statku.
 * \param start_index Indeks startowy statku.
 * \param is_vertical Jeśli prawdziwy statek jest w pozycji wertykalnej.
 * Przypisuje nowe indeksy pola indexes na false.
 */
ship::ship(ship_type type, uint32_t start_index, bool is_vertical) : vertical(is_vertical), type(type)
{
	if (vertical) {
		for (uint32_t i = 0; i < length(); i++)
			indexes[start_index + i*game_board::num_cells] = false;
	} else {
		for (uint32_t i = 0; i < length(); i++)
			indexes[start_index + i] = false;
	}
}

/** \memberof ship
 * \brief Metoda rysująca statku.
 * Rysuje każdy indeks statku na ekranie jako kwadrat o kolorze limonkowym.
 */
void ship::draw() const
{
	for (auto [i, _] : indexes)
		DrawRectangleV({game_board::cells[i].x + 5, game_board::cells[i].y + 5},
			       {game_board::cell_w - 10, game_board::cell_h - 10}, LIME);
}

/** \memberof game_ships
 * \brief Metoda sprawdzająca.
 * \param index Indeks do sprawdzenia.
 * \param ignore_last Jeśli prawdziwa to ostatni indeks zostanie pominięty.
 * Sprawdza indeksy wszystkich statków czy zawierają szukany indeks.
 */
bool game_ships::check(uint32_t index, bool ignore_last)
{
	for (auto it = ships.begin(); it != ships.end() - (ignore_last ? 1 : 0); it++) {
		if (it->indexes.contains(index))
			return true;
	}
	return false;
}

/** \memberof game_ships
 * \brief Funkcja sprawdzająca trafienie.
 * \param index Indeks do sprawdzenia.
 * Sprawdza indeksy wszystkich statków jeśli istnieją ustawia ich wartości na true.
 */
bool game_ships::was_hit(uint32_t index)
{
	for (auto &s : ships) {
		if (s.indexes.contains(index)) {
			s.indexes[index] = true;
			return s.indexes[index];
		}
			
	}
	return false;
}

/** \memberof game_ships
 * \brief Funkcja zwracjąca ostatnio zatopione indeksy.
 * \param index Indeks do sprawdzenia.
 * Sprawdza i ustawia zatopione statki, jeśli dany statek nie jest zatopiony ale wszystkie jego indeksy
 * są ustawione na true to jego indeksy zostają zwrócone.
 */
std::vector<uint32_t> game_ships::get_sunk_indexes()
{
	std::vector<uint32_t> sunk_indexes;
	for (auto &s : ships) {
		if (s.sunk)
			continue;

		uint32_t count = 0;
		for (auto [_, hit] : s.indexes)
			if (hit)
				count++;

		if (count == s.indexes.size()) {
			s.sunk = true;
			for (auto [i, _] : s.indexes)
				sunk_indexes.push_back(i);
			break;
		}
	}
	return sunk_indexes;
}

/** \memberof game_ships
 * \brief Funkcja sprawdzająca czy wszystkie statki są zatopione.
 */
bool game_ships::all_sunk()
{
	for (auto &s : ships)
		if (!s.sunk)
			return false;
	return true;
}

/** \memberof game_ships
 * \brief Funkcja sprawdzająca rozłożenie statków.
 * Sprawdza czy przy układaniu statków zachowano reguły:
 * - ilość statków większa od zera.
 * - ilośc typów statków <= limitowi
 * - dwa statki nie mają tego samego indeksu
 * - jeśli statek wertykalny to czy jego końcowy indeks jest mniejszy niż rozmiar planszy
 * - jeśli statke horyzontalny to czy jego indeksy początkowy i końcowe są na tym samym wierszu
 * - indeksy wokół statku nie są zajęte
 * 
 * Funkcja ustawia także zmienna statyczną klasy game_board która sygnalizuję skończenie układania.
 */
bool game_ships::valid_layout()
{
	if (ships.size() == 0)
		return false;

	auto type_count = [this](ship_type type) {
		uint32_t count = 0;
		for (auto &s : ships) {
			if (type == s.type)
				count++;
		}
		return count;
	};

	for (auto &[type, limit] : main_window::ship_types_count)
		if (type_count(type) > limit)
			return false;

	ship &s = ships.back();
	std::vector<uint32_t> ship_indexes;
	for (auto [i, _] : s.indexes)
		ship_indexes.push_back(i);

	uint32_t x_offset = 1, y_offset = game_board::num_cells;
	bool top = ship_indexes.front() < y_offset;
	bool bottom = ship_indexes.back() >= y_offset * y_offset - y_offset;
	bool left = ship_indexes.front() % 10 == 0;
	bool right = ship_indexes.back() + 1 % 10 == 0;


	// check if overlapping
	for (auto i : ship_indexes)
		if (check(i, true))
			return false;

	if (s.vertical) {
		// check if last index is in bounds
		if (ship_indexes.back() > game_board::num_cells * game_board::num_cells)
			return false;

		// corner checks
		if (!top && !left && check(ship_indexes.front() - x_offset - y_offset))
			return false;
		if (!top && !right && check(ship_indexes.front() + x_offset - y_offset))
			return false;
		if (!bottom && !right && check(ship_indexes.back() + x_offset + y_offset))
			return false;
		if (!bottom && !left && check(ship_indexes.back() - x_offset + y_offset))
			return false;

		// vertical checks
		if (!top && check(ship_indexes.front() - y_offset))
			return false;
		if (!bottom && check(ship_indexes.back() + y_offset))
			return false;

		// horizontal checks
		if (!left)
			for (auto i : ship_indexes)
				if (check(i - x_offset))
					return false;
		if (!right)
			for (auto i : ship_indexes)
				if (check(i + x_offset))
					return false;
	} else {
		// check if last index is in the same row
		int len =
		    (ship_indexes.back() % game_board::num_cells) - (ship_indexes.front() % game_board::num_cells);
		if (len < 0 || len > ship_indexes.size())
			return false;

		// corner checks
		if (!top && !left && check(ship_indexes.front() - x_offset - y_offset))
			return false;
		if (!top && !right && check(ship_indexes.back() + x_offset - y_offset))
			return false;
		if (!bottom && !right && check(ship_indexes.back() + x_offset + y_offset))
			return false;
		if (!bottom && !left && check(ship_indexes.front() - x_offset + y_offset))
			return false;

		// vertical checks
		if (!top)
			for (auto i : ship_indexes)
				if (check(i - y_offset))
					return false;
		if (!bottom)
			for (auto i : ship_indexes)
				if (check(i + y_offset))
					return false;

		// horizontal checks
		if (!left && check(ship_indexes.front() - x_offset))
			return false;
		if (!right && check(ship_indexes.back() + x_offset))
			return false;
	}

	game_board::placement_done = true;
	for (auto &[type, limit] : main_window::ship_types_count)
		if (type_count(type) != limit)
			game_board::placement_done = false;

	return true;
}

/** \memberof game_ships
 * \brief Funkcja dodająca lub usuwająca statek.
 * \param index indeks początkowy dodawanego statku lub indeks istnięjącego
 * Jeśli znaleziono statek z podanym indeksem to usuń ten statek.
 * W przeciwnym wypadku dodajemy nowy statek do wektora oraz sprawdzamy czy rozłożenie statków
 * jest zgodne z regułami.
 */
bool game_ships::update(uint32_t index, bool vertical_placement)
{
	auto ship_to_remove =
	    std::find_if(ships.begin(), ships.end(), [index](ship s) { return s.indexes.contains(index); });
	if (ship_to_remove != ships.end()) {
		ships.erase(ship_to_remove);
		game_board::placement_done = false;
	} else {
		ships.emplace_back(main_window::selected_ship_type, index, vertical_placement);
		if (!valid_layout()) {
			ships.erase(ships.end() - 1);
			return false;
		}
	}
	return true;
}

/** \memberof ship
 * \brief Metoda rysująca statki.
 */
void game_ships::draw() const
{
	for (const auto &s : ships) {
		s.draw();
	}
}