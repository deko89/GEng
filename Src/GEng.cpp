#include "GEng/GEng.h"
#include <set>
#ifdef __EMSCRIPTEN__
	#include "imgui/examples/libs/emscripten/emscripten_mainloop_stub.h"
#endif

#define M_IncludeCpp

#ifdef One_Translation_Unit
	#include "Std/File.cpp"
    #include "Std/Media/Image.cpp"
	#include "Std/Media/Svg.h"
    #include "Base/Meta/Meta.cpp"
	#include "Base/Meta/Shader.cpp"
	#include "Base/Meta/MemG.cpp"
	#include "Base/Meta/Model.cpp"
	#include "Base/Meta/Texture.cpp"
	#include "Base/Environment.cpp"
	#include "Base/Context.cpp"
	#include "Base/Types.cpp"
	#include "Base/Math.hpp"
	#include "World/Models.cpp"
	#include "World/Plant.cpp"
	#include "World/World.cpp"
	#include "World/SkyBox.cpp"
	#include "World/Shapes.cpp"
	#include "View/Wnd/Wnd.cpp"
	#include "View/Camera.cpp"
	#include "View/View.cpp"
	#include "View/Wnd/WndG.cpp"
	#include "View/Wnd/WndGui.cpp"
#endif

namespace GEng
{

Engine eng;

// Engine ///////////////////////////////////////////////////////////
Engine::Engine()
{
}

Engine::~Engine()
{
}

bool Engine::Init()
{
    // Инициализация.
	if ( !IsEnvironment() ) return false;
	context.Init();
	RectI posWnd {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, gSzWndDef.x, gSzWndDef.y};
	if ( !wnd.Wnd::Create("RpGame", posWnd, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI) )
		return false;
	if ( !context.Create(wnd) )
		return false;
	shaders.Compile();
    // Создание окна.
	wnd.CreateGui();
	return true;
}

View* Engine::CreateViewWorld()
{
    // Создание мира.
    World* world = aWorld.Add();
    // Создание вида.
    RectI posView {0, 0, gSzWndDef.x, gSzWndDef.y};
    wnd.view = wnd.aView.Make(world, posView);
    return wnd.view;
}

void Engine::Clear()
{
	wnd.ClearGui();
	context.Clear();
	wnd.Wnd::Clear();
}

int Engine::Execute()
{
    bRun = true;
	SDL_Event event;
	// Главный цикл.
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (bRun)
#endif
    {	// Обработка списка событий.
		while ( SDL_PollEvent(&event) )
		{	wnd.ProcessEvent(event);
			// Событие выхода.
			if ( event.type == SDL_QUIT ||
				(	event.type == SDL_WINDOWEVENT &&
					event.window.event == SDL_WINDOWEVENT_CLOSE &&
					event.window.windowID == SDL_GetWindowID( wnd.NativeWnd() )
				)
			   )
			{	bRun = false;
			}
		}
		// Рисование.
        wnd.Draw();
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif
    return 0;
}

WndGui& Engine::GetWnd()
{   return wnd;
}

Context& GetContext()
{	return eng.context;
}

Shaders& GetShaders()
{	return eng.shaders;
}

}
