#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <raylib.h>
#include <vector>

// type equals the number of cells it takes up
enum class ship_type { submarine = 1, destroyer, cruiser, battleship, carrier };

class ship
{
	friend class game_ships;

public:
	ship(ship_type type, uint32_t start_index, bool is_vertical = false);

	inline uint32_t length() { return static_cast<uint32_t>(type); };

	void draw() const;

private:
	ship_type type;
	std::map<uint32_t, bool> indexes;
	bool vertical;
	bool sunk = false;
};

class game_ships
{
public:
	bool check(uint32_t index);

	bool was_hit(uint32_t index);

	std::vector<uint32_t> get_sunk_indexes();
	
	bool all_sunk();

	bool valid_layout();

	bool update(uint32_t index, bool vertical_placement);

	void draw() const;

private:
	std::vector<ship> ships{};
};
