#include "board.hpp"

Board::Board(unsigned int nrows, unsigned int ncols, unsigned int cell_w, unsigned int cell_h, float x, float y)
    : rows(nrows), cols(ncols), board(x, y, static_cast<float>(rows * cell_w), static_cast<float>(cols * cell_h))
{
	for (unsigned int i = 0; i < rows * cols; i++) {
		unsigned int cell_x = to_row(i) * cell_w + static_cast<unsigned int>(board.x);
		unsigned int cell_y = to_col(i) * cell_h + static_cast<unsigned int>(board.y);
		cells.emplace_back(cell_x, cell_y, cell_w, cell_h);
	}
}

void Board::update_highlight(const raylib::Vector2 &mouse_pos)
{
	if (board.CheckCollision(mouse_pos))
		highlighted_index = static_cast<int>(to_index(mouse_pos));
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
	for (unsigned int i = 0; i < cells.size(); i++) {
		if (i == static_cast<unsigned int>(highlighted_index))
			cells[i].DrawLinesEx(2, highlighted_color);
		else
			cells[i].DrawLinesEx(2, normal_color);
	}

	for (auto &i : selected_cells) {
		raylib::Vector2 top_left = { cells[i].x + 2, cells[i].y + 2 };
		raylib::Vector2 bottom_right = { cells[i].x + cells[i].width - 2, cells[i].y + cells[i].height - 2 };
		selected_color.DrawLine(top_left, bottom_right);

		raylib::Vector2 top_right = { cells[i].x + 2, cells[i].y + cells[i].height - 2 };
		raylib::Vector2 bottom_left = { cells[i].x + cells[i].width - 2, cells[i].y + 2 };
		selected_color.DrawLine(top_right, bottom_left);

	}
}

unsigned int Board::to_index(const raylib::Vector2 &v) const
{
	float row = (v.y - board.y) / board.height * static_cast<float>(rows);
	float col = (v.x - board.x) / board.width * static_cast<float>(cols);
	return static_cast<unsigned int>(col) + static_cast<unsigned int>(row) * cols;
}
