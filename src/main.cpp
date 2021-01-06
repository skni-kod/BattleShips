#ifdef CLIENT
#include "main_window/main_window.hpp"

int main()
{
	main_window game;
	game.loop();
	return 0;
}
#endif
#ifdef SERVER
#include "net/server_impl.hpp"
int main()
{
	net_server s(60000);
	s.start();
	while (1) {
		s.update(SIZE_MAX, true);
	}
	return 0;
}
#endif