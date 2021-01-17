#include "game_ships.hpp"
#include "game_board.hpp"
#include "../main_window/main_window.hpp"

ship::ship(ship_type type, uint32_t start_index, bool is_vertical) : vertical(is_vertical), type(type)
{
	if (vertical) {
		for (uint32_t i = 0; i < length(); i++)
			indexes[start_index + game_board::num_cells*i] = false;
	} else {
		for (uint32_t i = 0; i < length(); i++)
			indexes[start_index + i] = false;
	}
}

bool ship::check(uint32_t index)
{
	if (indexes.contains(index)) {
		indexes[index] = true;
		return true;
	}
	return false;
}

void ship::draw() const
{
	for (auto [i, _] : indexes)
		DrawRectangleV({game_board::cells[i].x + 5, game_board::cells[i].y + 5},
			       {game_board::cell_w - 10, game_board::cell_h - 10}, LIME);
}

bool game_ships::check(uint32_t index)
{
	for (auto &s : ships) {
		if (s.check(index))
			return true;
	}

	return false;
}

bool game_ships::update(uint32_t index, bool vertical_placement)
{
	auto ship_to_remove = std::find_if(ships.begin(), ships.end(), [index](ship s) { return s.indexes.contains(index); });
	if (ship_to_remove != ships.end())
		ships.erase(ship_to_remove);
	else {
		ships.emplace_back(main_window::selected_ship_type, index, vertical_placement);
		if (!valid_layout()) {
			ships.erase(ships.end() - 1);
			return false;
		}
	}
	return true;
}

void game_ships::draw() const
{
	for (const auto &s : ships) {
		s.draw();
	}
}

bool game_ships::valid_layout()
{
	auto type_count = [this](ship_type type) {
		uint8_t count = 0;
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


	if (s.vertical) {
		int len =
		    (ship_indexes.back() / game_board::num_cells) - (ship_indexes.front() / game_board::num_cells);
		if (len < 0 || len > ship_indexes.size())
			return false;

		if (!top && check(ship_indexes.front() - y_offset))
			return false;
		if (!bottom && check(ship_indexes.back() + y_offset))
			return false;

		if (!left)
			for (auto i : ship_indexes)
				if (check(i - x_offset))
					return false;
		if (!right)
			for (auto i : ship_indexes)
				if (check(i + x_offset))
					return false;
	} else {
		int len =
		    (ship_indexes.back() % game_board::num_cells) - (ship_indexes.front() % game_board::num_cells);
		if (len < 0 || len > ship_indexes.size())
			return false;

		if (!top)
			for (auto i : ship_indexes)
				if (check(i - y_offset))
					return false;
		if (!bottom)
			for (auto i : ship_indexes)
				if (check(i + y_offset))
					return false;


		if (!left && check(ship_indexes.front() - x_offset))
			return false;
		if (!right && check(ship_indexes.back() + x_offset))
			return false;
	}

	if (!top && !left && check(ship_indexes.front() - x_offset - y_offset))
		return false;
	if (!top && !right && check(ship_indexes.front() + x_offset - y_offset))
		return false;
	if (!bottom && !right && check(ship_indexes.front() + x_offset + y_offset))
		return false;
	if (!bottom && !left && check(ship_indexes.front() - x_offset + y_offset))
		return false;

	return true;
}