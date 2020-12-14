#pragma once

#include <algorithm>
#include <vector>
#include <raylib.h>

class Board
{
public:
	Board(unsigned int nrows, unsigned int ncols, float cell_w, float cell_h, float x = 0, float y = 0);
	void update_highlight(const Vector2 &mouse_pos);
	void update_selected(const Vector2 &mouse_pos);
	inline const std::vector<unsigned int> &get_data() const { return selected_cells; };
	void draw();

private:
	const unsigned int rows, cols;
	Rectangle board;
	std::vector<Rectangle> cells;
	std::vector<unsigned int> selected_cells;
	int highlighted_index = -1;
	Color normal_color = RAYWHITE;
	Color highlighted_color = BLUE;
	Color selected_color = RED;

	inline unsigned int to_row(unsigned int i) const { return i % cols; }

	inline unsigned int to_col(unsigned int i) const { return i / cols; }

	unsigned int to_index(const Vector2 &v) const;
};
