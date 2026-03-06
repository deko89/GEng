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
}

void WndG::ProcessEventMouse(SDL_Event& event)
{
	if (view)
		view->ProcessEventMouse(event);
}

void WndG::ProcessEventKeyboard(SDL_Event& event)
{
	if (view)
		view->ProcessEventKeyboard(timeDelta);
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

void WndG::UpdateTimeDelta()
{
	Time timeNow = system_clock::now();
	duration<Val> dur = timeNow - timePrev;
	timePrev = timeNow;
	timeDelta = dur.count();
}

void WndG::Draw()
{
	// Рисование
	for (View* v : aView)
	{
		v->Update(timeDelta);
		v->Draw();
	}
}

}
