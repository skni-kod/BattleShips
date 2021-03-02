#include "game_board.hpp"

uint32_t game_board::num_cells = 0;
float game_board::cell_size = 0;

game_board::game_board(Rectangle rect, uint32_t cells_per_slice)
    : bounds{rect.x, rect.y, rect.width - 1, rect.height - 1}
{
	num_cells = cells_per_slice;
	cell_size = rect.width / static_cast<float>(num_cells);
	cells.resize(num_cells * num_cells);

	uint32_t row = 0, col = 0;
	for (auto &cell : cells) {
		cell = {rect.x + cell_size * static_cast<float>(col), rect.y + cell_size * static_cast<float>(row)};

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

const char *game_board::get_view_cstr() const
{
	if (view == view_type::opponent)
		return "opponent's";
	else
		return "player's";
}

bool game_board::update_ships(bool vertical_placement)
{
	if (highlight)
		return ships.update(mouse_index, vertical_placement);

	return false;
}

bool game_board::is_placement_done()
{
	ships.valid_layout();
	return placement_done;
}

guess_type game_board::add_guess(uint32_t guess_index)
{
	if (ships.was_hit(guess_index)) {
		opponent_guesses[guess_index] = guess_type::hit;
	}

	if (auto sunk_indexes = ships.get_sunk_indexes(); sunk_indexes.size() != 0) {
		for (auto i : sunk_indexes)
			opponent_guesses[i] = guess_type::hit_and_sunk;
	}

	if (ships.all_sunk()) game_over = true;

	return opponent_guesses[guess_index];
}

uint32_t game_board::get_guess_index()
{
	guesses[selected_cell] = guess_type::miss;
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
		if (!guesses.contains(mouse_index)) {
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
		DrawRectangleLinesEx({cell.x, cell.y, cell_size, cell_size}, 2, normal_color);
	}

	if (view == view_type::placement) {
		ships.draw();
	} else if (view == view_type::player) {
		if (highlight) {
			DrawRectangleLinesEx({cells[mouse_index].x, cells[mouse_index].y, cell_size, cell_size}, 2, highlighted_color);
		}

		for (const auto &[index, type] : guesses) {
			if (type == guess_type::hit_and_sunk)
				draw_circle(index);
			else if (type == guess_type::hit)
				draw_cross(index);
			else
				draw_line(index);
		}

		if (!has_guess) {
			DrawRectangleLinesEx({cells[selected_cell].x, cells[selected_cell].y, cell_size, cell_size}, 2,
					     selected_color);
		}
	} else if (view == view_type::opponent) {
		ships.draw();

		for (const auto &[index, type] : opponent_guesses) {
			if (type == guess_type::hit_and_sunk)
				draw_circle(index);
			else if (type == guess_type::hit)
				draw_cross(index);
			else
				draw_line(index);
		}
	}
}

uint32_t game_board::to_index(const Vector2 &v) const
{
	auto row = static_cast<uint32_t>((v.y - bounds.y) / cell_size);
	auto col = static_cast<uint32_t>((v.x - bounds.x) / cell_size);
	return col + row * num_cells;
}

void game_board::draw_line(uint32_t cell_index) const
{
	Vector2 top_left = {cells[cell_index].x + 6, cells[cell_index].y + 6};
	Vector2 bottom_right = {cells[cell_index].x + cell_size - 6, cells[cell_index].y + cell_size - 6};
	DrawLineEx(top_left, bottom_right, 2, selected_color);
}

void game_board::draw_cross(uint32_t cell_index) const
{
	draw_line(cell_index);

	Vector2 top_right = {cells[cell_index].x + 6, cells[cell_index].y + cell_size - 6};
	Vector2 bottom_left = {cells[cell_index].x + cell_size - 6, cells[cell_index].y + 6};
	DrawLineEx(top_right, bottom_left, 2, selected_color);
}

void game_board::draw_circle(uint32_t cell_index) const
{
	float radius = cell_size / 2 - 6;
	Vector2 center = {cells[cell_index].x + 6 + radius, cells[cell_index].y + 6 + radius};
	DrawCircleV(center, radius, selected_color);
}
