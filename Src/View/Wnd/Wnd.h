#ifndef WND_H
#define WND_H

#include "Base/Environment.h"

namespace GEng
{

/// Размер окна по умолчанию.
const Vec2I gSzWndDef(1280, 720);

/// Окно.
class Wnd
{	public:
	~Wnd();
	/** Создать окно.
		param[in] name - имя окна.
		param[in] pos - позиция окна. По x, y если SDL_WINDOWPOS_CENTERED, то по центру.
		param[in] flag - флаги окна SDL (SDL_WINDOW_FULLSCREEN, ...). */
	bool Create(const Str& name, const RectI& pos, uint32_t flag = 0);
	void Clear();
	SDL_Window* NativeWnd() const;
protected:
	SDL_Window* pWnd = nullptr;
};

}

#endif
