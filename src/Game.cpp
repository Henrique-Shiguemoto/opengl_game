#include "Game.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Game::Game(const char* name, i32 windowWidth, i32 windowHeight, b8 fullscreen){
	this->windowTitle = name;
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
	this->windowAspectRatio = (f32)this->windowWidth / (f32)this->windowHeight;

	if(SDL_Init(SDL_INIT_VIDEO)){
		std::cout << "SDL2 Couldn't Initialize!\n" << std::endl;
		this->isValid = false;
		return;
	}

	this->window = SDL_CreateWindow(this->windowTitle.c_str(), 100, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_OPENGL | (fullscreen ? SDL_WINDOW_FULLSCREEN : 0));
	if(!this->window){
		std::cout << "Couldn't create the window!\n" << std::endl;
		this->isValid = false;
		return;
	}

	// I'm currently using RenderDoc for graphics debugging and it is important that the code is in this order to avoid errors in RenderDoc, 
	// 		SDL_Init => SDL_CreateWindow => SDL_GL_SetAttribute => SDL_GL_CreateContext.
	// If it's not in this order, RenderDoc will show an error (at least on my machine) along the lines of "Context not created by CreateContextAttribs. Captures Disabled."
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

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

	SDL_SetRelativeMouseMode(SDL_TRUE); // this hides the mouse cursor and confines it to the window
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

	this->cursorTexture = new Texture("assets/cursor.png", GL_NEAREST, GL_RGBA, GL_TEXTURE0, true);
	this->playerTexture = new Texture("assets/container.png", GL_LINEAR, GL_RGBA, GL_TEXTURE0, true);
	this->mapTexture 	= new Texture("assets/map_moba.png", GL_LINEAR, GL_RGBA, GL_TEXTURE0, true);

	glEnable(GL_DEPTH_TEST);
	SDL_GL_SetSwapInterval(1); //enable vsync
	
	this->shader_withTextures = new Shader("shaders/shader_withTextures.vs", "shaders/shader_withTextures.fs");
	this->shader_UI = new Shader("shaders/shader_UI.vs", "shaders/shader_UI.fs");
	this->shader_light = new Shader("shaders/shader_light.vs", "shaders/shader_light.fs");

	// MAP CORNERS (Left, Right, Botton, Top, Near, Far)
	#define LBN_MAP this->mapPosition_f.x - 0.5f*this->mapDimension_f.x, this->mapPosition_f.y - 0.5f*this->mapDimension_f.y, this->mapPosition_f.z + 0.5f*this->mapDimension_f.z
	#define LTN_MAP this->mapPosition_f.x - 0.5f*this->mapDimension_f.x, this->mapPosition_f.y + 0.5f*this->mapDimension_f.y, this->mapPosition_f.z + 0.5f*this->mapDimension_f.z
	#define LTF_MAP this->mapPosition_f.x - 0.5f*this->mapDimension_f.x, this->mapPosition_f.y + 0.5f*this->mapDimension_f.y, this->mapPosition_f.z - 0.5f*this->mapDimension_f.z
	#define LBF_MAP this->mapPosition_f.x - 0.5f*this->mapDimension_f.x, this->mapPosition_f.y - 0.5f*this->mapDimension_f.y, this->mapPosition_f.z - 0.5f*this->mapDimension_f.z
	#define RBN_MAP this->mapPosition_f.x + 0.5f*this->mapDimension_f.x, this->mapPosition_f.y - 0.5f*this->mapDimension_f.y, this->mapPosition_f.z + 0.5f*this->mapDimension_f.z
	#define RTN_MAP this->mapPosition_f.x + 0.5f*this->mapDimension_f.x, this->mapPosition_f.y + 0.5f*this->mapDimension_f.y, this->mapPosition_f.z + 0.5f*this->mapDimension_f.z
	#define RTF_MAP this->mapPosition_f.x + 0.5f*this->mapDimension_f.x, this->mapPosition_f.y + 0.5f*this->mapDimension_f.y, this->mapPosition_f.z - 0.5f*this->mapDimension_f.z
	#define RBF_MAP this->mapPosition_f.x + 0.5f*this->mapDimension_f.x, this->mapPosition_f.y - 0.5f*this->mapDimension_f.y, this->mapPosition_f.z - 0.5f*this->mapDimension_f.z

	#define L_NORMAL -1.0f,  0.0f,  0.0f
	#define R_NORMAL  1.0f,  0.0f,  0.0f
	#define N_NORMAL  0.0f,  0.0f,  1.0f
	#define F_NORMAL  0.0f,  0.0f, -1.0f
	#define T_NORMAL  0.0f,  1.0f,  0.0f
	#define B_NORMAL  0.0f, -1.0f,  0.0f

	f32 mapVertices[] = {
		//position + texture coordinates + normals
		LBN_MAP, 0.0f, 0.0f, L_NORMAL,
		LTN_MAP, 0.0f, 0.0f, L_NORMAL,
		LTF_MAP, 0.0f, 0.0f, L_NORMAL,
		LTF_MAP, 0.0f, 0.0f, L_NORMAL,
		LBN_MAP, 0.0f, 0.0f, L_NORMAL,
		LBF_MAP, 0.0f, 0.0f, L_NORMAL,
		LTF_MAP, 0.0f, 0.0f, F_NORMAL,
		LBF_MAP, 0.0f, 0.0f, F_NORMAL,
		RBF_MAP, 0.0f, 0.0f, F_NORMAL,
		LTF_MAP, 0.0f, 0.0f, F_NORMAL,
		RBF_MAP, 0.0f, 0.0f, F_NORMAL,
		RTF_MAP, 0.0f, 0.0f, F_NORMAL,
		RTF_MAP, 0.0f, 0.0f, R_NORMAL,
		RBF_MAP, 0.0f, 0.0f, R_NORMAL,
		RTN_MAP, 0.0f, 0.0f, R_NORMAL,
		RTN_MAP, 0.0f, 0.0f, R_NORMAL,
		RBF_MAP, 0.0f, 0.0f, R_NORMAL,
		RBN_MAP, 0.0f, 0.0f, R_NORMAL,
		RBN_MAP, 0.0f, 0.0f, N_NORMAL,
		RTN_MAP, 0.0f, 0.0f, N_NORMAL,
		LTN_MAP, 0.0f, 0.0f, N_NORMAL,
		LTN_MAP, 0.0f, 0.0f, N_NORMAL,
		RBN_MAP, 0.0f, 0.0f, N_NORMAL,
		LBN_MAP, 0.0f, 0.0f, N_NORMAL,
		RBN_MAP, 0.0f, 0.0f, B_NORMAL,
		LBN_MAP, 0.0f, 0.0f, B_NORMAL,
		LBF_MAP, 0.0f, 0.0f, B_NORMAL,
		RBN_MAP, 0.0f, 0.0f, B_NORMAL,
		LBF_MAP, 0.0f, 0.0f, B_NORMAL,
		RBF_MAP, 0.0f, 0.0f, B_NORMAL,
		LTN_MAP, 0.0f, 0.0f, T_NORMAL,
		LTF_MAP, 0.0f, 1.0f, T_NORMAL,
		RTN_MAP, 1.0f, 0.0f, T_NORMAL,
		LTF_MAP, 0.0f, 1.0f, T_NORMAL,
		RTN_MAP, 1.0f, 0.0f, T_NORMAL,
		RTF_MAP, 1.0f, 1.0f, T_NORMAL
	};

	u32 mapIndices[] = {
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9, 10, 11,
		12, 13, 14,
		15, 16, 17,
		18, 19, 20,
		21, 22, 23,
		24, 25, 26,
		27, 28, 29,
		30, 31, 32, 
		33, 34, 35
	};

	#define LBN_PLAYER -0.5f*this->playerDimension_f.x, -0.5f*this->playerDimension_f.y, +0.5f*this->playerDimension_f.z
	#define LTN_PLAYER -0.5f*this->playerDimension_f.x, +0.5f*this->playerDimension_f.y, +0.5f*this->playerDimension_f.z
	#define LTF_PLAYER -0.5f*this->playerDimension_f.x, +0.5f*this->playerDimension_f.y, -0.5f*this->playerDimension_f.z
	#define LBF_PLAYER -0.5f*this->playerDimension_f.x, -0.5f*this->playerDimension_f.y, -0.5f*this->playerDimension_f.z
	#define RBN_PLAYER +0.5f*this->playerDimension_f.x, -0.5f*this->playerDimension_f.y, +0.5f*this->playerDimension_f.z
	#define RTN_PLAYER +0.5f*this->playerDimension_f.x, +0.5f*this->playerDimension_f.y, +0.5f*this->playerDimension_f.z
	#define RTF_PLAYER +0.5f*this->playerDimension_f.x, +0.5f*this->playerDimension_f.y, -0.5f*this->playerDimension_f.z
	#define RBF_PLAYER +0.5f*this->playerDimension_f.x, -0.5f*this->playerDimension_f.y, -0.5f*this->playerDimension_f.z

	f32 playerVertices[] = {
		//position + uv + normals
		LBN_PLAYER, 0.0f, 0.0f, L_NORMAL,
		LTN_PLAYER, 0.0f, 1.0f, L_NORMAL,
		LTF_PLAYER, 1.0f, 1.0f, L_NORMAL,
		LTF_PLAYER, 1.0f, 1.0f, L_NORMAL,
		LBN_PLAYER, 0.0f, 0.0f, L_NORMAL,
		LBF_PLAYER, 1.0f, 0.0f, L_NORMAL,
		LTF_PLAYER, 0.0f, 1.0f, F_NORMAL,
		LBF_PLAYER, 0.0f, 0.0f, F_NORMAL,
		RBF_PLAYER, 1.0f, 0.0f, F_NORMAL,
		LTF_PLAYER, 0.0f, 1.0f, F_NORMAL,
		RBF_PLAYER, 1.0f, 0.0f, F_NORMAL,
		RTF_PLAYER, 1.0f, 1.0f, F_NORMAL,
		RTF_PLAYER, 1.0f, 1.0f, R_NORMAL,
		RBF_PLAYER, 1.0f, 0.0f, R_NORMAL,
		RTN_PLAYER, 0.0f, 1.0f, R_NORMAL,
		RTN_PLAYER, 0.0f, 1.0f, R_NORMAL,
		RBF_PLAYER, 1.0f, 0.0f, R_NORMAL,
		RBN_PLAYER, 0.0f, 0.0f, R_NORMAL,
		RBN_PLAYER, 1.0f, 0.0f, N_NORMAL,
		RTN_PLAYER, 1.0f, 1.0f, N_NORMAL,
		LTN_PLAYER, 0.0f, 1.0f, N_NORMAL,
		LTN_PLAYER, 0.0f, 1.0f, N_NORMAL,
		RBN_PLAYER, 1.0f, 0.0f, N_NORMAL,
		LBN_PLAYER, 0.0f, 0.0f, N_NORMAL,
		RBN_PLAYER, 1.0f, 0.0f, B_NORMAL,
		LBN_PLAYER, 0.0f, 0.0f, B_NORMAL,
		LBF_PLAYER, 0.0f, 1.0f, B_NORMAL,
		RBN_PLAYER, 1.0f, 0.0f, B_NORMAL,
		LBF_PLAYER, 0.0f, 1.0f, B_NORMAL,
		RBF_PLAYER, 1.0f, 1.0f, B_NORMAL,
		LTN_PLAYER, 0.0f, 0.0f, T_NORMAL,
		LTF_PLAYER, 0.0f, 1.0f, T_NORMAL,
		RTN_PLAYER, 1.0f, 0.0f, T_NORMAL,
		LTF_PLAYER, 0.0f, 1.0f, T_NORMAL,
		RTN_PLAYER, 1.0f, 0.0f, T_NORMAL,
		RTF_PLAYER, 1.0f, 1.0f, T_NORMAL
	};

	u32 playerIndices[] = {
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9, 10, 11,
		12, 13, 14,
		15, 16, 17,
		18, 19, 20,
		21, 22, 23,
		24, 25, 26,
		27, 28, 29,
		30, 31, 32, 
		33, 34, 35
	};

	f32 cursorVertices[] = {
		// positions + uvs
		-0.5f * this->mouseDimensionUnit.x, -0.5f * this->mouseDimensionUnit.y, this->mousePositionUnit.z, 0.0f, 0.0f,
		+0.5f * this->mouseDimensionUnit.x, -0.5f * this->mouseDimensionUnit.y, this->mousePositionUnit.z, 1.0f, 0.0f,
		-0.5f * this->mouseDimensionUnit.x, +0.5f * this->mouseDimensionUnit.y, this->mousePositionUnit.z, 0.0f, 1.0f,
		+0.5f * this->mouseDimensionUnit.x, +0.5f * this->mouseDimensionUnit.y, this->mousePositionUnit.z, 1.0f, 1.0f
	};

	u32 cursorIndices[] = {
		0, 1, 2,
        1, 2, 3 
	};

	#define LBN_LIGHT this->light.lightPos.x - 0.5f*this->light.dimension_f.x, this->light.lightPos.y - 0.5f*this->light.dimension_f.y, this->light.lightPos.z + 0.5f*this->light.dimension_f.z
	#define LTN_LIGHT this->light.lightPos.x - 0.5f*this->light.dimension_f.x, this->light.lightPos.y + 0.5f*this->light.dimension_f.y, this->light.lightPos.z + 0.5f*this->light.dimension_f.z
	#define LTF_LIGHT this->light.lightPos.x - 0.5f*this->light.dimension_f.x, this->light.lightPos.y + 0.5f*this->light.dimension_f.y, this->light.lightPos.z - 0.5f*this->light.dimension_f.z
	#define LBF_LIGHT this->light.lightPos.x - 0.5f*this->light.dimension_f.x, this->light.lightPos.y - 0.5f*this->light.dimension_f.y, this->light.lightPos.z - 0.5f*this->light.dimension_f.z
	#define RBN_LIGHT this->light.lightPos.x + 0.5f*this->light.dimension_f.x, this->light.lightPos.y - 0.5f*this->light.dimension_f.y, this->light.lightPos.z + 0.5f*this->light.dimension_f.z
	#define RTN_LIGHT this->light.lightPos.x + 0.5f*this->light.dimension_f.x, this->light.lightPos.y + 0.5f*this->light.dimension_f.y, this->light.lightPos.z + 0.5f*this->light.dimension_f.z
	#define RTF_LIGHT this->light.lightPos.x + 0.5f*this->light.dimension_f.x, this->light.lightPos.y + 0.5f*this->light.dimension_f.y, this->light.lightPos.z - 0.5f*this->light.dimension_f.z
	#define RBF_LIGHT this->light.lightPos.x + 0.5f*this->light.dimension_f.x, this->light.lightPos.y - 0.5f*this->light.dimension_f.y, this->light.lightPos.z - 0.5f*this->light.dimension_f.z

	f32 lightVertices[] = {
		//position + color
		LBN_LIGHT, 1.0f, 1.0f, 1.0f,
		LTN_LIGHT, 1.0f, 1.0f, 1.0f,
		LTF_LIGHT, 1.0f, 1.0f, 1.0f,
		LTF_LIGHT, 1.0f, 1.0f, 1.0f,
		LBN_LIGHT, 1.0f, 1.0f, 1.0f,
		LBF_LIGHT, 1.0f, 1.0f, 1.0f,
		LTF_LIGHT, 1.0f, 1.0f, 1.0f,
		LBF_LIGHT, 1.0f, 1.0f, 1.0f,
		RBF_LIGHT, 1.0f, 1.0f, 1.0f,
		LTF_LIGHT, 1.0f, 1.0f, 1.0f,
		RBF_LIGHT, 1.0f, 1.0f, 1.0f,
		RTF_LIGHT, 1.0f, 1.0f, 1.0f,
		RTF_LIGHT, 1.0f, 1.0f, 1.0f,
		RBF_LIGHT, 1.0f, 1.0f, 1.0f,
		RTN_LIGHT, 1.0f, 1.0f, 1.0f,
		RTN_LIGHT, 1.0f, 1.0f, 1.0f,
		RBF_LIGHT, 1.0f, 1.0f, 1.0f,
		RBN_LIGHT, 1.0f, 1.0f, 1.0f,
		RBN_LIGHT, 1.0f, 1.0f, 1.0f,
		RTN_LIGHT, 1.0f, 1.0f, 1.0f,
		LTN_LIGHT, 1.0f, 1.0f, 1.0f,
		LTN_LIGHT, 1.0f, 1.0f, 1.0f,
		RBN_LIGHT, 1.0f, 1.0f, 1.0f,
		LBN_LIGHT, 1.0f, 1.0f, 1.0f,
		RBN_LIGHT, 1.0f, 1.0f, 1.0f,
		LBN_LIGHT, 1.0f, 1.0f, 1.0f,
		LBF_LIGHT, 1.0f, 1.0f, 1.0f,
		RBN_LIGHT, 1.0f, 1.0f, 1.0f,
		LBF_LIGHT, 1.0f, 1.0f, 1.0f,
		RBF_LIGHT, 1.0f, 1.0f, 1.0f,
		LTN_LIGHT, 1.0f, 1.0f, 1.0f,
		LTF_LIGHT, 1.0f, 1.0f, 1.0f,
		RTN_LIGHT, 1.0f, 1.0f, 1.0f,
		LTF_LIGHT, 1.0f, 1.0f, 1.0f,
		RTN_LIGHT, 1.0f, 1.0f, 1.0f,
		RTF_LIGHT, 1.0f, 1.0f, 1.0f
	};

	u32 lightIndices[] = {
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9, 10, 11,
		12, 13, 14,
		15, 16, 17,
		18, 19, 20,
		21, 22, 23,
		24, 25, 26,
		27, 28, 29,
		30, 31, 32, 
		33, 34, 35
	};

	this->vaoPlayer = new VertexArray();
	this->vaoPlayer->Bind();
	this->vboPlayer = new VertexBuffer(playerVertices, sizeof(playerVertices));
	this->iboPlayer = new IndexBuffer(playerIndices, sizeof(playerIndices));
	this->vaoPlayer->DefineVBOLayout(vboPlayer, 0, 3, 32, 0);
	this->vaoPlayer->DefineVBOLayout(vboPlayer, 1, 2, 32, 3);
	this->vaoPlayer->DefineVBOLayout(vboPlayer, 2, 3, 32, 5);

	this->vaoMap = new VertexArray();
	this->vaoMap->Bind();
	this->vboMap = new VertexBuffer(mapVertices, sizeof(mapVertices));
	this->iboMap = new IndexBuffer(mapIndices, sizeof(mapIndices));
	this->vaoMap->DefineVBOLayout(vboMap, 0, 3, 32, 0);
	this->vaoMap->DefineVBOLayout(vboMap, 1, 2, 32, 3);
	this->vaoMap->DefineVBOLayout(vboMap, 2, 3, 32, 5);

	this->vaoCursor = new VertexArray();
	this->vaoCursor->Bind();
	this->vboCursor = new VertexBuffer(cursorVertices, sizeof(cursorVertices));
	this->iboCursor = new IndexBuffer(cursorIndices, sizeof(cursorIndices));
	this->vaoCursor->DefineVBOLayout(vboCursor, 0, 3, 20, 0);
	this->vaoCursor->DefineVBOLayout(vboCursor, 1, 2, 20, 3);

	this->vaoLight = new VertexArray();
	this->vaoLight->Bind();
	this->vboLight = new VertexBuffer(lightVertices, sizeof(lightVertices));
	this->iboLight = new IndexBuffer(lightIndices, sizeof(lightIndices));
	this->vaoPlayer->DefineVBOLayout(vboLight, 0, 3, 24, 0);
	this->vaoPlayer->DefineVBOLayout(vboLight, 1, 3, 24, 3);

	this->light.lightPos += this->playerPosition_f;

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
		const u8* keyboardState = SDL_GetKeyboardState(NULL);
		this->isRunning = !keyboardState[SDL_SCANCODE_ESCAPE];

		static b8 cameraControlWasToggledLastFrame = false;
		b8 cameraControlWasToggled = keyboardState[SDL_SCANCODE_C];
		if(cameraControlWasToggled && !cameraControlWasToggledLastFrame){
			this->camera.toggleFPS = !this->camera.toggleFPS;
		}

		//Inthis engine, some controls are not allowed if you're not in an FPS camera (you can't control light movement for example)
		if(!this->camera.toggleFPS){
			this->light.lightHasToMoveForward 	= keyboardState[SDL_SCANCODE_W];
			this->light.lightHasToMoveBack 		= keyboardState[SDL_SCANCODE_S];
			this->light.lightHasToMoveLeft 		= keyboardState[SDL_SCANCODE_A];
			this->light.lightHasToMoveRight 	= keyboardState[SDL_SCANCODE_D];
			this->light.lightHasToMoveUp 		= keyboardState[SDL_SCANCODE_UP];
			this->light.lightHasToMoveDown 		= keyboardState[SDL_SCANCODE_DOWN];
		}else{
			this->camera.hasToMoveFront		= keyboardState[SDL_SCANCODE_W];
			this->camera.hasToMoveBack		= keyboardState[SDL_SCANCODE_S];
			this->camera.hasToMoveLeft		= keyboardState[SDL_SCANCODE_A];
			this->camera.hasToMoveRight		= keyboardState[SDL_SCANCODE_D];
		}

		// mouse
		if(e.type == SDL_MOUSEBUTTONDOWN){
			if(e.button.button == SDL_BUTTON_RIGHT){
				this->playerHasClicked = true;
			}
		}
		if(e.type == SDL_MOUSEMOTION){
			if(!this->camera.toggleFPS){
				// This is the only place in the code where the mouse position variable actually changes, 
				//			since if it doesn't move, it doesn't need to change.
				// Because of that, I'm synchronizing the Y axis between SDL2 and OpenGL, SDL2 uses a Y-is-Down coordinate system.
				this->mousePosition.x = e.motion.x;
				this->mousePosition.y = this->windowHeight - e.motion.y - 1;
				this->mousePositionUnit.x = (this->mousePosition.x - 0.5f * (this->windowWidth - 1)) / (0.5f * (this->windowWidth - 1));
				this->mousePositionUnit.y = (this->mousePosition.y - 0.5f * (this->windowHeight - 1)) / (0.5f * (this->windowHeight - 1));
				this->mousePositionUnit.z = 0.0f;

				if(this->mousePosition.x >= this->windowWidth - this->pixelsFromBorderToMove){
					this->camera.hasToMoveRight = true;
					this->camera.hasToMoveLeft 	= false;
				}else if(this->mousePosition.x <= this->pixelsFromBorderToMove){
					this->camera.hasToMoveLeft 	= true;
					this->camera.hasToMoveRight = false;
				}else{
					this->camera.hasToMoveLeft 	= false;
					this->camera.hasToMoveRight = false;
				}

				if(this->mousePosition.y >= this->windowHeight - this->pixelsFromBorderToMove){
					this->camera.hasToMoveUp 	= true;
					this->camera.hasToMoveDown 	= false;
				}else if(this->mousePosition.y <= this->pixelsFromBorderToMove){
					this->camera.hasToMoveDown 	= true;
					this->camera.hasToMoveUp 	= false;
				}else{
					this->camera.hasToMoveDown 	= false;
					this->camera.hasToMoveUp 	= false;
				}
			}else{
				if(this->firstMouseInput){
					this->mousePosition.x = e.motion.x;
					this->mousePosition.y = e.motion.y;
					this->firstMouseInput = false;
				}
				f32 xoffset = e.motion.xrel;
				f32 yoffset = e.motion.yrel;
    			this->lastMousePosition.x = this->mousePosition.x;
				this->lastMousePosition.y = this->mousePosition.y;
				this->camera.ProcessMouseMovement(xoffset, yoffset);
			}
		}
		if(e.type == SDL_MOUSEWHEEL){
			this->camera.ProcessMouseScroll((f32)e.wheel.y);
		}
		if(e.type == SDL_QUIT) this->isRunning = false;

		//update input state
		cameraControlWasToggledLastFrame = cameraControlWasToggled;
	}
}

void Game::SimulateWorld(){
	if(!this->camera.toggleFPS){
		float lightSpeed = 500.0f;
		if(this->camera.hasToMoveUp)	this->camera.position_f += this->camera.up_f * this->camera.maximumSpeed * this->performanceData.deltaTimeInSeconds;
		if(this->camera.hasToMoveDown)	this->camera.position_f -= this->camera.up_f * this->camera.maximumSpeed * this->performanceData.deltaTimeInSeconds;
		if(this->camera.hasToMoveRight)	this->camera.position_f += this->camera.right_f * this->camera.maximumSpeed * this->performanceData.deltaTimeInSeconds;
		if(this->camera.hasToMoveLeft)	this->camera.position_f -= this->camera.right_f * this->camera.maximumSpeed * this->performanceData.deltaTimeInSeconds;

		if(this->light.lightHasToMoveForward) 	this->light.lightPos.z -= 0.01f * lightSpeed * this->performanceData.deltaTimeInSeconds;
		if(this->light.lightHasToMoveBack) 		this->light.lightPos.z += 0.01f * lightSpeed * this->performanceData.deltaTimeInSeconds;
		if(this->light.lightHasToMoveLeft) 		this->light.lightPos.x -= 0.01f * lightSpeed * this->performanceData.deltaTimeInSeconds;
		if(this->light.lightHasToMoveRight) 	this->light.lightPos.x += 0.01f * lightSpeed * this->performanceData.deltaTimeInSeconds;
		if(this->light.lightHasToMoveUp) 		this->light.lightPos.y += 0.01f * lightSpeed * this->performanceData.deltaTimeInSeconds;
		if(this->light.lightHasToMoveDown) 		this->light.lightPos.y -= 0.01f * lightSpeed * this->performanceData.deltaTimeInSeconds;
	}else{
		//camera is an FPS camera
		if(this->camera.hasToMoveFront) this->camera.position_f += this->camera.front_f * this->camera.maximumSpeed * this->performanceData.deltaTimeInSeconds;
		if(this->camera.hasToMoveBack)  this->camera.position_f -= this->camera.front_f * this->camera.maximumSpeed * this->performanceData.deltaTimeInSeconds;
		if(this->camera.hasToMoveLeft)  this->camera.position_f -= glm::normalize(glm::cross(this->camera.front_f, this->camera.up_f)) * this->camera.maximumSpeed * this->performanceData.deltaTimeInSeconds;
		if(this->camera.hasToMoveRight) this->camera.position_f += glm::normalize(glm::cross(this->camera.front_f, this->camera.up_f)) * this->camera.maximumSpeed * this->performanceData.deltaTimeInSeconds;
	}

	if(this->playerHasClicked){
		// Mouse raycast
		// Normalized Device Coordinates
		this->currentMouseRay.x = this->mousePositionUnit.x;
		this->currentMouseRay.y = this->mousePositionUnit.y;

		// Homogeneous Clip Space
		glm::vec4 rayClip = glm::vec4(this->currentMouseRay.x, this->currentMouseRay.y, -1.0f, 1.0f);

		// Camera Space Coordinates
		glm::vec4 rayEye = glm::inverse(this->projectionMatrix) * rayClip;
		rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

		// World Coordinates
		glm::vec4 rayWorld4D = glm::inverse(this->viewMatrix) * rayEye;
		glm::vec3 rayWorld = glm::vec3(rayWorld4D.x, rayWorld4D.y, rayWorld4D.z);
		rayWorld = glm::normalize(rayWorld);

		glm::vec3 rayMapTopSurfaceCollision = glm::vec3(this->camera.position_f.x + rayWorld.x * ((this->randomPointFromMapTopSurface_f.y - this->camera.position_f.y) / rayWorld.y),
														this->randomPointFromMapTopSurface_f.y,
														this->camera.position_f.z + rayWorld.z * ((this->randomPointFromMapTopSurface_f.y - this->camera.position_f.y) / rayWorld.y));

		rayMapTopSurfaceCollision = glm::clamp(rayMapTopSurfaceCollision, glm::vec3(this->mapPosition_f.x - 0.5f*this->mapDimension_f.x, 0.5f, this->mapPosition_f.z - 0.5f*this->mapDimension_f.z),
																		  glm::vec3(this->mapPosition_f.x + 0.5f*this->mapDimension_f.x, 0.5f, this->mapPosition_f.z + 0.5f*this->mapDimension_f.z));

		this->locationPlayerHasToGo = glm::vec3(rayMapTopSurfaceCollision.x, rayMapTopSurfaceCollision.y + 0.5f*this->playerDimension_f.y, rayMapTopSurfaceCollision.z);
		this->playerVelocity_f = glm::normalize(this->locationPlayerHasToGo - this->playerPosition_f);
		this->playerIsMoving = true;
		this->playerHasClicked = false;
	}

	if(this->playerIsMoving){
		f32 distanceBetweenDestinationAndPlayer = glm::distance(this->locationPlayerHasToGo, this->playerPosition_f);
		if(distanceBetweenDestinationAndPlayer <= this->distanceForPlayerToStopMoving){
			this->playerVelocity_f = glm::vec3(0.0f);
			this->playerIsMoving = false;
		}
		this->playerPosition_f += this->playerVelocity_f * this->playerMaximumSpeed * this->performanceData.deltaTimeInSeconds;
	}
}

void Game::RenderGraphics(){
	glClearColor(0.561f, 0.224f, 0.639f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Drawing the objects
	this->shader_withTextures->Use();

	//lighting stuff
	this->shader_withTextures->SetVec3("lightPos", this->light.lightPos);
	this->light.lightColor.r = 1.0f;
	this->light.lightColor.g = 1.0f;
	this->light.lightColor.b = 1.0f;
	this->shader_withTextures->SetVec3("light.ambient",  this->light.ambientColor);
	this->shader_withTextures->SetVec3("light.diffuse",  this->light.diffuseColor);
	this->shader_withTextures->SetVec3("light.specular", this->light.specularColor);

	// material stuff
	this->material.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	this->material.specular = glm::vec3(1.0f, 1.0f, 1.0f);
	this->material.shininess = 0.4 * 128;
	this->shader_withTextures->SetVec3("cameraPos", this->camera.position_f);
	this->shader_withTextures->SetVec3("material.ambient", this->material.ambient);
	this->shader_withTextures->SetInt("material.diffuse", this->material.diffuse);
	this->shader_withTextures->SetVec3("material.specular", this->material.specular);
	this->shader_withTextures->SetFloat("material.shininess", this->material.shininess);

	// Draw map
	// this->mapTexture->Activate();
	// this->mapTexture->Bind();
	// this->modelMatrix = glm::mat4(1.0f);
	this->viewMatrix = this->camera.GetViewMatrix();
	this->projectionMatrix = glm::perspective(glm::radians(this->camera.fovDegrees), this->windowAspectRatio, this->camera.nearClipDistance, this->camera.farClipDistance);
	// this->shader_withTextures->SetMat4("modelMatrix", this->modelMatrix);
	this->shader_withTextures->SetMat4("viewMatrix", this->viewMatrix);
	this->shader_withTextures->SetMat4("projectionMatrix", this->projectionMatrix);
	// this->vaoMap->Bind();
	// glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	// Draw Player
	this->playerTexture->Activate();
	this->playerTexture->Bind();
	this->modelMatrix = glm::translate(glm::mat4(1.0f), this->playerPosition_f);
	this->shader_withTextures->SetMat4("modelMatrix", this->modelMatrix);
	this->vaoPlayer->Bind();
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	// Draw Light
	this->shader_light->Use();
	this->modelMatrix = glm::translate(glm::mat4(1.0f), this->light.lightPos);
	this->shader_withTextures->SetMat4("modelMatrix", this->modelMatrix);
	this->shader_withTextures->SetMat4("viewMatrix", this->viewMatrix);
	this->shader_withTextures->SetMat4("projectionMatrix", this->projectionMatrix);
	this->vaoLight->Bind();
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	// Drawing the UI
	this->shader_UI->Use();

	// Draw Cursor
	if(!this->camera.toggleFPS){
		this->modelMatrix = glm::translate(glm::mat4(1.0f), this->mousePositionUnit);
		this->shader_UI->SetMat4("modelMatrix", this->modelMatrix);
		this->cursorTexture->Activate();
		this->cursorTexture->Bind();
		this->vaoCursor->Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	SDL_GL_SwapWindow(this->window);
}

void Game::UpdatePerformanceData(){
	performanceData.currentTimeInSeconds = SDL_GetTicks() / 1000.0f;
	performanceData.deltaTimeInSeconds = performanceData.currentTimeInSeconds - performanceData.lastTimeInSeconds;
	performanceData.lastTimeInSeconds = performanceData.currentTimeInSeconds;
}

void Game::Quit(){
	this->cursorTexture->Delete();
	this->mapTexture->Delete();
	this->playerTexture->Delete();
	this->shader_withTextures->Delete();
	this->shader_UI->Delete();
	this->shader_light->Delete();
	this->vaoLight->Delete();
	this->vboLight->Delete();
	this->iboLight->Delete();
	this->vaoPlayer->Delete();
	this->vboPlayer->Delete();
	this->iboPlayer->Delete();
	this->vaoMap->Delete();
	this->vboMap->Delete();
	this->iboMap->Delete();
	this->vaoCursor->Delete();
	this->vboCursor->Delete();
	this->iboCursor->Delete();
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
	std::cout << "Player's Position: (" << this->playerPosition_f.x << ", " << this->playerPosition_f.y << ", " << this->playerPosition_f.z << ")" << std::endl;
}

void Game::PrintPlayerVelocity(){
	std::cout << "Player's Velocity: (" << this->playerVelocity_f.x << ", " << this->playerVelocity_f.y << ")" << std::endl;
}

void Game::PrintLocationPlayerHasToGo(){
	std::cout << "Location Player has to go = (" << this->locationPlayerHasToGo.x << ", " << this->locationPlayerHasToGo.y << ", " << this->locationPlayerHasToGo.z << ")" << std::endl;
}

void Game::PrintDeltaTime(){
	std::cout << "Delta Time: " << this->performanceData.deltaTimeInSeconds << std::endl;
}

bool Game::IsOutOfMapBounds(glm::vec3 pos){
	f32 leftBound = mapPosition_f.x - 0.5f*mapDimension_f.x;
	f32 rightBound = mapPosition_f.x + 0.5f*mapDimension_f.x;
	f32 farBound = mapPosition_f.z + 0.5f*mapDimension_f.z;
	f32 nearBound = mapPosition_f.z - 0.5f*mapDimension_f.z;
	if((pos.x < leftBound) || (pos.x > rightBound) || (pos.z < nearBound) || (pos.z > farBound)) return true;
	return false;
}