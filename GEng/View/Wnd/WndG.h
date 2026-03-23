#ifndef WndG_H
#define WndG_H

#include "Std/MassivPtr.h"
#include "GEng/View/Wnd/Wnd.h"
#include "GEng/View/View.h"

namespace GEng
{

/// Окно с графикой.
class WndG : public Wnd
{	public:
    MassivPtr<View> aView;      ///< Области.
    View* view = nullptr;       ///< Активная область.
    WndG();
    void Save(pugi::xml_node ndWnd);
    /** \brief Загрузка.
        \warning Пока только для 1 активного вида. */
    void Load(pugi::xml_node ndWnd);
    void ProcessEvent(SDL_Event& event); ///< Обработка общего события.
    void ProcessEventMouse(SDL_Event& event); ///< Обработка мыши.
    void ProcessEventKeyboard(SDL_Event& event); ///< Обработка клавиатуры.
    void Draw(); ///< Рисование.
    void UpdateTimeDelta(); ///< Обновить timeDelta.
private:
    Time timePrev;          ///< Время последнего рисования.
    Val timeDelta;          ///< Разница во времени с предыдущим кадром.
};

}

#endif
