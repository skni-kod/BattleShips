#pragma once

enum class guess_type { miss, hit, hit_and_sunk };

struct guess {
	uint32_t index;
	guess_type type;
};
