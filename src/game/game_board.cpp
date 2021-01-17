#include "game_board.hpp"

uint32_t game_board::num_cells = 0;
float game_board::cell_w = 0;
float game_board::cell_h = 0;

game_board::game_board(Rectangle rect, uint32_t cells_per_slice)
    : bounds{rect.x, rect.y, rect.width - 1, rect.height - 1}
{
	num_cells = cells_per_slice;
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

void game_board::toggle_view()
{
	if (view == view_type::player)
		view = view_type::opponent;
	else
		view = view_type::player;
}

bool game_board::update_ships(bool vertical_placement)
{
	if (highlight)
		return ships.update(mouse_index, vertical_placement);

	return false;
}

bool game_board::add_guess(uint32_t guess_index)
{
	guess g{guess_index, ships.was_hit(guess_index)};
	opponent_guesses.push_back(g);

	if (ships.was_sunk())
		message = "Your ship was sunk!";
	else if (g.good)
		message = "Your ship was hit!";
	else
		message = "Opponent missed!";

	if (ships.all_sunk()) game_over = true;
		 
	return g.good;
}

uint32_t game_board::get_guess()
{
	guesses.push_back(guess{selected_cell, false});
	return selected_cell;
}

void game_board::update_highlight()
{
	auto mouse_pos = GetMousePosition();
	mouse_index = to_index(mouse_pos);
	if (CheckCollisionPointRec(mouse_pos, bounds))
		highlight = true;
	else
		highlight = false;
}

void game_board::update_selected()
{
	if (view == view_type::player && highlight) {
		bool not_in_guesses = std::find_if(guesses.begin(), guesses.end(), [this](guess g) {
						    return g.index == mouse_index;
					    }) == guesses.end();

		if (not_in_guesses) {
			if (has_guess) {
				selected_cell = mouse_index;
				has_guess = false;
			} else if (mouse_index == selected_cell) {
				has_guess = true;
			}
		}
	}
}

void game_board::draw() const
{

	for (const auto &cell : cells) {
		DrawRectangleLinesEx({cell.x, cell.y, cell_w, cell_h}, 2, normal_color);
	}

	if (view == view_type::placement) {
		ships.draw();
	} else if (view == view_type::player) {
		if (highlight) {
			DrawRectangleLinesEx({cells[mouse_index].x, cells[mouse_index].y, cell_w, cell_h}, 2, highlighted_color);
		}

		for (const auto &guess : guesses) {
			if (guess.good)
				draw_cross(guess.index);
			else
				draw_line(guess.index);
		}

		if (!has_guess) {
			DrawRectangleLinesEx({cells[selected_cell].x, cells[selected_cell].y, cell_w, cell_h}, 2,
					     selected_color);
		}
	} else if (view == view_type::opponent) {
		ships.draw();

		for (const auto &guess : opponent_guesses) {
			if (guess.good)
				draw_cross(guess.index);
			else
				draw_line(guess.index);
		}
	}
}

uint32_t game_board::to_index(const Vector2 &v) const
{
	auto row = static_cast<uint32_t>((v.y - bounds.y) / cell_h);
	auto col = static_cast<uint32_t>((v.x - bounds.x) / cell_w);
	return col + row * num_cells;
}


void game_board::draw_line(uint32_t cell_index) const
{
	Vector2 top_left = {cells[cell_index].x + 2, cells[cell_index].y + 2};
	Vector2 bottom_right = {cells[cell_index].x + cell_w - 2, cells[cell_index].y + cell_h - 2};
	DrawLineV(top_left, bottom_right, selected_color);
}

void game_board::draw_cross(uint32_t cell_index) const
{
	draw_line(cell_index);

	Vector2 top_right = {cells[cell_index].x + 2, cells[cell_index].y + cell_h - 2};
	Vector2 bottom_left = {cells[cell_index].x + cell_w - 2, cells[cell_index].y + 2};
	DrawLineV(top_right, bottom_left, selected_color);
}
