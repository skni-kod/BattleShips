#pragma once

#include <algorithm>
#include <cstdio>
#include <raylib.h>
#include <vector>

template <unsigned int num_cells> class Board
{
public:
	Board(float width, float height, float x = 0, float y = 0)
	    : cell_w(width / num_cells), cell_h(height / num_cells)
	{
		board = {x, y, width, height};
		unsigned int row = 0, col = 0;
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

	unsigned int update_highlight(const Vector2 &mouse_pos)
	{
		// FIXME: print what comes out of to_index
		if (CheckCollisionPointRec(mouse_pos, board)) {
			highlight = true;
			highlighted_cell = cells[to_index(mouse_pos)];
		} else {
			highlight = false;
		}
		return to_index(mouse_pos);
	}

	void update_selected(const Vector2 &mouse_pos)
	{
		if (CheckCollisionPointRec(mouse_pos, board)) {
			auto index = to_index(mouse_pos);
			selected_cells[index] = !selected_cells[index];
		}
	}

	inline const std::array<bool, num_cells * num_cells> &get_data() const { return selected_cells; };

	void draw()
	{
		for (auto &cell : cells) {
			DrawRectangleLinesEx(cell, 1, normal_color);
		}

		if (highlight)
			DrawRectangleLinesEx(highlighted_cell, 2, highlighted_color);

		unsigned int i = 0;
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
	std::array<bool, num_cells * num_cells> selected_cells{};

	bool highlight;
	Rectangle &highlighted_cell = board;

	Color normal_color = RAYWHITE;
	Color highlighted_color = BLUE;
	Color selected_color = RED;

	unsigned int to_index(const Vector2 &v) const
	{
		float row = (v.y - board.y) / board.height * static_cast<float>(num_cells);
		float col = (v.x - board.x) / board.width * static_cast<float>(num_cells);
		return static_cast<unsigned int>(col) + static_cast<unsigned int>(row) * num_cells;
	}
};
