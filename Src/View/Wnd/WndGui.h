#ifndef WNDGUI_H
#define WNDGUI_H

#include "imgui.h"
#include "GEng/View/Wnd/WndG.h"

namespace GEng
{

class WndGui : public WndG
{	public:
	~WndGui();
	void Clear();					///< Очистить.
	void CreateGui();	///< Создать интерфейс.
	void ClearGui();	///< Удалить интерфейс.
	void ProcessEvent(SDL_Event& event); ///< Обработка события.
	void Draw(); ///< Рисование.
protected:
	ImGuiIO* pIO = nullptr;
    // Our state
    bool show_demo_window = false;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    void UpdateUI();
};

}

#endif
