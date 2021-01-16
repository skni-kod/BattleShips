#include "game_ships.hpp"
#include "game_board.hpp"

ship::ship(ship_type type, uint32_t start_index, bool is_vertical) : vertical(is_vertical), type(type)
{
	if (vertical) {
		for (uint32_t i = 0; i < length(); i++)
			indexes.push_back(start_index + game_board::num_cells + i); // that needs width from game_board
	} else {
		for (uint32_t i = 0; i < length(); i++)
			indexes.push_back(start_index + i);
	}
}

void ship::draw() const
{
	for (auto i : indexes)
		DrawRectangleV({game_board::cells[i].x + 5, game_board::cells[i].y + 5},
			       {game_board::cell_w - 10, game_board::cell_h - 10}, LIME);
}

game_ships::game_ships()
{
	ships.emplace_back(ship_type::submarine, 1);
	ships.emplace_back(ship_type::submarine, 31);
	ships.emplace_back(ship_type::submarine, 42);
	ships.emplace_back(ship_type::submarine, 69);
	ships.emplace_back(ship_type::submarine, 99);
}

void game_ships::place(ship_type type)
{
	//
	//
	type = ship_type::battleship;
}

bool game_ships::check(uint32_t index)
{
	//
	for (const auto &s : ships) {
		for (auto i : s.indexes)
			if (i == index)
				return true;
	}

	return false;
}

void game_ships::update(){}

void game_ships::draw() const
{
	for (const auto &s : ships) {
		s.draw();
	}
}

bool game_ships::verify() { return true; }