#pragma once

#include <algorithm>
#include <cstdint>
#include <raylib.h>
#include <vector>

class game_board
{
public:
	std::vector<uint32_t> selected_cells{};

	game_board(Rectangle rect, uint8_t cells_per_slice);

	void update_highlight();

	void update_selected();

	void draw() const;

private:
	Rectangle bounds;
	float cell_w, cell_h;
	uint8_t num_cells;
	std::vector<Vector2> cells;

	Vector2 mouse_pos;
	bool highlight = false;

	Color normal_color = RAYWHITE;
	Color highlighted_color = BLUE;
	Color selected_color = RED;

	uint32_t to_index(const Vector2 &v) const;
};
