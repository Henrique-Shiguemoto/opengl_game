#include "Game.h"

i32 main(){
	Game game("OpenGL Game", 800, 600, "shaders/shader.vs", "shaders/shader.fs", "assets/spaceship.png");
	game.Quit();
}