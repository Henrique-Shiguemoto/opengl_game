#include "Window.h"

Window::Window(const char* title, i32 width, i32 height, u32 flags){
	this->title = title;
	this->width = width;
	this->height = height;
	this->flags = flags;
	
	if(this->height > 1){
		this->aspectRatio = (f32)this->width / (f32)this->height;
	}else{
		std::cout << "Invalid aspect ratio! Width = " << this->width << " - Height = " << this->height << "\n";
	}

	this->_init_SDL_OPENGL_stuff(flags);
}

void Window::_init_SDL_OPENGL_stuff(u32 flags){
	if(SDL_Init(SDL_INIT_VIDEO)){
		std::cout << "SDL2 Couldn't Initialize!\n" << std::endl;
		return;
	}

	this->_window = SDL_CreateWindow(this->title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | flags);
	if(!this->_window){
		std::cout << "Couldn't create the window!\n" << std::endl;
		return;
	}

	// I'm currently using RenderDoc for graphics debugging and it is important that the code is in this order to avoid errors in RenderDoc, 
	// 		SDL_Init => SDL_CreateWindow => SDL_GL_SetAttribute => SDL_GL_CreateContext.
	// If it's not in this order, RenderDoc will show an error (at least on my machine) along the lines of "Context not created by CreateContextAttribs. Captures Disabled."
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	this->_context = SDL_GL_CreateContext(this->_window);
	if(!this->_context){
		std::cout << "Couldn't create OpenGL context!\n" << std::endl;
		return;
	}
}

void Window::ChangeIcon(const char* image_path){
	i32 iconWidth, iconHeight, iconChannelCount;
	u8* iconPixels = stbi_load(image_path, &iconWidth, &iconHeight, &iconChannelCount, 0);
	if(!iconPixels){
		std::cout << "Couldn't load icon image" << std::endl;
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
	SDL_SetWindowIcon(this->_window, iconSurface);
	SDL_FreeSurface(iconSurface);
	stbi_image_free(iconPixels);
}

void Window::SwapWindow(){
	SDL_GL_SwapWindow(this->_window);
}

void Window::Delete(){
	SDL_GL_DeleteContext(this->_context);
	SDL_DestroyWindow(this->_window);
}