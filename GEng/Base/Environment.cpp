#include "Environment.h"
#include <stdio.h>
#include "Std/Media/Image.h"

// Environment //////////////////////////////////////////////////////
namespace GEng
{

Str gAppPathS;

Environment::Environment()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
	{
		printf(_("Error: %s\n"), SDL_GetError());
		return;
	}

	Image::Init();

	// Параметры окна.
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	// From 2.0.18: Enable native IME.
	#ifdef SDL_HINT_IME_SHOW_UI
	SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
	#endif

	// Путь до приложения.
	char* cPath = SDL_GetBasePath();
	gAppPathS = cPath;
	SDL_free(cPath);
}
Environment::~Environment()
{
	Image::DeInit();
	SDL_Quit();
}
bool Environment::IsEnvironment()
{	return SDL_WasInit(0) != 0;
}

}
