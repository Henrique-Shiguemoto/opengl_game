#include "Game.h"

i32 main(){
	Game game("OpenGL Game", WINDOW_WIDTH, WINDOW_HEIGHT, false);
	if(!game.isValid) return 1;

	while(game.isRunning){
		game.HandleInput();
		game.SimulateWorld();
		game.RenderGraphics();
	}

	game.Quit();
}