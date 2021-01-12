#include "game_board.hpp"

game_board::game_board(Rectangle rect, uint8_t cells_per_slice)
	: bounds{rect.x, rect.y, rect.width - 1, rect.height - 1}, num_cells(cells_per_slice)
{
	cell_w = rect.width / num_cells;
	cell_h = rect.height / num_cells;

	cells.resize(num_cells * num_cells);

	uint32_t row = 0, col = 0;
	for (auto &cell : cells) {
		cell = {rect.x + cell_w * static_cast<float>(col), rect.y + cell_h * static_cast<float>(row)};

		col++;
		if (col % num_cells == 0) {
			row++;
			col = 0;
		}
	}
}

void game_board::update_highlight()
{
	mouse_pos = GetMousePosition();
	if (CheckCollisionPointRec(mouse_pos, bounds))
		highlight = true;
	else
		highlight = false;
}

void game_board::update_selected()
{
	if (CheckCollisionPointRec(mouse_pos, bounds)) {
		auto cell_index = to_index(mouse_pos);

		if (const auto pos = std::find(selected_cells.begin(), selected_cells.end(), cell_index); pos != std::end(selected_cells))
			selected_cells.erase(pos);
		else
			selected_cells.push_back(cell_index);

	}
}

void game_board::draw() const
{
	for (auto &cell : cells) {
		DrawRectangleLinesEx({cell.x, cell.y, cell_w, cell_h}, 2, normal_color);
	}

	if (highlight) {
		auto i = to_index(mouse_pos);
		DrawRectangleLinesEx({cells[i].x, cells[i].y, cell_w, cell_h}, 2, highlighted_color);
	}

	for (auto &i : selected_cells) {
		Vector2 top_left = {cells[i].x + 2, cells[i].y + 2};
		Vector2 bottom_right = {cells[i].x + cell_w - 2, cells[i].y + cell_h - 2};
		DrawLineV(top_left, bottom_right, selected_color);

		Vector2 top_right = {cells[i].x + 2, cells[i].y + cell_h - 2};
		Vector2 bottom_left = {cells[i].x + cell_w - 2, cells[i].y + 2};
		DrawLineV(top_right, bottom_left, selected_color);
	}
}

uint32_t game_board::to_index(const Vector2 &v) const
{
	auto row = static_cast<uint32_t>((v.y - bounds.y) / cell_h);
	auto col = static_cast<uint32_t>((v.x - bounds.x) / cell_w);
	return col + row * num_cells;
}
