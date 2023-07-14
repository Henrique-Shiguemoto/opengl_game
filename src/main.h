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

#define GAME_NAME 		"OpenGL Game"
#define WINDOW_WIDTH 	800
#define WINDOW_HEIGHT 	600
#define ASPECT_RATIO 	((f32)WINDOW_WIDTH / (f32)WINDOW_HEIGHT)

struct PerformanceData{
	u32 currentTime 				= 0;
	u32 lastTime 					= 0;
	u32 framesSinceFrametimeUpdate 	= 0;
	u32 framesPerUpdate 			= 60;
	u32 frameTimeSum 				= 0;
};

b8 Init();
void HandleInput();
void SimulateWorld();
void RenderGraphics();
void UpdateFrametimeInWindowTitle(PerformanceData& perfData);
void Quit();