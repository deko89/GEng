#ifndef Camera_H
#define Camera_H

#include "SDL_events.h"
#include "GEng/View/Wnd/Wnd.h"

namespace GEng
{

/// Камера.
class Camera
{	public:
	Pos     pos;    ///< Позиция.
	Angle   angle;	///< Углы последовательного поворота x, y, z.
	float	fFov = pi4,									///< Обзор камеры (вертикальный угол).
			fAspect = (float)gSzWndDef.x / gSzWndDef.y,	///< Соотношение сторон.
			fNear = 0.04f,								///< Ближнее отсечение.
			fFar = 512;									///< Дальнее отсечение.
	Vec3 GetLook() const;	///< Получить направление взгляда (единичный вектор).
	Mat4 GetMatrix() const;	///< Получить общую матрицу положения камеры.
	Mat4 GetView() const;	///< Получить матрицу вида камеры.
	Mat4 GetProjection() const;	///< Получить матрицу проекции камеры.
	/** Получить вектор от взгляда, до точки на экране (не единичной длины).
		param[in] p - координаты точки от 0 до 1, от левого верхнего угла. */
	Vec3 CalcVecPoint(Vec2 p) const;
	void ProcessEventInput(SDL_Event& event); /// Обработка событий клавиатуры / мыши.
	void ProcessStateInput(Val timeDelta); ///< Обработка состояния клавиатуры / мыши.
private:
	const Vec3 vUp = {0.0f, 0.0f, 1.0f};    ///< Мировая ось вверх.
	Vec3 vLook; ///< Направление куда смотрит камера. Вычисляется из angle. Вектор единичный.
	void UpdateLook(); ///< Обновление вектора направления.
};

}

#endif
