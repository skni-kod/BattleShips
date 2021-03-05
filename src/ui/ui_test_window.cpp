#include "ui_test_window.hpp"
#include <iostream>

ui_test_window::ui_test_window()
{
    InitWindow(window_width, window_height, "UI TEST WINDOW");
    SetTargetFPS(60);
}

ui_test_window::~ui_test_window() { CloseWindow(); }

void ui_test_window::loop()
{
    while (!WindowShouldClose())
    {
        this->draw();
    }
}

void ui_test_window::draw()
{
    BeginDrawing();

    ClearBackground(BLACK);

    ui::set_fill_color(GREEN);
    ui::set_button_label_color(BLACK);
    ui::set_font_size(22);

    if (ui::button(0,0,120,50, "Click me 1!"))
		std::cout << "BUTTON 1 CLICKED" << std::endl; 
	if (ui::button(0,55,120,50, "Click me 2!"))
        std::cout << "BUTTON 2 CLICKED" << std::endl;
	
	static int select_1 = 0;
	static int select_2 = 0;
	static int selected = 0;
	ui::set_selection_label_color(GREEN);
	ui::set_border_color(GREEN);
	ui::set_border_line_thickness(1);
	ui::set_selection_highlight_width(200);

	short a = ui::selection({1, 120}, "Selection 1", selected==1);
	short b = ui::selection({1, 170}, "Selection 2", selected==2);
	if (a == -1) 
    {
		select_1--;
		std::cout << "Selection 1: " << select_1 << std::endl;
	}
	else if (a == 1) {
		select_1++;
		std::cout << "Selection 1: " << select_1 << std::endl;
	}
	else if (a == 2)
		selected = 1;
	if (b == -1) {
		select_2--;
		std::cout << "Selection 2: " << select_2 << std::endl;
	}
	if (b == 1) {
		select_2++;
		std::cout << "Selection 2: " << select_2 << std::endl;
	}
	else if (b == 2)
		selected = 2;

    EndDrawing();
}