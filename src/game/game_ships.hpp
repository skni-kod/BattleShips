#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <raylib.h>
#include <vector>

/// \brief Enum zawierający typy statku, liczba przypisana typowi jest długością tego typu statków.
enum class ship_type { submarine = 1, destroyer, cruiser, battleship, carrier };


/**
 * \brief Klasa statku.
 *
 * Klasa reprezentująca statek.
 */
class ship
{
	friend class game_ships;

public:
	/**
	 * \brief Konstruktor statku.
	 *
	 * Przypisuje nowe indeksy pola indexes na false.
	 */
	ship(ship_type type, uint32_t start_index, bool is_vertical = false);

	/// \brief Metoda zwracjąca długość w zależności od typu statku.
	inline uint32_t length() { return static_cast<uint32_t>(type); };

	/**
	 * \brief Metoda rysująca statku.
	 *
	 * Rysuje każdy indeks statku na ekranie jako kwadrat o kolorze limonkowym.
	 */
	void draw() const;

private:
	/// \brief Typ statku.
	ship_type type;
	/// \brief Zbiór indeksów statku z przypisanymi zmiennymi trafienia.
	std::map<uint32_t, bool> indexes;
	/// \brief Jeśli true to statek jest rozłożony pionowo.
	bool vertical;
	/// \brief Jeśli true to statke jest zatopiony.
	bool sunk = false;
};


/**
 * \brief Klasa menadżera statków.
 *
 * Klasa zarządzająca wektorem ze statkami.
 */
class game_ships
{
public:
	/**
	 * \brief Metoda sprawdzająca.
	 *
	 * Sprawdza indeksy wszystkich statków czy zawierają szukany indeks.
	 */
	bool check(uint32_t index, bool ignore_last = false);

	/**
	 * \brief Metoda sprawdzająca trafienie.
	 *
	 * Sprawdza indeksy wszystkich statków jeśli istnieją ustawia ich wartości na true.
	 */
	bool was_hit(uint32_t index);

	/**
	 * \brief Metoda zwracjąca ostatnio zatopione indeksy.
	 *
	 * Sprawdza i ustawia zatopione statki, jeśli dany statek nie jest zatopiony ale wszystkie jego indeksy
	 * są ustawione na true to jego indeksy zostają zwrócone.
	 */
	std::vector<uint32_t> get_sunk_indexes();

	/// \brief Metoda sprawdzająca czy wszystkie statki są zatopione.
	bool all_sunk();

	/**
	 * \brief Metoda sprawdzająca rozłożenie statków.
	 *
	 * Sprawdza czy przy układaniu statków zachowano reguły:
	 * - ilość statków większa od zera,
	 * - ilośc typów statków <= limitowi,
	 * - dwa statki nie mają tego samego indeksu,
	 * - jeśli statek wertykalny to czy jego końcowy indeks jest mniejszy niż rozmiar planszy,
	 * - jeśli statke horyzontalny to czy jego indeksy początkowy i końcowe są na tym samym wierszu,
	 * - indeksy wokół statku nie są zajęte.
	 * Jeśli tak to ustawia zmienna statyczną game_board::is_placement_done,
	 * która sygnalizuję skończenie układania.
	 */
	bool valid_layout();

	/**
	 * \brief Metoda dodająca lub usuwająca statek.
	 * \param index Indeks początkowy dodawanego statku lub indeks istnięjącego.
	 *
	 * Jeśli znaleziono statek z podanym indeksem to usuń ten statek.
	 * W przeciwnym wypadku dodajemy nowy statek do wektora oraz sprawdzamy
	 * czy rozłożenie statków jest zgodne z regułami.
	 */
	bool update(uint32_t index, bool vertical_placement);

	/// \brief Metoda rysująca statki.
	void draw() const;

private:
	/// \brief Wektor zawierający statki.
	std::vector<ship> ships{};
};
