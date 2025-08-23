#include "Context.h"

namespace GEng
{

Context::~Context()
{	Clear();
}

void Context::Init()
{
	// Decide GL+GLSL versions
	#if defined(IMGUI_IMPL_OPENGL_ES2)
	// GL ES 2.0 + GLSL 100
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	#elif defined(IMGUI_IMPL_OPENGL_ES3)
	// GL ES 3.0 + GLSL 300 es (WebGL 2.0)
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	#elif defined(__APPLE__)
	// GL 3.2 Core + GLSL 150
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	#else
	// GL 3.0 + GLSL 130
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	#endif
}

bool Context::Create(Wnd& wnd)
{
	SDL_Window* pWnd = wnd.NativeWnd();
	gl_context = SDL_GL_CreateContext(pWnd);
	//SDL_GL_MakeCurrent(pWnd, gl_context);
	const GLubyte* version = glGetString(GL_VERSION);
	if (version == 0)
	{	printf( "Unable to get OpenGL ES version string: %d\n", glGetError() );
		return 0;
	}
	printf("GL_VERSION: %s\n", version);
	SDL_GL_SetSwapInterval(1); // Enable vsync

	glEnable(GL_DEPTH_TEST);	// Включить тест глубины.
	glDisable(GL_DEPTH_CLAMP);	// Включить отсечение по расстоянию от камеры.
	glEnable(GL_CULL_FACE);	// Отсекать грани которые по часовой стрелки.
	// Прозрачность.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return 1;
}

void Context::Clear()
{	if (gl_context)
	{	SDL_GL_DeleteContext(gl_context);
		gl_context = 0;
	}
}

SDL_GLContext& Context::NativeContext()
{	return gl_context;
}

}
