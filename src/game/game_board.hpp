#pragma once

#include <algorithm>
#include <cstdint>
#include <unordered_map>
#include <raylib.h>
#include <vector>

#include "guess_type.hpp"
#include "game_ships.hpp"

enum class view_type { placement, player, opponent };

class game_board
{
	friend class ship;
	friend class game_ships;

public:
	bool has_guess = false;
	bool game_over = false;

	game_board(Rectangle rect, uint32_t cells_per_slice);

	inline void set_view(view_type desired_view) { view = desired_view; }

	void toggle_view();

	const char *get_view_cstr() const;

	bool update_ships(bool vertical_placement);

	bool is_placement_done();

	guess_type add_guess(uint32_t guess_index);

	inline void validate_last_guess(guess_type type) { guesses[selected_cell] = type; }

	uint32_t get_guess_index();

	void update_highlight();

	void update_selected();

	void draw() const;

private:
	Rectangle bounds;

	static uint32_t num_cells;
	static float cell_size;
	inline static std::vector<Vector2> cells{};

	view_type view = view_type::placement;
	static inline bool placement_done = false;

	std::unordered_map<uint32_t, guess_type> guesses;
	std::unordered_map<uint32_t, guess_type> opponent_guesses;

	game_ships ships;

	uint32_t mouse_index;
	uint32_t selected_cell = 0;
	bool highlight = false;

	Color normal_color = DARKGREEN;
	Color highlighted_color = LIME;
	Color selected_color = GREEN;

	void draw_line(uint32_t cell_index) const;
	void draw_cross(uint32_t cell_index) const;
	void draw_circle(uint32_t cell_index) const;
	uint32_t to_index(const Vector2 &v) const;
};
