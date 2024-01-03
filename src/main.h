#pragma once

#define GAME_NAME 				"OpenGL Game"
#define WINDOW_WIDTH 			840
#define WINDOW_HEIGHT 			640
#define ASPECT_RATIO 			((f32)WINDOW_WIDTH / (f32)WINDOW_HEIGHT)

struct PerformanceData{
	f32 currentTimeInSeconds 		= 0;
	f32 lastTimeInSeconds			= 0;
	f32 deltaTimeInSeconds			= 0.0f;
};
