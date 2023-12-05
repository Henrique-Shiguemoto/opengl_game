#pragma once

typedef char i8;
typedef short i16;
typedef int i32;
typedef long int i64;
typedef float f32;
typedef double f64;
typedef bool b8;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

#define GAME_NAME 				"OpenGL Game"
#define WINDOW_WIDTH 			840
#define WINDOW_HEIGHT 			640
#define ASPECT_RATIO 			((f32)WINDOW_WIDTH / (f32)WINDOW_HEIGHT)

struct PerformanceData{
	f32 currentTimeInSeconds 		= 0;
	f32 lastTimeInSeconds			= 0;
	f32 deltaTimeInSeconds			= 0.0f;
};