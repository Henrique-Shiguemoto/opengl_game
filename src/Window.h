#pragma once

#include "types.h"

#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"

#include "stb_image.h"

#include <iostream>

typedef enum _WindowFlags{
	WINDOW_FULLSCREEN 			= 0x00000001,
} WindowFlags;

class Window{
public: 
	Window(const char* title, i32 width, i32 height, u32 flags);
	void ChangeIcon(const char* image_path);
	void SwapWindow();
	void Delete();

	const char* 	title;
	i32 			width;
	i32 			height;
	f32 			aspectRatio;
	u32  			flags;

private:
	SDL_Window* 	_window;
	SDL_GLContext 	_context;

	void _init_SDL_OPENGL_stuff(u32 flags);
};