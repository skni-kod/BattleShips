#pragma once

#include <algorithm>
#include <raylib-cpp/Color.hpp>
#include <raylib-cpp/Rectangle.hpp>
#include <vector>

class Board
{
public:
	Board(unsigned int rows, unsigned int cols, unsigned int cell_w, unsigned int cell_h, float x = 0, float y = 0)
	    : _rows(rows), _cols(cols), _board(x, y, rows * cell_w, cols * cell_h)
	{
		for (int i; i < rows * cols; i++) {
			unsigned int cell_x = to_row(i) * cell_w + _board.x;
			unsigned int cell_y = to_col(i) * cell_h + _board.y;
			_cells.emplace_back(cell_x, cell_y, cell_w, cell_h);
		}
	}

	void update_highlight(const raylib::Vector2 &mouse_pos);
	void update_selected(const raylib::Vector2 &mouse_pos);
	const std::vector<unsigned int> &get_data() const;
	void draw() const;

private:
	const unsigned int _rows, _cols;
	raylib::Rectangle _board;
	std::vector<raylib::Rectangle> _cells;
	std::vector<unsigned int> _selected_cells;
	unsigned int _highlighted_index = -1;
	const raylib::Color _normal_color = raylib::Color::RayWhite;
	const raylib::Color _highlighted_color = raylib::Color::Blue;
	const raylib::Color _selected_color = raylib::Color::Red;

	inline int to_row(int i) const { return i % _cols; }

	inline int to_col(int i) const { return i / _cols; }

	unsigned int to_index(const raylib::Vector2 &v) const;
};
