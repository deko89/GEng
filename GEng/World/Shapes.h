#ifndef Shapes_H
#define Shapes_H

#include "Std/Base.h"
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

/// Меню.
class Menu : public Shape
{	public:
	struct Separator {};
	struct Item
	{	Str name;						///< Имя.
		//Str desc;						///< Описание.
		Str key;						///< Горячая клавиша.
		bool enable = true;
		std::function<void()> action;	///< Действие при нажатии.
	};
	struct Fold
	{	Str name;
		std::vector< std::variant<Separator, Item, Fold> > aItem;
		Fold(StrR name) : name(name) {}
		void Draw() const;
	};
	Fold& Add(StrR name) {return aFold.emplace_back(name);}
	virtual void Draw() const override;
private:
	std::vector<Fold> aFold;
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
