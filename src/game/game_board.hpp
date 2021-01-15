#pragma once

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <raylib.h>
#include <vector>

struct guess {
	uint32_t index;
	bool good;
};

class game_board
{
public:
	bool has_guess = false;

	game_board(Rectangle rect, uint8_t cells_per_slice);

	void update_highlight();

	void update_selected();

	void draw() const;

	bool add_guess(uint32_t guess);

	uint32_t get_guess();

	void validate_guess(bool good);

private:
	Rectangle bounds;
	float cell_w, cell_h;
	uint8_t num_cells;
	std::vector<Vector2> cells;
	std::vector<guess> guesses{};
	std::vector<guess> opponent_guesses{};
	uint32_t selected_cell;

	Vector2 mouse_pos;
	bool highlight = false;

	Color normal_color = RAYWHITE;
	Color highlighted_color = BLUE;
	Color selected_color = RED;

	void draw_line(uint32_t cell_index) const;
	void draw_cross(uint32_t cell_index) const;
	uint32_t to_index(const Vector2 &v) const;
};
