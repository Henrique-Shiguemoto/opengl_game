#include <iostream>

#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glad/glad.h>

#define GAME_NAME "OpenGL Game"
#define WINDOW_WIDTH 800 
#define WINDOW_HEIGHT 600

SDL_Window* g_Window;
SDL_GLContext g_OpenGLContext;
bool g_GameIsRunning = true;

float r = 0.0f;
float g = 0.0f;
float b = 0.0f;

bool Init();
void HandleInput();
void SimulateWorld();
void RenderGraphics();
void Quit();

int main(){
	if(!Init()) return 1;

	unsigned int currentTime = 0;
	unsigned int lastTime = SDL_GetTicks();
	unsigned int framesSinceFrametimeUpdate = 0;
	unsigned int framesPerUpdate = 60;
	unsigned int frameTimeSum = 0;

	while(g_GameIsRunning){
		currentTime = SDL_GetTicks();
		if(framesSinceFrametimeUpdate >= framesPerUpdate){
			std::string newWindowTitle = GAME_NAME" - Frametime (ms): " + std::to_string((float)frameTimeSum / (float)framesPerUpdate);
			SDL_SetWindowTitle(g_Window, newWindowTitle.c_str());
			framesSinceFrametimeUpdate = 0;
			frameTimeSum = 0;
		}
		frameTimeSum += currentTime - lastTime;
		lastTime = currentTime;
		framesSinceFrametimeUpdate++;

		HandleInput();
		SimulateWorld();
		RenderGraphics();
	}

	Quit();
}

bool Init(void){
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		std::cout << "SDL2 Couldn't Initialize!\n" << std::endl;
		return false;
	}

	g_Window = SDL_CreateWindow(GAME_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
	if(!g_Window){
		std::cout << "Couldn't create the window!\n" << std::endl;
		return false;
	}

	int iconWidth, iconHeight, iconChannelCount;
	unsigned char* iconPixels = stbi_load("assets/spaceship.png", &iconWidth, &iconHeight, &iconChannelCount, 0);
	if(!iconPixels){
		std::cout << "Couldn't load icon image" << std::endl;
		return false;
	}

	SDL_Surface* iconSurface = SDL_CreateRGBSurfaceFrom(iconPixels,
                                      iconWidth,
                                      iconHeight,
                                      32,
                                      iconWidth * 4,
                                      0x000000FF,
                                      0x0000FF00,
                                      0x00FF0000,
                                      0xFF000000);
	SDL_SetWindowIcon(g_Window, iconSurface);
	SDL_FreeSurface(iconSurface);
	stbi_image_free(iconPixels);

	g_OpenGLContext = SDL_GL_CreateContext(g_Window);
	if(!g_OpenGLContext){
		std::cout << "Couldn't create OpenGL context!\n" << std::endl;
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)){
		std::cout << "Couldn't initialize GLAD and load OpenGL function pointers!\n" << std::endl;
		return false;
	}
	
	SDL_GL_SetSwapInterval(1); //enable vsync

	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;

	return true;
}

void HandleInput(void){
	SDL_Event e;
	float colorIncrement = 0.01f;
	while(SDL_PollEvent(&e)){
		const unsigned char* keyboardState = SDL_GetKeyboardState(NULL);

		if(e.type == SDL_QUIT || keyboardState[SDL_SCANCODE_ESCAPE]){
			g_GameIsRunning = false;
		}
		if(keyboardState[SDL_SCANCODE_A]){
			r += colorIncrement;
			if(r >= 1.0f){
				r = 0.0f;
			}
		}
		if(keyboardState[SDL_SCANCODE_S]){
			g += colorIncrement;
			if(g >= 1.0f){
				g = 0.0f;
			}
		}
		if(keyboardState[SDL_SCANCODE_D]){
			b += colorIncrement;
			if(b >= 1.0f){
				b = 0.0f;
			}
		}
	}
}

void SimulateWorld(void){

}

void RenderGraphics(void){
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(g_Window);
}

void Quit(){
	SDL_DestroyWindow(g_Window);
	SDL_Quit();
}
