#pragma once

#include <algorithm>
#include <vector>

#include "Color.hpp"
#include "Rectangle.hpp"
#include "Vector2.hpp"

class Board
{
public:
	Board(unsigned int rows, unsigned int cols, unsigned int cell_w, unsigned int cell_h, float x = 0, float y = 0);
	void update_highlight(const raylib::Vector2 &mouse_pos);
	void update_selected(const raylib::Vector2 &mouse_pos);
	inline const std::vector<unsigned int> &get_data() const { return selected_cells; };
	void draw();

private:
	const unsigned int rows, cols;
	raylib::Rectangle board;
	std::vector<raylib::Rectangle> cells;
	std::vector<unsigned int> selected_cells;
	unsigned int highlighted_index = -1;
	raylib::Color normal_color = raylib::Color::RayWhite;
	raylib::Color highlighted_color = raylib::Color::Blue;
	raylib::Color selected_color = raylib::Color::Red;

	inline int to_row(int i) const { return i % cols; }

	inline int to_col(int i) const { return i / cols; }

	unsigned int to_index(const raylib::Vector2 &v) const;
};
