#pragma once

#include <algorithm>
#include <cstdint>
#include <raylib.h>
#include <string>
#include <vector>

#include "game_ships.hpp"

enum class view_type { placement, player, opponent };

struct guess {
	uint32_t index;
	bool good;
};

class game_board
{
	friend class ship;
	friend class game_ships;

public:
	bool has_guess = false;

	game_board(Rectangle rect, uint32_t cells_per_slice);

	void set_view(view_type desired_view) { view = desired_view; }

	void toggle_view();

	bool update_ships(bool vertical_placement);

	inline bool is_placement_done()
	{
		ships.valid_layout();
		return placement_done;
	}

	bool add_guess(uint32_t guess);

	uint32_t get_guess();

	inline void validate_guess(bool good) { guesses.back().good = good; }

	inline std::string &get_message() { return message; }

	inline bool is_game_over() { return game_over; }

	void update_highlight();

	void update_selected();

	void draw() const;

private:
	Rectangle bounds;
	static uint32_t num_cells;
	static float cell_w, cell_h;
	inline static std::vector<Vector2> cells{};
	std::vector<guess> guesses;
	std::vector<guess> opponent_guesses;
	uint32_t selected_cell = 0;
	game_ships ships;
	view_type view = view_type::placement;
	static inline bool placement_done = false;
	bool game_over = false;
	std::string message = "";

	uint32_t mouse_index;
	bool highlight = false;

	Color normal_color = DARKGREEN;
	Color highlighted_color = LIME;
	Color selected_color = GREEN;

	void draw_line(uint32_t cell_index) const;
	void draw_cross(uint32_t cell_index) const;
	uint32_t to_index(const Vector2 &v) const;
};
