#ifndef WndG_H
#define WndG_H

#include "Std/MassivPtr.h"
#include "View/Wnd/Wnd.h"
#include "View/View.h"

namespace GEng
{

/// Окно с графикой.
class WndG : public Wnd
{	public:
    MassivPtr<View> aView;      ///< Области.
    View* view = nullptr;       ///< Активная область.
    WndG();
    void ProcessEvent(SDL_Event& event); ///< Обработка события.
    void Draw(); ///< Рисование.
private:
    Time timePrev;          ///< Время последнего рисования.
};

}

#endif
