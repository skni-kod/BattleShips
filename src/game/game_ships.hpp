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

	bool check(uint32_t index);

	void draw() const;

private:
	ship_type type;
	std::map<uint32_t, bool> indexes;
	bool vertical;
};

class game_ships
{
public:
	game_ships();

	void place(ship_type type);

	bool check(uint32_t index);

	void update();

	void draw() const;

	void draw_highlight() const;

private:
	std::vector<ship> ships{};

	bool verify();
};
