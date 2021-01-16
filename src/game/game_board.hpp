#pragma once

#include <algorithm>
#include <cstdint>
#include <raylib.h>
#include <vector>

#include "game_ships.hpp"

enum class view_type { player, opponent };

struct guess {
	uint32_t index;
	bool good;
};

class ship;

class game_board
{
	friend class ship;

public:
	bool has_guess = false;

	game_board(Rectangle rect, uint8_t cells_per_slice);

	void set_view(view_type desired_view);

	void toggle_view();

	bool add_guess(uint32_t guess);

	uint32_t get_guess();

	void validate_guess(bool good);

	void update_highlight();

	void update_selected();

	void draw() const;

private:
	Rectangle bounds;
	static uint8_t num_cells;
	static float cell_w, cell_h;
	inline static std::vector<Vector2> cells{};
	std::vector<guess> guesses{};
	std::vector<guess> opponent_guesses{};
	uint32_t selected_cell;
	game_ships ships;
	view_type view = view_type::player;

	Vector2 mouse_pos;
	bool highlight = false;

	Color normal_color = DARKGREEN;
	Color highlighted_color = LIME;
	Color selected_color = GREEN;

	void draw_line(uint32_t cell_index) const;
	void draw_cross(uint32_t cell_index) const;
	uint32_t to_index(const Vector2 &v) const;
};
