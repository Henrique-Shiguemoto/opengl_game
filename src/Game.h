#pragma once

#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/detail/type_vec2.hpp>

#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "main.h"

class Game{
	
public:
	Game(const char* name, i32 windowWidth, i32 windowHeight, const char* vertexShaderFilepath, const char* fragmentShaderFilepath, const char* windowIconFilepath);
	void HandleInput();
	void SimulateWorld();
	void RenderGraphics();
	void UpdatePerformanceData();
	void Quit();

public:
	std::string name 								= "Custom Game Name";
	
	//window stuff
	SDL_Window* window 								= nullptr;
	SDL_GLContext context 							= NULL;
	i32 windowWidth 								= 800;
	i32 windowHeight 								= 600;
	f32 windowAspectRatio							= windowWidth / windowHeight;
	
	//performance
	PerformanceData performanceData;

	//flags
	b8 isRunning 									= false;
	b8 isFullscreen 								= false;
	b8 isVSyncEnabled 								= true;
	b8 isValid										= false;
	
	//opengl stuff
	Shader* shader 									= nullptr;
	VertexArray*  vaoPlayer							= nullptr;
	VertexBuffer* vboPlayer							= nullptr;
	IndexBuffer*  iboPlayer 						= nullptr;
	VertexArray*  vaoMap							= nullptr;
	VertexBuffer* vboMap							= nullptr;
	IndexBuffer*  iboMap 							= nullptr;

	//mouse stuff
	glm::ivec2 lastMouseClickPosition 				= glm::ivec2(0, 0);
	glm::vec2 lastMouseClickPositionNormalized 		= glm::vec2(0.0f);
	glm::ivec2 mousePosition						= glm::ivec2(windowWidth / 2, windowHeight / 2);
	glm::ivec2 lastMousePosition					= glm::ivec2(0, 0);
	f32 mouseSensitivity 							= 0.1f;
	b8 firstMouseInput 								= true;

	//player parameters
	glm::vec3 playerPosition_f 						= glm::vec3(0.0f, 5.0f, 0.0f);
	glm::vec3 playerDimension_f 					= glm::vec3(3.0f, 3.0f, 3.0f);
	glm::vec3 playerVelocity_f 						= glm::vec3(0.0f);
	f32 playerMaximumSpeed 							= 0.05f;
	b8 playerStartedMoving 							= false;
	b8 playerIsMoving 								= false;
	f32 distanceForPlayerToStopMoving 				= 0.005f;

	//map parameters
	glm::vec3 mapPosition_f							= glm::vec3(0.0f);
	glm::vec3 mapDimension_f						= glm::vec3(25.0f, 1.0f, 25.0f);
	
	//camera parameters
	glm::vec3 cameraPosition_f						= glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront_f 						= glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp_f							= glm::vec3(0.0f, 1.0f, 0.0f);
	f32 cameraMaximumSpeed 							= 2.5f;
	f32 fovDegrees 									= 45.0f;
	f32 nearClipDistance 							= 0.1f;
	f32 farClipDistance 							= 100.0f;
	b8 cameraHasToMoveLeft 							= false;
	b8 cameraHasToMoveRight 						= false;
	b8 cameraHasToMoveFront 						= false;
	b8 cameraHasToMoveBack 							= false;
	f32 cameraYaw 									= -90.0f;
	f32 cameraPitch									= 0.0f;

private:
	void PrintMouseClickPosition();
	void PrintMouseClickPositionNormalized();
	void PrintPlayerPosition();
	void PrintPlayerVelocity();
	void PrintDeltaTime();
};