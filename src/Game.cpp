#include "Game.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Game::Game(const char* name, i32 windowWidth, i32 windowHeight){
	this->window = new Window(name, windowWidth, windowHeight, 0);

	if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)){
		std::cout << "Couldn't initialize GLAD and load OpenGL function pointers!\n" << std::endl;
		this->isValid = false;
		return;
	}

	this->SetupCursor("assets/cursor.png");

	stbi_set_flip_vertically_on_load(true);

	this->window->ChangeIcon("assets/spaceship.png");

	glEnable(GL_DEPTH_TEST);
	SDL_GL_SetSwapInterval(1); //enable vsync
	
	this->shader_withTextures = new Shader("shaders/shader_withTextures.vs", "shaders/shader_withTextures.fs");
	this->shader_light = new Shader("shaders/shader_light.vs", "shaders/shader_light.fs");

	this->player = new Player(glm::vec3(-11.0f, 0.75f, 11.0f), glm::vec3(0.5f, 0.5f, 0.5f), 1.0f);
	this->map = new Map(glm::vec3(0.0f), glm::vec3(25.0f, 1.0f, 25.0f));
	this->light = new Light(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.25f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f), glm::vec3(0.1f), glm::vec3(1.0f));
	this->backpackModel = new Model("assets/backpack/backpack.obj");

	this->light->pos += this->player->pos;

	this->isValid = true;
	this->isRunning = true;
	this->isVSyncEnabled = true;
}

void Game::HandleInput(){
	this->UpdatePerformanceData();
}

void Game::SimulateWorld(){
	
}

void Game::RenderGraphics(){
	glClearColor(0.001f, 0.02f, 0.01f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	this->window->SwapWindow();
}

void Game::UpdatePerformanceData(){
	performanceData.currentTimeInSeconds = SDL_GetTicks() / 1000.0f;
	performanceData.deltaTimeInSeconds = performanceData.currentTimeInSeconds - performanceData.lastTimeInSeconds;
	performanceData.lastTimeInSeconds = performanceData.currentTimeInSeconds;
}

void Game::Quit(){
	this->window->Delete();
	SDL_Quit();
}

void Game::SetupCursor(const char* image_path){
	i32 cursorWidth, cursorHeight, cursorChannelCount;
	u8* cursorPixels = stbi_load(image_path, &cursorWidth, &cursorHeight, &cursorChannelCount, 0);
	if(!cursorPixels){
		std::cout << "Couldn't load cursor image" << std::endl;
		return;
	}

	SDL_Surface* cursorSurface = SDL_CreateRGBSurfaceFrom(cursorPixels,
                                      cursorWidth,
                                      cursorHeight,
                                      32,
                                      cursorWidth * 4,
                                      0x000000FF,
                                      0x0000FF00,
                                      0x00FF0000,
                                      0xFF000000);
	SDL_Cursor *cursor = SDL_CreateColorCursor(cursorSurface, 4, 4);
	SDL_SetCursor(cursor);
	SDL_FreeSurface(cursorSurface);
	stbi_image_free(cursorPixels);
}

bool Game::IsOutOfMapBounds(glm::vec3 pos){
	f32 leftBound = this->map->pos.x - 0.5f * this->map->dimension.x;
	f32 rightBound = this->map->pos.x + 0.5f * this->map->dimension.x;
	f32 farBound = this->map->pos.z + 0.5f * this->map->dimension.z;
	f32 nearBound = this->map->pos.z - 0.5f * this->map->dimension.z;
	if((pos.x < leftBound) || (pos.x > rightBound) || (pos.z < nearBound) || (pos.z > farBound)) return true;
	return false;
}

glm::vec3 Game::CalculateWherePlayerNeedsToGoBasedOnNormalizedCursorPosition(f32 x, f32 y){
	// COORDINATES SHOULD BE NORMALIZED ALREADY!!

	// Homogeneous Clip Space
	glm::vec4 rayClip = glm::vec4(x, y, -1.0f, 1.0f);

	// Camera Space Coordinates
	glm::vec4 rayEye = glm::inverse(this->projectionMatrix) * rayClip;
	rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

	// World Coordinates
	glm::vec4 rayWorld4D = glm::inverse(this->viewMatrix) * rayEye;
	glm::vec3 rayWorld = glm::vec3(rayWorld4D.x, rayWorld4D.y, rayWorld4D.z);
	rayWorld = glm::normalize(rayWorld);

	glm::vec3 rayMapTopSurfaceCollision = glm::vec3(this->camera.position_f.x + rayWorld.x * ((this->map->point_from_map.y - this->camera.position_f.y) / rayWorld.y),
													this->map->point_from_map.y,
													this->camera.position_f.z + rayWorld.z * ((this->map->point_from_map.y - this->camera.position_f.y) / rayWorld.y));

	rayMapTopSurfaceCollision = glm::clamp(rayMapTopSurfaceCollision, glm::vec3(this->map->pos.x - 0.5f*this->map->dimension.x, 0.5f, this->map->pos.z - 0.5f * this->map->dimension.z),
																	  glm::vec3(this->map->pos.x + 0.5f*this->map->dimension.x, 0.5f, this->map->pos.z + 0.5f * this->map->dimension.z));

	return glm::vec3(rayMapTopSurfaceCollision.x, rayMapTopSurfaceCollision.y + 0.5f*this->player->dimension.y, rayMapTopSurfaceCollision.z);
}
