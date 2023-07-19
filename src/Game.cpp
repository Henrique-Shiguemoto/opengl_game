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

	SDL_GL_SetSwapInterval(1); //enable vsync
	
	this->shader = new Shader(vertexShaderFilepath, fragmentShaderFilepath);

	f32 vertices[] = {
		(this->playerPosition_f.x - 0.5f*this->playerDimension_f.x), (this->playerPosition_f.y - 0.5f*this->playerDimension_f.y), 1.0f, 1.0f, 0.0f, 0.0f,
		(this->playerPosition_f.x + 0.5f*this->playerDimension_f.x), (this->playerPosition_f.y - 0.5f*this->playerDimension_f.y), 1.0f, 0.0f, 0.0f, 1.0f,
		(this->playerPosition_f.x + 0.5f*this->playerDimension_f.x), (this->playerPosition_f.y + 0.5f*this->playerDimension_f.y), 1.0f, 0.0f, 1.0f, 0.0f,
		(this->playerPosition_f.x - 0.5f*this->playerDimension_f.x), (this->playerPosition_f.y + 0.5f*this->playerDimension_f.y), 1.0f, 1.0f, 0.0f,	0.0f
	};

	u32 indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	this->vao = new VertexArray();
	this->vao->Bind();
	this->vbo = new VertexBuffer(vertices, sizeof(vertices));
	this->ibo = new IndexBuffer(indices, sizeof(indices));
	this->vao->DefineVBOLayout(vbo, 0, 3, 24, 0);
	this->vao->DefineVBOLayout(vbo, 1, 3, 24, 3);
	this->isValid = true;
	this->isRunning = true;
	this->isFullscreen = false;
	this->isVSyncEnabled = true;
}

void Game::HandleInput(){
	this->UpdateFrametimeInWindowTitle();

	SDL_Event e;
	while(SDL_PollEvent(&e)){
		const u8* keyboardState = SDL_GetKeyboardState(NULL);

		if(e.type == SDL_QUIT || keyboardState[SDL_SCANCODE_ESCAPE]){
			this->isRunning = false;
		}

		if(e.type == SDL_MOUSEBUTTONDOWN){
			if(e.button.button == SDL_BUTTON_RIGHT){
				SDL_GetMouseState(&this->lastMouseClickPosition[0], &this->lastMouseClickPosition[1]);
				this->playerStartedMoving = true;
				this->playerIsMoving = true;
				this->lastMouseClickPositionNormalized[0] =  (this->lastMouseClickPosition[0] - this->windowWidth * 0.5f) / (this->windowWidth * 0.5f);
				this->lastMouseClickPositionNormalized[1] = -(this->lastMouseClickPosition[1] - this->windowHeight * 0.5f) / (this->windowHeight * 0.5f);
			}
		}
	}
}

void Game::SimulateWorld(){
	if(this->playerStartedMoving){
		//calculate velocity vector only once
		this->playerVelocity_f[0] = this->playerPosition_f[0] - this->lastMouseClickPositionNormalized[0];
		this->playerVelocity_f[1] = this->playerPosition_f[1] - this->lastMouseClickPositionNormalized[1];
		this->playerVelocity_f = glm::normalize(this->playerVelocity_f);
		this->playerStartedMoving = false;
	}
	if(this->playerIsMoving){
		f32 mousePlayerDiffX_abs = glm::abs(this->playerPosition_f.x - this->lastMouseClickPositionNormalized[0]);
		f32 mousePlayerDiffY_abs = glm::abs(this->playerPosition_f.y - this->lastMouseClickPositionNormalized[1]);
		if(mousePlayerDiffX_abs < distanceForPlayerToStopMoving && mousePlayerDiffY_abs < distanceForPlayerToStopMoving){
			this->playerIsMoving = false;
			this->playerVelocity_f = glm::vec3(0.0f);
		}
	}
	
	// updating player's position
	this->playerPosition_f[0] -= this->playerVelocity_f[0] * this->playerMaximumSpeed;
	this->playerPosition_f[1] -= this->playerVelocity_f[1] * this->playerMaximumSpeed;
}

void Game::RenderGraphics(){
	glClearColor(0.4f, 0.2f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(this->playerPosition_f[0], this->playerPosition_f[1], 0.0f));

	f32 scaleMuch = glm::abs(glm::sin(0.001f*(f32)SDL_GetTicks())) + 0.25f;
	transform = glm::scale(transform, glm::vec3(scaleMuch, scaleMuch, 1.0f));
	transform = glm::rotate(transform, 0.001f * (f32)SDL_GetTicks(), glm::vec3(0.0f, 0.0f, 1.0f));


	this->shader->Use();
	this->vao->Bind();

	this->shader->SetMat4("transform", transform);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	SDL_GL_SwapWindow(this->window);
}

void Game::UpdateFrametimeInWindowTitle(){
	performanceData.currentTime = SDL_GetTicks();
	if(performanceData.framesSinceFrametimeUpdate >= performanceData.framesPerUpdate){
		std::string newWindowTitle = this->name + " - Frametime (ms): " + std::to_string((f32)performanceData.frameTimeSum / (f32)performanceData.framesPerUpdate);
		SDL_SetWindowTitle(this->window, newWindowTitle.c_str());
		performanceData.framesSinceFrametimeUpdate = 0;
		performanceData.frameTimeSum = 0;
	}
	performanceData.frameTimeSum += (performanceData.currentTime - performanceData.lastTime);
	performanceData.lastTime = performanceData.currentTime;
	performanceData.framesSinceFrametimeUpdate++;
}

void Game::Quit(){
	shader->Delete();
	vbo->Delete();
	ibo->Delete();
	vao->Delete();
	SDL_GL_DeleteContext(this->context);
	SDL_DestroyWindow(this->window);
	SDL_Quit();
}

void Game::PrintMouseClickPosition(){
	std::cout << "Mouse Click Position: (" << this->lastMouseClickPosition[0] << ", " << this->lastMouseClickPosition[1] << ")" << std::endl;
}

void Game::PrintMouseClickPositionNormalized(){
	std::cout << "Mouse Click Position: (" << this->lastMouseClickPositionNormalized[0] << ", " << this->lastMouseClickPositionNormalized[1] << ")" << std::endl;
}

void Game::PrintPlayerPosition(){
	std::cout << "Player's Position: (" << this->playerPosition_f[0] << ", " << this->playerPosition_f[1] << ")" << std::endl;
}

void Game::PrintPlayerVelocity(){
	std::cout << "Player's Velocity: (" << this->playerVelocity_f[0] << ", " << this->playerVelocity_f[1] << ")" << std::endl;
}