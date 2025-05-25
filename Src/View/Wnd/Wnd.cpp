#include "Wnd.h"

namespace GEng
{

Wnd::~Wnd()
{	Clear();
}

bool Wnd::Create(const Str& name, const RectI& pos, uint32_t flag)
{
    pWnd = SDL_CreateWindow(name.c_str(), pos.x, pos.y, pos.w, pos.h, flag);
    if (pWnd == nullptr)
    {   printf( "Error: SDL_CreateWindow: %s\n", SDL_GetError() );
        return 0;
    }
    return 1;
}

void Wnd::Clear()
{	if (pWnd)
	{	SDL_DestroyWindow(pWnd);
		pWnd = nullptr;
	}
}

SDL_Window* Wnd::NativeWnd() const
{
	return pWnd;
}

}
