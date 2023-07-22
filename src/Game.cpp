#include "Game.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Game::Game(const char* name, i32 windowWidth, i32 windowHeight, const char* vertexShaderFilepath, const char* fragmentShaderFilepath, const char* windowIconFilepath){
	this->name = name;
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	if(SDL_Init(SDL_INIT_VIDEO)){
		std::cout << "SDL2 Couldn't Initialize!\n" << std::endl;
		this->isValid = false;
		return;
	}

	this->window = SDL_CreateWindow(this->name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_OPENGL);
	if(!this->window){
		std::cout << "Couldn't create the window!\n" << std::endl;
		this->isValid = false;
		return;
	}

	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_WarpMouseInWindow(this->window, this->windowWidth / 2, this->windowHeight / 2); // fix the mouse in the middle of the screen always

	i32 iconWidth, iconHeight, iconChannelCount;
	u8* iconPixels = stbi_load(windowIconFilepath, &iconWidth, &iconHeight, &iconChannelCount, 0);
	if(!iconPixels){
		std::cout << "Couldn't load icon image" << std::endl;
		this->isValid = false;
		return;
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
	SDL_SetWindowIcon(this->window, iconSurface);
	SDL_FreeSurface(iconSurface);
	stbi_image_free(iconPixels);

	this->context = SDL_GL_CreateContext(this->window);
	if(!this->context){
		std::cout << "Couldn't create OpenGL context!\n" << std::endl;
		this->isValid = false;
		return;
	}

	if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)){
		std::cout << "Couldn't initialize GLAD and load OpenGL function pointers!\n" << std::endl;
		this->isValid = false;
		return;
	}

	glEnable(GL_DEPTH_TEST);
	SDL_GL_SetSwapInterval(1); //enable vsync
	
	this->shader = new Shader(vertexShaderFilepath, fragmentShaderFilepath);

	f32 mapVertices[] = {
		this->mapPosition_f.x - 0.5f*this->mapDimension_f.x, this->mapPosition_f.y - 0.5f*this->mapDimension_f.y, this->mapPosition_f.z + 0.5f*this->mapDimension_f.z, 0.0f, 0.0f, 0.0f, //left-bottom-near
		this->mapPosition_f.x - 0.5f*this->mapDimension_f.x, this->mapPosition_f.y + 0.5f*this->mapDimension_f.y, this->mapPosition_f.z + 0.5f*this->mapDimension_f.z, 0.0f, 0.0f, 1.0f, //left-top-near
		this->mapPosition_f.x - 0.5f*this->mapDimension_f.x, this->mapPosition_f.y + 0.5f*this->mapDimension_f.y, this->mapPosition_f.z - 0.5f*this->mapDimension_f.z, 0.0f, 1.0f, 0.0f, //left-top-far
		this->mapPosition_f.x - 0.5f*this->mapDimension_f.x, this->mapPosition_f.y - 0.5f*this->mapDimension_f.y, this->mapPosition_f.z - 0.5f*this->mapDimension_f.z, 0.0f, 1.0f, 1.0f, //left-bottom-far
		this->mapPosition_f.x + 0.5f*this->mapDimension_f.x, this->mapPosition_f.y - 0.5f*this->mapDimension_f.y, this->mapPosition_f.z + 0.5f*this->mapDimension_f.z, 1.0f, 0.0f, 0.0f, //right-bottom-near
		this->mapPosition_f.x + 0.5f*this->mapDimension_f.x, this->mapPosition_f.y + 0.5f*this->mapDimension_f.y, this->mapPosition_f.z + 0.5f*this->mapDimension_f.z, 1.0f, 0.0f, 1.0f, //right-top-near
		this->mapPosition_f.x + 0.5f*this->mapDimension_f.x, this->mapPosition_f.y + 0.5f*this->mapDimension_f.y, this->mapPosition_f.z - 0.5f*this->mapDimension_f.z, 1.0f, 1.0f, 0.0f, //right-top-far
		this->mapPosition_f.x + 0.5f*this->mapDimension_f.x, this->mapPosition_f.y - 0.5f*this->mapDimension_f.y, this->mapPosition_f.z - 0.5f*this->mapDimension_f.z, 1.0f, 1.0f, 1.0f //right-bottom-far
	};

	u32 mapIndices[] = {
		0, 1, 2,
		0, 2, 3,
		2, 3, 7,
		2, 7, 6,
		7, 6, 5,
		7, 5, 4,
		0, 5, 4,
		0, 1, 5,
		1, 2, 5,
		2, 5, 6,
		0, 3, 4,
		3, 4, 7
	};

	f32 playerVertices[] = {
		this->playerPosition_f.x - 0.5f*this->playerDimension_f.x, this->playerPosition_f.y - 0.5f*this->playerDimension_f.y, this->playerPosition_f.z + 0.5f*this->playerDimension_f.z, 0.0f, 0.0f, 0.0f, //left-bottom-near
		this->playerPosition_f.x - 0.5f*this->playerDimension_f.x, this->playerPosition_f.y + 0.5f*this->playerDimension_f.y, this->playerPosition_f.z + 0.5f*this->playerDimension_f.z, 0.0f, 0.0f, 1.0f, //left-top-near
		this->playerPosition_f.x - 0.5f*this->playerDimension_f.x, this->playerPosition_f.y + 0.5f*this->playerDimension_f.y, this->playerPosition_f.z - 0.5f*this->playerDimension_f.z, 0.0f, 1.0f, 0.0f, //left-top-far
		this->playerPosition_f.x - 0.5f*this->playerDimension_f.x, this->playerPosition_f.y - 0.5f*this->playerDimension_f.y, this->playerPosition_f.z - 0.5f*this->playerDimension_f.z, 0.0f, 1.0f, 1.0f, //left-bottom-far
		this->playerPosition_f.x + 0.5f*this->playerDimension_f.x, this->playerPosition_f.y - 0.5f*this->playerDimension_f.y, this->playerPosition_f.z + 0.5f*this->playerDimension_f.z, 1.0f, 0.0f, 0.0f, //right-bottom-near
		this->playerPosition_f.x + 0.5f*this->playerDimension_f.x, this->playerPosition_f.y + 0.5f*this->playerDimension_f.y, this->playerPosition_f.z + 0.5f*this->playerDimension_f.z, 1.0f, 0.0f, 1.0f, //right-top-near
		this->playerPosition_f.x + 0.5f*this->playerDimension_f.x, this->playerPosition_f.y + 0.5f*this->playerDimension_f.y, this->playerPosition_f.z - 0.5f*this->playerDimension_f.z, 1.0f, 1.0f, 0.0f, //right-top-far
		this->playerPosition_f.x + 0.5f*this->playerDimension_f.x, this->playerPosition_f.y - 0.5f*this->playerDimension_f.y, this->playerPosition_f.z - 0.5f*this->playerDimension_f.z, 1.0f, 1.0f, 1.0f //right-bottom-far
	};

	u32 playerIndices[] = {
		0, 1, 2,
		0, 2, 3,
		2, 3, 7,
		2, 7, 6,
		7, 6, 5,
		7, 5, 4,
		0, 5, 4,
		0, 1, 5,
		1, 2, 5,
		2, 5, 6,
		0, 3, 4,
		3, 4, 7
	};

	this->vaoPlayer = new VertexArray();
	this->vaoPlayer->Bind();
	this->vboPlayer = new VertexBuffer(playerVertices, sizeof(playerVertices));
	this->iboPlayer = new IndexBuffer(playerIndices, sizeof(playerIndices));
	this->vaoPlayer->DefineVBOLayout(vboPlayer, 0, 3, 24, 0);
	this->vaoPlayer->DefineVBOLayout(vboPlayer, 1, 3, 24, 3);
	
	this->vaoMap = new VertexArray();
	this->vaoMap->Bind();
	this->vboMap = new VertexBuffer(mapVertices, sizeof(mapVertices));
	this->iboMap = new IndexBuffer(mapIndices, sizeof(mapIndices));
	this->vaoMap->DefineVBOLayout(vboMap, 0, 3, 24, 0);
	this->vaoMap->DefineVBOLayout(vboMap, 1, 3, 24, 3);

	this->isValid = true;
	this->isRunning = true;
	this->isFullscreen = false;
	this->isVSyncEnabled = true;
}

void Game::HandleInput(){
	this->UpdatePerformanceData();

	SDL_Event e;
	while(SDL_PollEvent(&e)){
		// keyboard stuff
		const u8* keyboardState 		= SDL_GetKeyboardState(NULL);
		this->isRunning 				= !keyboardState[SDL_SCANCODE_ESCAPE];
		this->cameraHasToMoveFront 		= keyboardState[SDL_SCANCODE_W];
		this->cameraHasToMoveBack 		= keyboardState[SDL_SCANCODE_S];
		this->cameraHasToMoveLeft 		= keyboardState[SDL_SCANCODE_A];
		this->cameraHasToMoveRight 		= keyboardState[SDL_SCANCODE_D];

		// mouse
		if(e.type == SDL_MOUSEBUTTONDOWN){
			if(e.button.button == SDL_BUTTON_RIGHT){
				SDL_GetMouseState(&this->lastMouseClickPosition.x, &this->lastMouseClickPosition.y);
				this->playerStartedMoving = true;
				this->playerIsMoving = true;
				this->lastMouseClickPositionNormalized.x =  (this->lastMouseClickPosition.x - this->windowWidth * 0.5f) / (this->windowWidth * 0.5f);
				this->lastMouseClickPositionNormalized.y = -(this->lastMouseClickPosition.y - this->windowHeight * 0.5f) / (this->windowHeight * 0.5f);
			}
		}
		if(e.type == SDL_MOUSEMOTION){
			if(this->firstMouseInput){
				this->mousePosition.x = e.motion.x;
				this->mousePosition.y = e.motion.y;
				this->firstMouseInput = false;
			}

			f32 xMousePosOffset = e.motion.xrel;
			f32 yMousePosOffset = e.motion.yrel;

			this->lastMousePosition.x = this->mousePosition.x;
			this->lastMousePosition.y = this->mousePosition.y;

			xMousePosOffset *= this->mouseSensitivity;
			yMousePosOffset *= this->mouseSensitivity;

			this->cameraYaw   += xMousePosOffset;
			this->cameraPitch -= yMousePosOffset;
			std::cout << this->cameraPitch << std::endl;

			if(this->cameraPitch >  89.0f) this->cameraPitch =  89.0f;
			if(this->cameraPitch < -89.0f) this->cameraPitch = -89.0f;

			glm::vec3 direction;
			direction.x = glm::cos(glm::radians(this->cameraYaw)) * glm::cos(glm::radians(this->cameraPitch));
			direction.y = glm::sin(glm::radians(this->cameraPitch));
			direction.z = glm::sin(glm::radians(this->cameraYaw)) * glm::cos(glm::radians(this->cameraPitch));
			this->cameraFront_f = glm::normalize(direction);
		}
		if(e.type == SDL_MOUSEWHEEL){
			fovDegrees -= (f32)e.wheel.y;
			if(fovDegrees <  1.0f) fovDegrees =  1.0f;
			if(fovDegrees > 45.0f) fovDegrees = 45.0f;
		}
		if(e.type == SDL_QUIT) this->isRunning = false;
	}
}

void Game::SimulateWorld(){
	if(this->cameraHasToMoveFront) this->cameraPosition_f += this->cameraFront_f * this->cameraMaximumSpeed * this->performanceData.deltaTimeInSeconds;
	if(this->cameraHasToMoveBack)  this->cameraPosition_f -= this->cameraFront_f * this->cameraMaximumSpeed * this->performanceData.deltaTimeInSeconds;
	if(this->cameraHasToMoveLeft)  this->cameraPosition_f -= glm::normalize(glm::cross(this->cameraFront_f, this->cameraUp_f)) * this->cameraMaximumSpeed * this->performanceData.deltaTimeInSeconds;
	if(this->cameraHasToMoveRight) this->cameraPosition_f += glm::normalize(glm::cross(this->cameraFront_f, this->cameraUp_f)) * this->cameraMaximumSpeed * this->performanceData.deltaTimeInSeconds;
}

void Game::RenderGraphics(){
	glClearColor(0.4f, 0.2f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Position/Scale/Rotate the objects in their place
	glm::mat4 modelMatrix = glm::mat4(1.0f);

	// Camera position (nothing for now)
	glm::mat4 viewMatrix = glm::lookAt(this->cameraPosition_f, this->cameraPosition_f + this->cameraFront_f, this->cameraUp_f);

	// Perspective projection matrix config
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(this->fovDegrees), this->windowAspectRatio, this->nearClipDistance, this->farClipDistance);

	this->shader->Use();
	this->shader->SetMat4("modelMatrix", modelMatrix);
	this->shader->SetMat4("viewMatrix", viewMatrix);
	this->shader->SetMat4("projectionMatrix", projectionMatrix);

	//drawing player
	this->vaoPlayer->Bind();
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	//drawing map
	this->vaoMap->Bind();
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	SDL_GL_SwapWindow(this->window);
}

void Game::UpdatePerformanceData(){
	performanceData.currentTimeInSeconds = SDL_GetTicks() / 1000.0f;
	performanceData.deltaTimeInSeconds = performanceData.currentTimeInSeconds - performanceData.lastTimeInSeconds;
	performanceData.lastTimeInSeconds = performanceData.currentTimeInSeconds;
}

void Game::Quit(){
	shader->Delete();
	vboPlayer->Delete();
	iboPlayer->Delete();
	vaoPlayer->Delete();
	SDL_GL_DeleteContext(this->context);
	SDL_DestroyWindow(this->window);
	SDL_Quit();
}

void Game::PrintMouseClickPosition(){
	std::cout << "Mouse Click Position: (" << this->lastMouseClickPosition.x << ", " << this->lastMouseClickPosition.y << ")" << std::endl;
}

void Game::PrintMousePosition(){
	std::cout << "Mouse Click Position: (" << this->mousePosition.x << ", " << this->mousePosition.y << ")" << std::endl;
}

void Game::PrintMouseClickPositionNormalized(){
	std::cout << "Mouse Click Position: (" << this->lastMouseClickPositionNormalized.x << ", " << this->lastMouseClickPositionNormalized.y << ")" << std::endl;
}

void Game::PrintPlayerPosition(){
	std::cout << "Player's Position: (" << this->playerPosition_f.x << ", " << this->playerPosition_f.y << ")" << std::endl;
}

void Game::PrintPlayerVelocity(){
	std::cout << "Player's Velocity: (" << this->playerVelocity_f[0] << ", " << this->playerVelocity_f[1] << ")" << std::endl;
}

void Game::PrintDeltaTime(){
	std::cout << "Delta Time: " << this->performanceData.deltaTimeInSeconds << std::endl;
}