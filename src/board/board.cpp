#include "board.hpp"

Board::Board(unsigned int rows, unsigned int cols, unsigned int cell_w, unsigned int cell_h, float x, float y)
    : _rows(rows), _cols(cols), _board(x, y, rows * cell_w, cols * cell_h)
{
	for (int i = 0; i < rows * cols; i++) {
		unsigned int cell_x = to_row(i) * cell_w + _board.x;
		unsigned int cell_y = to_col(i) * cell_h + _board.y;
		_cells.emplace_back(cell_x, cell_y, cell_w, cell_h);
	}
}

void Board::update_highlight(const raylib::Vector2 &mouse_pos)
{
	if (_board.CheckCollision(mouse_pos))
		_highlighted_index = to_index(mouse_pos);
	else
		_highlighted_index = -1;
}

void Board::update_selected(const raylib::Vector2 &mouse_pos)
{
	if (_board.CheckCollision(mouse_pos)) {
		auto index = to_index(mouse_pos);
		auto &v = _selected_cells;
		auto result = std::find(v.begin(), v.end(), index);

		if (result != v.end())
			v.erase(result);
		else
			v.push_back(index);
	}
}

void Board::draw()
{
	for (int i = 0; i < _cells.size(); i++) {
		if (i == _highlighted_index)
			_cells[i].DrawLinesEx(2, _highlighted_color);
		else
			_cells[i].DrawLinesEx(2, _normal_color);
	}

	for (auto &i : _selected_cells) {
		_selected_color.DrawLine(_cells[i].x + 2, _cells[i].y + 2, _cells[i].x + _cells[i].width - 2,
					 _cells[i].y + _cells[i].height - 2);
		_selected_color.DrawLine(_cells[i].x + 2, _cells[i].y + _cells[i].height - 2,
					 _cells[i].x + _cells[i].width - 2, _cells[i].y + 2);
	}
}

unsigned int Board::to_index(const raylib::Vector2 &v) const
{
	unsigned int row = (v.y - _board.y) / _board.height * _rows;
	unsigned int col = (v.x - _board.x) / _board.width * _cols;
	return col + row * _cols;
}
