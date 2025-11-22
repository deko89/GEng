/** \file
	\brief Основные типы в 3d.
	\details Более специфичные типы используемые в общем моделировании, графики.
	Без привязки к библиотекам (OpenGL, Vulcan, ...).
*/

#ifndef Types_H
#define Types_H

#include <vector>
#include "Std/MassivPtr.h"
#include "GEng/Base/Meta/Meta.h"
#include "GEng/Base/Math.hpp"

namespace GEng
{

using std::vector;

/// Луч.
struct Ray
{	Pos pos;	///< Позиция начала.
	Vec3 dir;	///< Направление.
};

/// Параллелепипед.
struct Box
{	Pos a, b;	///< Позиция начального и конечного угла.
	bool IsIn(Pos p) const; ///< Находится ли точка внутри.
	bool IsIntersect(const Ray& ray) const; ///< Пересекает ли луч.
};
extern Box box0;

/// Вершины.
class Points : public vector<Pos>
{	public:
	typedef vector<Pos> Base;

	/// Данные точек.
	struct Data
	{
		Points* aPoint = nullptr;
		Vec3 sz; ///< Размер.
		Data(Points& p);
	};

	/// Базовый модификатор точек.
	struct Modif
	{
		virtual ~Modif() {}
		/// Применить к точкам.
		virtual void Set(Points::Data& pData) = 0;
	};

	/// Модификаторы точек. Обёртки преобразований, хранящие значения преобразований. Что позволяет знать и менять их.
	struct Modifs : public MassivPtr<Modif>
	{
		/// Поворот.
		struct Rotate : public Modif
		{
			Angle angle = Angle(0, 0, 0);
			Rotate(const Angle& angle);
			virtual void Set(Points::Data& pData) override;
		};

		/// Заострение.
		struct Taper : public Modif
		{
			Val taper = 0; ///< Коэф. заострения. 1 - нет, 0 - максимальное.
			Os osMain; ///< Основная ось, вдоль которой идёт заострение.
			Taper(Val taper, Os osMain = osX);
			virtual void Set(Points::Data& pData) override;
		};

		/*	@brief Сплайн - плавная кривая линия.
			Строится из цилиндра по оси Z: устанавливаются координаты вершин по x, y (по z - не трогаются). */
		struct Spline : public Modif
		{
			vector<Pos> aKey; ///< Ключевые точки, через которые проходит линия.
			Os osMain; ///< Основная ось, вдоль которой идёт "цилиндр" для сплайна.
			Spline(const vector<Pos>& aKey, Os osMain = osX);
			virtual void Set(Points::Data& pData) override;
		};

		/// Применить модификаторы.
		void Set(Points::Data& pData);
	};

	/// Создать прямоугольник.
	void MakeQuad(Val szX, Val szY);

	/** Создать плоскость.
		param[in] szX, szY - размеры.
		param[in] sgmX, sgmY - число сегментов. */
	void MakePlane(Val szX, Val szY, ValN sgmX, ValN sgmY);

	/** Создать куб.
		param[in] s - размер. */
	void MakeCube(Val s);

	/** Создать параллелепипед.
		param[in] a, b - позиция начала и конца. */
	void MakeBox(Pos a = {0, 0, 0}, Pos b = {1, 1, 1});

	/** Создать цилиндр
		param[in] d, len - диаметр и длина;
		param[in] sgmC, sgmL - количество сегментов в круге, и по длине;
		param[in] bCloseB, bCloseE - закрыть кругом начало, и конец. */
	void MakeCylinder(Val d, Val len, ValN sgmC, ValN sgmL, bool bCloseB = 1, bool bCloseE = 1);

	// Модификаторы. Собственно выполняют преобразование своих точек.
	/// Смещение.
	void Move(const Vec3& v);

	/// Поворот.
	void Rotate(const Angle& angle);

	/** Заострение.
		param[in] t - коэф. размера. 1 - заострения нет, 0 - максимальное;
		param[in] length - длина фигуры;
		param[in] osMain - основная ось, вдоль которой идёт заострение. */
	void Taper(Val t, Val length, Os osMain = osX);

	/** Сплайн. Сделать из точек ("цилиндра") плавную кривую линию, проходящую через заданные точки.
		param[in] aPoint - точки через которые проходит кривая;
		param[in] osMain - основная ось, вдоль которой идёт "цилиндр" для сплайна. */
	void Spline(const vector<Pos>& aKey, Os osMain = osX);
};

typedef Points::Modifs Modifs;

/// Индексы (например точек).
typedef vector<Ind> Indexes;

/** Сетка.
	\details Геометрическая форма из отдельных треугольников. Вершин и правил их соединения в грани.
	Например, по 3 индекса для каждого треугольника. */
class Mesh
{	public:
	Points aVert; ///< Вершины.
	Indexes aInd; ///< Порядок соединения точек в грани.

	/// Создание фигур, см Points.
	void MakeQuad(Val szX, Val szY);
	void MakePlane(Val szX, Val szY, ValN sgmX, ValN sgmY);
	void MakeBox(Pos a = {0, 0, 0}, Pos b = {1, 1, 1});
	void MakeCubeIn(Val s); ///< Создать куб, который виден изнутри.
	void MakeCylinder(Val d, Val len, ValN sgmC, ValN sgmL, bool bCloseB = 1, bool bCloseE = 1);
};

/// Координаты текстуры. Предполагается что каждая координата будет соответствовать отдельной вершине.
class PlaceTex: public vector<PosTex>
{	public:
	typedef vector<PosTex> Base;

	/// Квадрат.
	void SetQuad();

	/** Плоскость.
		param[in] sgmX, sgmY - количество сегментов по осям X, Y.
		param[in] u, v - число повторений текстуры по осям X, Y. */
	void SetPlane(ValN sgmX, ValN sgmY, Val u, Val v);

	/** Параллелепипед.
		Подходит только для текстур допускающих отражение по диагонали. */
	void SetBox();

	/** Цилиндр.
		param[in] sgmC - количество сегментов в круге. Должен быть чётным.
		param[in] sgmL - количество сегментов по длине.
		param[in] u, v - число повторений текстуры. */
	void SetCylinder(ValN sgmC, ValN sgmL, Val u, Val v);
};

}

#endif
