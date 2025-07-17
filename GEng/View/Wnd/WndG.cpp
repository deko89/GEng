#include "WndG.h"

namespace GEng
{

WndG::WndG()
{
    timePrev = system_clock::now();
    //SDL_SetRelativeMouseMode(SDL_TRUE);
}

void WndG::ProcessEvent(SDL_Event& event)
{
	for (View* v : aView)
		v->ProcessEvent(event);
	if (view && event.type == SDL_MOUSEMOTION)
		view->ProcessEventInput(event);
    if (event.type == SDL_KEYDOWN)
    {
    	switch (event.key.keysym.scancode)
    	{
    		case SDL_SCANCODE_LCTRL:
    			SDL_SetRelativeMouseMode(SDL_GetRelativeMouseMode()? SDL_FALSE: SDL_TRUE);
	    		break;
    		case SDL_SCANCODE_F2:
    			SetPolygonMode(PolygonMode::Point);
	    		break;
    		case SDL_SCANCODE_F3:
    			SetPolygonMode(PolygonMode::Line);
	    		break;
    		case SDL_SCANCODE_F4:
    			SetPolygonMode(PolygonMode::Fill);
	    		break;
	    	default: ;
    	}
    }
}

void WndG::Draw()
{
	// Прошедшее время.
	Time timeNow = system_clock::now();
	duration<Val> dur = timeNow - timePrev;
	timePrev = timeNow;
	Val timeDelta = dur.count();
	// Обновление активного вида.
	if (view)
		view->ProcessStateInput(timeDelta);
	// Рисование
	for (View* v : aView)
	{
		v->Update(timeDelta);
		v->Draw();
	}
}

}
