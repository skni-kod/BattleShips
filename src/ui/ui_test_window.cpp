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

    EndDrawing();
}