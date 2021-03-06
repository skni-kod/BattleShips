﻿#include "game_ships.hpp"
#include "game_board.hpp"
#include "../main_window/main_window.hpp"

ship::ship(ship_type t, uint32_t start_index, bool is_vertical) : type(t), vertical(is_vertical)
{
	if (vertical) {
		for (uint32_t i = 0; i < length(); i++)
			indexes[start_index + i*game_board::num_cells] = false;
	} else {
		for (uint32_t i = 0; i < length(); i++)
			indexes[start_index + i] = false;
	}
}

void ship::draw() const
{
	for (auto [i, _] : indexes)
		DrawRectangleV({game_board::cells[i].x + 5, game_board::cells[i].y + 5},
			       {game_board::cell_size - 10, game_board::cell_size - 10}, LIME);
}

bool game_ships::check(uint32_t index, bool ignore_last)
{
	for (auto it = ships.begin(); it != ships.end() - (ignore_last ? 1 : 0); it++) {
		if (it->indexes.contains(index))
			return true;
	}
	return false;
}

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

bool game_ships::all_sunk()
{
	for (auto &s : ships)
		if (!s.sunk)
			return false;
	return true;
}

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
		if (ship_indexes.back() >= game_board::num_cells * game_board::num_cells)
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
		    static_cast<int>((ship_indexes.back() % game_board::num_cells) - (ship_indexes.front() % game_board::num_cells));
		if (len < 0 || len > static_cast<int>(ship_indexes.size()))
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

void game_ships::draw() const
{
	for (const auto &s : ships) {
		s.draw();
	}
}