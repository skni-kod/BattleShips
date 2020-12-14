#include "board.hpp"

Board::Board(unsigned int nrows, unsigned int ncols, float cell_w, float cell_h, float x, float y)
    : rows(nrows), cols(ncols)
{
	board = {x, y, static_cast<float>(rows * cell_w), static_cast<float>(cols * cell_h)};
	for (unsigned int i = 0; i < rows * cols; i++) {
		float cell_x = to_row(i) * cell_w + board.x;
		float cell_y = to_col(i) * cell_h + board.y;
		cells.push_back({cell_x, cell_y, cell_w, cell_h});
	}
}

void Board::update_highlight(const Vector2 &mouse_pos)
{
	if (CheckCollisionPointRec(mouse_pos, board))
		highlighted_index = static_cast<int>(to_index(mouse_pos));
	else
		highlighted_index = -1;
}

void Board::update_selected(const Vector2 &mouse_pos)
{
	if (CheckCollisionPointRec(mouse_pos, board)) {
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
			DrawRectangleLinesEx(cells[i], 2, highlighted_color);
		else
			DrawRectangleLinesEx(cells[i], 2, normal_color);
	}

	for (auto &i : selected_cells) {
		Vector2 top_left = { cells[i].x + 2, cells[i].y + 2 };
		Vector2 bottom_right = { cells[i].x + cells[i].width - 2, cells[i].y + cells[i].height - 2 };
		DrawLineV(top_left, bottom_right, selected_color);

		Vector2 top_right = { cells[i].x + 2, cells[i].y + cells[i].height - 2 };
		Vector2 bottom_left = { cells[i].x + cells[i].width - 2, cells[i].y + 2 };
		DrawLineV(top_right, bottom_left, selected_color);

	}
}

unsigned int Board::to_index(const Vector2 &v) const
{
	float row = (v.y - board.y) / board.height * static_cast<float>(rows);
	float col = (v.x - board.x) / board.width * static_cast<float>(cols);
	return static_cast<unsigned int>(col) + static_cast<unsigned int>(row) * cols;
}
