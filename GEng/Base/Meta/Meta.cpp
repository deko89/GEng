#include "Meta.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

namespace GEng
{

bool Rect2::IsIntersect(Ray2 ray) const
{
	// Проверка что луч может пересечь - идёт в сторону прямоугольника.
	// Луч это половинка прямой. Если прямая пересечёт, такой луч тоже.
	if (ray.pos.x < a.x && ray.dir.x <= 0 ||	// Луч слева и идёт влево?
		ray.pos.x > b.x && ray.dir.x >= 0 ||	// Луч справа и идёт вправо?
		ray.pos.y < a.y && ray.dir.y <= 0 ||	// Луч снизу и идёт вниз?
		ray.pos.y > b.y && ray.dir.y >= 0)		// Луч сверху и идёт вверх?
		return false; // Тогда не может.
	// Особый случай - вертикальный луч. Он не подходит для уравнения.
	// После проверки - если луч вертикальный, то начало луча внутри по оси X.
	if (ray.dir.x == 0) return true;
	// Горизонтальный луч. Проверка для оптимизации и избежания ошибок.
	if (ray.dir.y == 0) return true;
	// Проверка пересечения с прямой.
		// Получение коэффициента луча (угла).
	const Val r = ray.dir.y / ray.dir.x;
		// Проверка точки пересечения с левой стороной прямоугольника.
	Val yRayL = ray.pos.y + r * (a.x - ray.pos.x); // Позиция луча Y слева.
	if (a.y <= yRayL && yRayL <= b.y) return true; // Если Y в прямоугольнике.
		// Проверка точки пересечения с правой стороной прямоугольника.
	Val yRayR = ray.pos.y + r * (b.x - ray.pos.x); // Позиция луча Y справа.
	if (a.y <= yRayR && yRayR <= b.y) return true; // Если Y в прямоугольнике.
		// Проверка пересечения с нижней стороной прямоугольника.
	if (yRayL < a.y && yRayR > a.y ||
		yRayL > a.y && yRayR < a.y) return true;
		// Проверка пересечения с верхней стороной прямоугольника.
	if (yRayL < b.y && yRayR > b.y ||
		yRayL > b.y && yRayR < b.y) return true;
	return false;
}

std::ostream& operator<<(std::ostream& os, const Vec2& v)
{
	return os << glm::to_string(v);
}

std::ostream& operator<<(std::ostream& os, const Vec3& v)
{
	return os << glm::to_string(v); //v.x << " " << v.y << " " << v.z;
}

std::ostream& operator<<(std::ostream& os, const Vec4& v)
{
	return os << glm::to_string(v); //v.x << " " << v.y << " " << v.z << " " << v.w;
}

std::ostream& operator<<(std::ostream& os, const Mat4& m)
{
	return os << glm::to_string(m);
}

void SetPolygonMode(PolygonMode mode)
{
#if !defined(IMGUI_IMPL_OPENGL_ES2) && !defined(IMGUI_IMPL_OPENGL_ES3)
	GLenum m;
	switch (mode)
	{	case PolygonMode::Point:	m = GL_POINT;	break;
		case PolygonMode::Line:		m = GL_LINE;	break;
		case PolygonMode::Fill:		m = GL_FILL;	break;
	}
	glPolygonMode(GL_FRONT_AND_BACK, m);
#endif
}

}
