#pragma once

#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/detail/type_vec2.hpp>

#include "Window.h"
#include "Player.h"
#include "Map.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Camera.h"
#include "main.h"
#include "Light.h"
#include "Material.h"
#include "Model.h"

class Game{
	
public:
	Game(const char* name, i32 windowWidth, i32 windowHeight);
	void HandleInput();
	void SimulateWorld();
	void RenderGraphics();
	void UpdatePerformanceData();
	void Quit();

public:	
	//window stuff
	Window* 		window 							= nullptr;
	
	//performance
	PerformanceData performanceData;

	//flags
	b8 				isRunning						= false;
	b8 				isFullscreen					= false;
	b8 				isVSyncEnabled					= true;
	b8 				isValid							= false;
	
	//shaders, vaos, vbos, ebos
	Shader* 		shader_withTextures				= nullptr;
	Shader* 		shader_light 					= nullptr;
	Shader* 		shader_diffuse_specular 		= nullptr;

	//shader matrices
	glm::mat4 		modelMatrix 					= glm::mat4(1.0f);
	glm::mat4 		viewMatrix 						= glm::mat4(1.0f);
	glm::mat4 		projectionMatrix 				= glm::mat4(1.0f);

	//mouse stuff
	glm::ivec2 		lastMouseClickPosition 			= glm::ivec2(0, 0);
	glm::vec2 		lastMouseClickPositionUnit 		= glm::vec2(0.0f);
	glm::ivec2 		mousePosition					= glm::ivec2(0, 0);
	glm::vec3 		mousePositionUnit				= glm::vec3(0.0f, 0.0f, 0.0f);
	glm::ivec2 		lastMousePosition				= glm::ivec2(0, 0);
	b8 				firstMouseInput 				= true;
	i32 			pixelsFromBorderToMove 			= 15;
	b8 				mouseWasClicked 				= false;

	//player parameters
	Player* 		player 							= nullptr;
	f32 			distanceForPlayerToStopMoving	= 0.01f;

	//map parameters
	Map* 			map 							= nullptr;
	
	Camera 			camera;

	// Lights
	Light* 			light							= nullptr;

	// backpack (testing)
	Model* backpackModel 							= nullptr;

	// Materials
	Material 		material;

private:
	void PrintDeltaTime();
	void SetupCursor(const char* image_path);
	bool IsOutOfMapBounds(glm::vec3 pos);
	glm::vec3 CalculateWherePlayerNeedsToGoBasedOnNormalizedCursorPosition(f32 x, f32 y); // coordinates should be normalized
};