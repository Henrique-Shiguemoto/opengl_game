#include "Game.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Game::Game(const char* name, i32 windowWidth, i32 windowHeight, b8 fullscreen){
	this->windowTitle = name;
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
	this->windowAspectRatio = (f32)this->windowWidth / (f32)this->windowHeight;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	if(SDL_Init(SDL_INIT_VIDEO)){
		std::cout << "SDL2 Couldn't Initialize!\n" << std::endl;
		this->isValid = false;
		return;
	}

	this->window = SDL_CreateWindow(this->windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_OPENGL | (fullscreen ? SDL_WINDOW_FULLSCREEN : 0));
	if(!this->window){
		std::cout << "Couldn't create the window!\n" << std::endl;
		this->isValid = false;
		return;
	}

	// SDL_ShowCursor(SDL_DISABLE);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_WarpMouseInWindow(this->window, this->windowWidth / 2, this->windowHeight / 2); // fix the mouse in the middle of the screen always
	stbi_set_flip_vertically_on_load(true);

	// icon
	i32 iconWidth, iconHeight, iconChannelCount;
	u8* iconPixels = stbi_load("assets/spaceship.png", &iconWidth, &iconHeight, &iconChannelCount, 0);
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

	// cursor texture
	glGenTextures(1, &(this->cursorTextureId));
	glBindTexture(GL_TEXTURE_2D, this->cursorTextureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	i32 cursorWidth, cursorHeight, cursorChannelCount;
	u8* cursorImageData = stbi_load("assets/cursor.png", &cursorWidth, &cursorHeight, &cursorChannelCount, 0);
	if (!cursorImageData){
		std::cout << "Failed to load texture\n";
		this->isValid = false;
		return;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, cursorWidth, cursorHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, cursorImageData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(cursorImageData);

	glEnable(GL_DEPTH_TEST);
	SDL_GL_SetSwapInterval(1); //enable vsync
	
	this->shader_noTextures 	= new Shader("shaders/shader_noTextures.vs", "shaders/shader_noTextures.fs");
	this->shader_withTextures 	= new Shader("shaders/shader_withTextures.vs", "shaders/shader_withTextures.fs");

	f32 mapVertices[] = {
		//position + colors
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
		//position + colors
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

	f32 cursorVertices[] = {
		// positions + uvs
		this->mousePositionUnit.x - 0.5f * this->mouseDimensionUnit.x, this->mousePositionUnit.y - 0.5f * this->mouseDimensionUnit.y, this->mousePositionUnit.z, 0.0f, 0.0f,
		this->mousePositionUnit.x + 0.5f * this->mouseDimensionUnit.x, this->mousePositionUnit.y - 0.5f * this->mouseDimensionUnit.y, this->mousePositionUnit.z, 1.0f, 0.0f,
		this->mousePositionUnit.x - 0.5f * this->mouseDimensionUnit.x, this->mousePositionUnit.y + 0.5f * this->mouseDimensionUnit.y, this->mousePositionUnit.z, 0.0f, 1.0f,
		this->mousePositionUnit.x + 0.5f * this->mouseDimensionUnit.x, this->mousePositionUnit.y + 0.5f * this->mouseDimensionUnit.y, this->mousePositionUnit.z, 1.0f, 1.0f
	};

	u32 cursorIndices[] = {
		0, 1, 2,
        1, 2, 3 
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

	this->vaoCursor = new VertexArray();
	this->vaoCursor->Bind();
	this->vboCursor = new VertexBuffer(cursorVertices, sizeof(cursorVertices));
	this->iboCursor = new IndexBuffer(cursorIndices, sizeof(cursorIndices));
	this->vaoCursor->DefineVBOLayout(vboCursor, 0, 3, 20, 0);
	this->vaoCursor->DefineVBOLayout(vboCursor, 1, 2, 20, 3);

	this->isValid = true;
	this->isRunning = true;
	this->isFullscreen = fullscreen;
	this->isVSyncEnabled = true;
}

void Game::HandleInput(){
	this->UpdatePerformanceData();

	SDL_Event e;
	while(SDL_PollEvent(&e)){
		// keyboard stuff
		const u8* keyboardState 				= SDL_GetKeyboardState(NULL);
		this->isRunning 						= !keyboardState[SDL_SCANCODE_ESCAPE];
		this->camera.hasToMoveUp 				= keyboardState[SDL_SCANCODE_W];
		this->camera.hasToMoveDown 				= keyboardState[SDL_SCANCODE_S];
		this->camera.hasToMoveLeft 				= keyboardState[SDL_SCANCODE_A];
		this->camera.hasToMoveRight 			= keyboardState[SDL_SCANCODE_D];

		// mouse
		if(e.type == SDL_MOUSEBUTTONDOWN){
			if(e.button.button == SDL_BUTTON_RIGHT){
				// PLAYER MOVEMENT
			}
		}
		if(e.type == SDL_MOUSEMOTION){
			// This is the only place in the code where the mouse position variable actually changes, 
			//			since if it doesn't move, it doesn't need to change.
			// Because of that, I'm synchronizing the Y axis between SDL2 and OpenGL, SDL2 uses a Y-is-Down coordinate system.
			this->mousePosition.x = e.motion.x;
			this->mousePosition.y = this->windowHeight - e.motion.y - 1;
			this->mousePositionUnit.x = (this->mousePosition.x - 0.5f * (this->windowWidth - 1)) / (0.5f * (this->windowWidth - 1));
			this->mousePositionUnit.y = (this->mousePosition.y - 0.5f * (this->windowHeight - 1)) / (0.5f * (this->windowHeight - 1));
			this->mousePositionUnit.z = 0.0f;
			
			// camera motion (only moves if the cursor close is close to the window border)
			if(this->mousePosition.x >= this->windowWidth - this->pixelsFromBorderToMove){
				this->camera.hasToMoveRight = true;
				this->camera.hasToMoveLeft 	= false;
			}
			if(this->mousePosition.x <= this->pixelsFromBorderToMove){
				this->camera.hasToMoveLeft 	= true;
				this->camera.hasToMoveRight = false;
			}
			if(this->mousePosition.y >= this->windowHeight - this->pixelsFromBorderToMove){
				this->camera.hasToMoveUp 	= true;
				this->camera.hasToMoveDown 	= false;
			}
			if(this->mousePosition.y <= this->pixelsFromBorderToMove){
				this->camera.hasToMoveDown 	= true;
				this->camera.hasToMoveUp 	= false;
			}
		}
		if(e.type == SDL_MOUSEWHEEL){
			this->camera.fovDegrees -= (f32)e.wheel.y;
			if(this->camera.fovDegrees <  1.0f) this->camera.fovDegrees =  1.0f;
			if(this->camera.fovDegrees > 45.0f) this->camera.fovDegrees = 45.0f;
		}
		if(e.type == SDL_QUIT) this->isRunning = false;
	}
}

void Game::SimulateWorld(){
	if(this->camera.hasToMoveUp)	this->camera.position_f += this->camera.up_f * this->camera.maximumSpeed * this->performanceData.deltaTimeInSeconds;
	if(this->camera.hasToMoveDown)	this->camera.position_f -= this->camera.up_f * this->camera.maximumSpeed * this->performanceData.deltaTimeInSeconds;
	if(this->camera.hasToMoveRight)	this->camera.position_f += this->camera.right_f * this->camera.maximumSpeed * this->performanceData.deltaTimeInSeconds;
	if(this->camera.hasToMoveLeft)	this->camera.position_f -= this->camera.right_f * this->camera.maximumSpeed * this->performanceData.deltaTimeInSeconds;
}

void Game::RenderGraphics(){
	glClearColor(0.4f, 0.2f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// calculating and setting matrix uniforms
	glm::mat4 modelMatrix 		= glm::mat4(1.0f);
	glm::mat4 viewMatrix 		= glm::lookAt(this->camera.position_f, this->camera.position_f + this->camera.front_f, this->camera.up_f);
	glm::mat4 projectionMatrix 	= glm::perspective(glm::radians(this->camera.fovDegrees), this->windowAspectRatio, this->camera.nearClipDistance, this->camera.farClipDistance);
	this->shader_noTextures->Use();
	this->shader_noTextures->SetMat4("modelMatrix", modelMatrix);
	this->shader_noTextures->SetMat4("viewMatrix", viewMatrix);
	this->shader_noTextures->SetMat4("projectionMatrix", projectionMatrix);

	this->vaoPlayer->Bind();
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	this->vaoMap->Bind();
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	this->shader_withTextures->Use();
	glm::mat4 cursorPos = glm::translate(glm::mat4(1.0f), this->mousePositionUnit);
	this->shader_withTextures->SetMat4("cursorOffsetMatrix", cursorPos);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->cursorTextureId);
	this->vaoCursor->Bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	SDL_GL_SwapWindow(this->window);
}

void Game::UpdatePerformanceData(){
	performanceData.currentTimeInSeconds = SDL_GetTicks() / 1000.0f;
	performanceData.deltaTimeInSeconds = performanceData.currentTimeInSeconds - performanceData.lastTimeInSeconds;
	performanceData.lastTimeInSeconds = performanceData.currentTimeInSeconds;
}

void Game::Quit(){
	shader_noTextures->Delete();
	shader_withTextures->Delete();
	vaoPlayer->Delete();
	vboPlayer->Delete();
	iboPlayer->Delete();
	vaoMap->Delete();
	vboMap->Delete();
	iboMap->Delete();
	vaoCursor->Delete();
	vboCursor->Delete();
	iboCursor->Delete();
	SDL_GL_DeleteContext(this->context);
	SDL_DestroyWindow(this->window);
	SDL_Quit();
}

void Game::PrintMouseClickPosition(){
	std::cout << "Mouse Click Position: (" << this->lastMouseClickPosition.x << ", " << this->lastMouseClickPosition.y << ")" << std::endl;
}

void Game::PrintMousePosition(){
	std::cout << "Mouse Position: (" << this->mousePosition.x << ", " << this->mousePosition.y << ")" << std::endl;
}

void Game::PrintMousePositionUnit(){
	std::cout << "Mouse Position Unit: (" << this->mousePositionUnit.x << ", " << this->mousePositionUnit.y << ", " << this->mousePositionUnit.z << ")" << std::endl;
}

void Game::PrintMouseClickPositionUnit(){
	std::cout << "Mouse Click Position: (" << this->lastMouseClickPositionUnit.x << ", " << this->lastMouseClickPositionUnit.y << ")" << std::endl;
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