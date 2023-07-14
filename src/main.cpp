#include <iostream>
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"
#include "main.h"

SDL_Window* g_Window;
SDL_GLContext g_OpenGLContext;
b8 g_GameIsRunning = true;
PerformanceData g_PerData;

f32 r = 0.0f;
f32 g = 0.0f;
f32 b = 0.0f;

i32 main(){
	if(!Init()) return 1;

	while(g_GameIsRunning){
		HandleInput();
		SimulateWorld();
		RenderGraphics();
	}

	Quit();
}

b8 Init(){
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		std::cout << "SDL2 Couldn't Initialize!\n" << std::endl;
		return false;
	}

	g_Window = SDL_CreateWindow(GAME_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
	if(!g_Window){
		std::cout << "Couldn't create the window!\n" << std::endl;
		return false;
	}

	i32 iconWidth, iconHeight, iconChannelCount;
	u8* iconPixels = stbi_load("assets/spaceship.png", &iconWidth, &iconHeight, &iconChannelCount, 0);
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

void HandleInput(){
	UpdateFrametimeInWindowTitle(g_PerData);

	SDL_Event e;
	f32 colorIncrement = 0.01f;
	while(SDL_PollEvent(&e)){
		const u8* keyboardState = SDL_GetKeyboardState(NULL);

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

void SimulateWorld(){

}

void RenderGraphics(){
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(g_Window);
}

void UpdateFrametimeInWindowTitle(PerformanceData& perfData){
	perfData.currentTime = SDL_GetTicks();
	if(perfData.framesSinceFrametimeUpdate >= perfData.framesPerUpdate){
		std::string newWindowTitle = GAME_NAME" - Frametime (ms): " + std::to_string((f32)perfData.frameTimeSum / (f32)perfData.framesPerUpdate);
		SDL_SetWindowTitle(g_Window, newWindowTitle.c_str());
		perfData.framesSinceFrametimeUpdate = 0;
		perfData.frameTimeSum = 0;
	}
	perfData.frameTimeSum += (perfData.currentTime - perfData.lastTime);
	perfData.lastTime = perfData.currentTime;
	perfData.framesSinceFrametimeUpdate++;
}

void Quit(){
	SDL_DestroyWindow(g_Window);
	SDL_Quit();
}