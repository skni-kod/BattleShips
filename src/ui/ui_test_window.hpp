#pragma once

#include "ui.hpp"

class ui_test_window
{
    public:
	ui_test_window();

	~ui_test_window();

	void loop();

    private:
    static constexpr int window_width = 800, window_height = 450;

    void draw();

};