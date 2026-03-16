#ifndef WNDGUI_H
#define WNDGUI_H

#include "imgui/imgui.h"
#include "GEng/View/Wnd/WndG.h"

namespace GEng
{

class WndGui : public WndG
{	public:
	~WndGui();
	void Clear();		///< Очистить.
	void CreateGui();	///< Создать интерфейс.
	void ClearGui();	///< Удалить интерфейс.
	void ProcessEvent(SDL_Event& event); ///< Обработка события.
	void Draw(); ///< Рисование.
protected:
	ImGuiIO* pIO = nullptr;
    void UpdateUI();
};

}

#endif
