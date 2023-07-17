#include <iostream>
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"
#include "main.h"

#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

SDL_Window* g_Window;
SDL_GLContext g_OpenGLContext;
b8 g_GameIsRunning = true;
PerformanceData g_PerData;

i32 main(){
	if(!Init()) return 1;
	
	Shader shader("shaders/shader.vs", "shaders/shader.fs");

	f32 vertices[] = {
		 0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f
	};

	u32 indices[] = {
		0, 1, 2,
		1, 2, 3
	};

	VertexArray vao;
	vao.Bind();
	VertexBuffer vbo(vertices, sizeof(vertices));
	IndexBuffer ibo(indices, sizeof(indices));
	vao.DefineVBOLayout(&vbo, 0, 3, 24, 0);
	vao.DefineVBOLayout(&vbo, 1, 3, 24, 3);

	while(g_GameIsRunning){
		HandleInput();
		SimulateWorld();

		glClearColor(0.4f, 0.2f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.Use();
		vao.Bind();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		SDL_GL_SwapWindow(g_Window);
	}

	shader.Delete();
	vbo.Delete();
	ibo.Delete();
	vao.Delete();

	Quit();
}

b8 Init(){
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

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

	if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)){
		std::cout << "Couldn't initialize GLAD and load OpenGL function pointers!\n" << std::endl;
		return false;
	}

	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;

	SDL_GL_SetSwapInterval(1); //enable vsync

	return true;
}

void HandleInput(){
	UpdateFrametimeInWindowTitle(g_PerData);

	SDL_Event e;
	while(SDL_PollEvent(&e)){
		const u8* keyboardState = SDL_GetKeyboardState(NULL);

		if(e.type == SDL_QUIT || keyboardState[SDL_SCANCODE_ESCAPE]){
			g_GameIsRunning = false;
		}
	}
}

void SimulateWorld(){ 
}

void RenderGraphics(){
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
	SDL_GL_DeleteContext(g_OpenGLContext);
	SDL_DestroyWindow(g_Window);
	SDL_Quit();
}