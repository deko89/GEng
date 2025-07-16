#ifndef CONTEXT_H
#define CONTEXT_H

/** \file
	\brief Контекст.
	Даёт доступ к графической библиотеке (можно считать что это она и есть).
*/

#include "GEng/Base/Meta/Meta.h"
#include "GEng/View/Wnd/Wnd.h"

namespace GEng
{

/// Версия.
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* const glsl_version = "#version 100";
#elif defined(IMGUI_IMPL_OPENGL_ES3)
	// GL ES 3.0 + GLSL 300 es (WebGL 2.0)
	#define M_GlslVer "#version 300 es"
	const char* const glsl_version = M_GlslVer;
#elif defined(__APPLE__)
    // GL 3.2 Core + GLSL 150
    const char* const glsl_version = "#version 150";
#else
	#define M_GlslVer "#version 330 core"
	const char* const glsl_version = M_GlslVer;
#endif

/// Контекст графики.
class Context
{	public:
	~Context();
	void Init();
	bool Create(Wnd& wnd);	///< Создание (требует окно).
	void Clear();
	SDL_GLContext& NativeContext();
private:
	SDL_GLContext gl_context = 0;
};

/// Получить текущий контекст.
Context& GetContext(); // Реализовано в Engine.cpp

}

#endif
