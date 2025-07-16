#ifndef Shapes_H
#define Shapes_H

#include "GEng/Base/Meta/Meta.h"
#include "Std/MassivPtr.h"
#define M_Svg_OwnTypes
namespace Svg {using namespace GEng;}
#include "Std/Media/Svg.h"

namespace GEng
{

/// Базовая 2d фигура.
class Shape
{	public:
	virtual ~Shape() {}
	virtual void Draw() const = 0;	///< Рисование.
};

/// Линия.
class Line : public Shape
{	public:
	Vec2 a, b;		///< Позиция.
	Val w;			///< Ширина.
	Color c;		///< Цвет.
	Line() {}
	Line(Vec2 a, Vec2 b, Val w, Color c) : a(a), b(b), w(w), c(c) {}
	virtual void Draw() const override;
};

/** Путь (svg).
	\details Фигура - многоугольник, полилиния, сплайн. */
class ShPath : public Shape, public Svg::ShapePath
{	public:
	Svg::ShapeStyle style;
	virtual void Draw() const override;
};

/// 2д фигуры.
class Shapes : public MassivPtr<Shape>
{	public:
	typedef MassivPtr<Shape> Base;
	void LoadSvg(FilePath path); ///< Загрузить из svg файла.
	void Draw() const;
};

}

#endif
