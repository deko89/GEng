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
    void Save(pugi::xml_node ndView);
    void Load(pugi::xml_node ndView);
	const RectI& GetPos() const;
	void SetPos(const RectI& r);
	Vec2I WndToView(Vec2I p) const; ///< Позиция в окне -> позиция в виде.
	void Update(Val timeDelta); ///< Обновить.
	void Draw();
	void ProcessEvent(SDL_Event& event); ///< Обработка общего события.
	void ProcessEventMouse(SDL_Event& event); ///< Обработка события мыши.
	void ProcessEventKeyboard(Val timeDelta); ///< Обработка состояния клавиатуры.
private:
	RectI pos;              ///< Позиция в окне.
	void DrawTransform();	///< Вывести инструменты трансформации выделенного.
};

}

#endif
