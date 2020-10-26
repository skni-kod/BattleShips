#include "game/game.hpp"
#include "network/network.hpp"

int main()
{
	Game game;

	game.loop();

	network::test();

	return 0;
}
