#ifndef View_H
#define View_H

#include "GEng/World/World.h"
#include "GEng/View/Camera.h"
#include "GEng/World/Shapes.h"

namespace GEng
{

/// Вид. Область в окне показывающая мир.
class View
{	public:
	World* world = nullptr; ///< Отображаемый мир.
	Camera cam;             ///< Камера.
	Shapes aShape;			///< Интерфейс и 2d фигуры.
	View(World* w, const RectI& pos);
	const RectI& GetPos() const;
	void SetPos(const RectI& r);
	void Update(Val timeDelta); ///< Обновить.
	void Draw();
	void ProcessEvent(SDL_Event& event); ///< Обработка события.
	void ProcessEventInput(SDL_Event& event); ///< Обработка события клавиатуры / мыши.
	void ProcessStateInput(Val timeDelta); ///< Обработка состояния клавиатуры / мыши.
private:
	RectI pos;              ///< Позиция в окне.
};

}

#endif
