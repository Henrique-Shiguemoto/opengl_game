#include "Game.h"

i32 main(){
	std::cout << sizeof(char*) << std::endl;

	Game game("OpenGL Game", WINDOW_WIDTH, WINDOW_HEIGHT);
	if(!game.isValid) return 1;

	while(game.isRunning){
		game.HandleInput();
		game.SimulateWorld();
		game.RenderGraphics();
	}

	game.Quit();
}