#pragma once

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <raylib.h>
#include <vector>

template <uint32_t num_cells> class Board
{
public:
	std::array<bool, num_cells * num_cells> selected_cells{};

	Board(float width, float height, float x = 0, float y = 0)
	    : board{x, y, width, height}, cell_w(width / num_cells), cell_h(height / num_cells)
	{
		uint32_t row = 0, col = 0;
		for (auto &cell : cells) {
			cell = {x + cell_w * static_cast<float>(col), y + cell_h * static_cast<float>(row), cell_w,
				cell_h};

			col++;
			if (col % num_cells == 0) {
				row++;
				col = 0;
			}
		}
	}

	void update_highlight()
	{
		mouse_pos = GetMousePosition();
		if (CheckCollisionPointRec(mouse_pos, board))
			highlight = true;
		else
			highlight = false;
	}

	void update_selected()
	{
		mouse_pos = GetMousePosition();
		if (CheckCollisionPointRec(mouse_pos, board)) {
			uint32_t index = to_index(mouse_pos);
			selected_cells[index] = !selected_cells[index];
		}
	}

	void draw()
	{
		for (auto &cell : cells) {
			DrawRectangleLinesEx(cell, 1, normal_color);
		}

		if (highlight)
			DrawRectangleLinesEx(cells[to_index(mouse_pos)], 2, highlighted_color);

		uint32_t i = UINT32_MAX;
		for (auto &selected : selected_cells) {
			i++;
			if (!selected)
				continue;

			Vector2 top_left = {cells[i].x + 2, cells[i].y + 2};
			Vector2 bottom_right = {cells[i].x + cells[i].width - 2, cells[i].y + cells[i].height - 2};
			DrawLineV(top_left, bottom_right, selected_color);

			Vector2 top_right = {cells[i].x + 2, cells[i].y + cells[i].height - 2};
			Vector2 bottom_left = {cells[i].x + cells[i].width - 2, cells[i].y + 2};
			DrawLineV(top_right, bottom_left, selected_color);

		}
	}

private:
	Rectangle board;
	float cell_w, cell_h;
	std::array<Rectangle, num_cells * num_cells> cells;

	Vector2 mouse_pos;
	bool highlight = false;

	Color normal_color = RAYWHITE;
	Color highlighted_color = BLUE;
	Color selected_color = RED;

	uint32_t to_index(const Vector2 &v) const
	{
		auto row = static_cast<uint32_t>((v.y - board.y) / cell_h);
		auto col = static_cast<uint32_t>((v.x - board.x) / cell_w);
		return col + row * num_cells;
	}
};
