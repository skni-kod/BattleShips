#include "board.hpp"

Board::Board(unsigned int rows, unsigned int cols, unsigned int cell_w, unsigned int cell_h, float x, float y)
    : rows(rows), cols(cols), board(x, y, rows * cell_w, cols * cell_h)
{
	for (int i = 0; i < rows * cols; i++) {
		unsigned int cell_x = to_row(i) * cell_w + board.x;
		unsigned int cell_y = to_col(i) * cell_h + board.y;
		cells.emplace_back(cell_x, cell_y, cell_w, cell_h);
	}
}

void Board::update_highlight(const raylib::Vector2 &mouse_pos)
{
	if (board.CheckCollision(mouse_pos))
		highlighted_index = to_index(mouse_pos);
	else
		highlighted_index = -1;
}

void Board::update_selected(const raylib::Vector2 &mouse_pos)
{
	if (board.CheckCollision(mouse_pos)) {
		auto index = to_index(mouse_pos);
		auto &v = selected_cells;
		auto result = std::find(v.begin(), v.end(), index);

		if (result != v.end())
			v.erase(result);
		else
			v.push_back(index);
	}
}

void Board::draw()
{
	for (int i = 0; i < cells.size(); i++) {
		if (i == highlighted_index)
			cells[i].DrawLinesEx(2, highlighted_color);
		else
			cells[i].DrawLinesEx(2, normal_color);
	}

	for (auto &i : selected_cells) {
		selected_color.DrawLine(cells[i].x + 2, cells[i].y + 2, cells[i].x + cells[i].width - 2,
					 cells[i].y + cells[i].height - 2);
		selected_color.DrawLine(cells[i].x + 2, cells[i].y + cells[i].height - 2,
					 cells[i].x + cells[i].width - 2, cells[i].y + 2);
	}
}

unsigned int Board::to_index(const raylib::Vector2 &v) const
{
	unsigned int row = (v.y - board.y) / board.height * rows;
	unsigned int col = (v.x - board.x) / board.width * cols;
	return col + row * cols;
}
