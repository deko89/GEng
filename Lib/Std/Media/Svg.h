#ifndef Svg_H
#define Svg_H

#include <variant>
#include <functional>
#include "Std/Base.h"
#include "Std/File.h"
#include "Std/ArDif.h"

constexpr auto streamSizeMax = std::numeric_limits<std::streamsize>::max();

namespace Svg
{

#ifndef M_Svg_OwnTypes
	#include "glm/glm.hpp"
	typedef float Val;
	typedef glm::vec2 Vec2;
	typedef glm::vec3 Vec3;
	typedef glm::mat3 Mat3;
	typedef uint32_t Color;
#endif

class ShapeLine
{	public:
	Vec2 a, b;
};

/// Типы команд Path.
enum CmdType : uint8_t
{	cmdM, cmdL, cmdC, cmdZ,
};
/// Информация о команде Path.
struct CmdInf
{	size_t size; ///< Размер в байтах.
	bool bMean; ///< Имеет ли смысл сама по себе (без других команд).
};
/// Команды Path.
struct CmdM
{	Vec2 p;
	CmdM(Vec2 p) : p(p) {}
};
struct CmdL
{	Vec2 p;
	CmdL(Vec2 p) : p(p) {}
};
struct CmdC
{	Vec2 a, b, c;
	CmdC(Vec2 a, Vec2 b, Vec2 c) : a(a), b(b), c(c) {}
};
struct CmdZ {};

class ShapePath
{	public:
	static const size_t nCmd = 3; ///< Число команд.
	static CmdInf aCmdInf[nCmd]; ///< Информация о командах.
	/// Набор команд (CmdType, CmdM, CmdType, CmdL, ...).
	ArDif aCmd;
	/// Имеет ли смысл (не пустой и т.п.).
	bool IsMean() const
	{	for (size_t i = 0; i < aCmd.size();)
		{	const CmdType c = (CmdType) aCmd[i];
			const CmdInf& inf = aCmdInf[c];
			if (inf.bMean) return 1;
			i += sizeof(CmdType) + inf.size;
		}
		return 0;
	}
	/// Добавить команду M (move).
	void M(Vec2 p)
	{	aCmd.Add<CmdType>(cmdM);
		aCmd.Add<CmdM>(p);
	}
	/// Добавить команду L (line to).
	void L(Vec2 p)
	{	aCmd.Add<CmdType>(cmdL);
		aCmd.Add<CmdL>(p);
	}
	/// Добавить команду C (cubic Bézier).
	void C(Vec2 a, Vec2 b, Vec2 c)
	{	aCmd.Add<CmdType>(cmdC);
		aCmd.Add<CmdC>(a, b, c);
	}
	/// Добавить команду Z (close path).
	void Z() { aCmd.Add<CmdType>(cmdZ); }
};

/// Стиль фигуры.
struct ShapeStyle
{
	Val w = 1.0;					///< Толщина линий.
	Color col = 0;					///< Цвет заливки.
	Color colStroke = 0xff000000;	///< Цвет линий.
};

/// Тип фигуры.
enum ShapeType : uint8_t
{
	stLine,
	//stRect,
	//stCircle,
	//stBezier, ///< Кривая Безье.
	stPath,
};

/// Данные фигуры.
struct ShapeData
{	/// Вариант фигуры. Совпадает с ShapeType.
	std::variant<ShapeLine, ShapePath> vShape;
	ShapeStyle style;
};

typedef std::function<void(const ShapeData&)> FunReadShape;
/** Прочитать svg.
	param[in] path - Путь до файла svg.
	param[in] f - Функция чтения фигуры. */
bool SvgRead(FilePath path, FunReadShape f);

}

#endif



#ifdef M_IncludeCpp
#include <algorithm>
#include <sstream>
#include <iostream>
#include "pugixml/src/pugixml.hpp"
#include "Std/TextRead.h"

namespace Svg
{

using namespace pugi;

FunReadShape funRead; ///< Функция чтения фигуры.

CmdInf ShapePath::aCmdInf[ShapePath::nCmd]
{	{sizeof(CmdM), false},
	{sizeof(CmdL), true},
	{0, false}, // CmdZ
};

/// Чтение из строки для Svg.
struct TextReadSvg : public TextRead
{
	using TextRead::TextRead;
	Color ReadColor()
	{	Color col = 0xff000000;
		if ( Cmp("rgb") )
		{
			SkipD('(');
			col.r = ReadColorPart(',');
			col.g = ReadColorPart(',');
			col.b = ReadColorPart(')');
		} else if ( Cmp("none") )
		{	return 0;
		} else
			std::cout << "Unknown color: " << s.substr(pos, 16) << std::endl;
		return col;
	}
protected:
	uint8_t ReadColorPart(Sym d)
	{
		StrV s;  s = ReadD(d);
		if ( s.empty() ) return 0;
		Val v = ToVal<Val>(s);
		if (s.back() == '%') v = 255.0 * v / 100.0;
		return v;
	}
};

struct SvgReadNode
{
	xml_node ndXml; ///< Читаемый узел.
	SvgReadNode* parent = nullptr; ///< Родительский узел.
	ShapeData shape; ///< Итоговые данные фигуры.
	Mat3 mTrans = Mat3(1); ///< Итоговая трансформация.

	SvgReadNode(xml_node ndXml, SvgReadNode* parent = nullptr) :
		ndXml(ndXml), parent(parent)
	{
	}
	void ReadChilds()
	{
		for ( SvgReadNode n(ndXml.first_child(), this);
			  n.ndXml;
			  n.ndXml = n.ndXml.next_sibling() )
		{
			n.ReadNode();
		}
	}
	void ReadNode()
	{
		ReadTransform();
		ReadStyle();
		const Str name = ndXml.name();
		if (name == "path")
		{	ReadPath();
		} else if (name == "g")
		{	ReadChilds();
		} else if (name == "defs")
		{
		}
	}
	void ReadPath()
	{
		ShapePath& shPath = shape.vShape.emplace<ShapePath>();
		Vec2 posSubPath(0, 0); // Начальная позиция подпути.
		textRead.Set( ndXml.attribute("d").value() );
		for (Sym cmd; textRead;)
		{
			// Начало подпути.
			cmd = textRead.ReadSym();
			if (cmd == 0) break;
			if (cmd == 'M')
			{	posSubPath = ReadPos();
				shPath.M(posSubPath);
			} else
				textRead.UnReadSym();
			// Чтение подпути.
			for (bool bReadSubPath = 1; bReadSubPath && textRead;)
			{
				cmd = textRead.ReadSym();
				switch (cmd)
				{
					case 0: // Конец у Path.
						break;
					case 'M':	shPath.M( ReadPos() );	break;
					case 'L':	shPath.L( ReadPos() );	break;
					case 'C':
					{	shPath.C( ReadPos(), ReadPos(), ReadPos() );
					}	break;
					case 'z': case 'Z':
					{	shPath.Z();
						bReadSubPath = 0;
					}	break;
					default:
						std::cout << "Unknown path command: " << cmd << std::endl;
						textRead.End();
				}
			}
			// Чтение.
			if ( shPath.IsMean() ) funRead(shape);
			shPath.aCmd.clear();
		}
	}
private:
	TextReadSvg textRead;
	void ReadTransform()
	{
		mTrans = parent? parent->mTrans : Mat3(1);
		xml_attribute a = ndXml.attribute("transform");
		std::istringstream ss( a.value() );
		while (ss)
		{
			while ( std::isspace(ss.peek()) ) ss.ignore(); // Пропуск пробелов.
			string type;  std::getline(ss, type, '(');
			if ( type.empty() ) break;
			string sTrans;  std::getline(ss, sTrans, ')');
			std::replace(sTrans.begin(), sTrans.end(), ',', ' ');
			if (type == "matrix")
			{
				std::istringstream ss(sTrans);
				Mat3 mat(1);
				ss >> mat[0][0];  ss >> mat[0][1];
				ss >> mat[1][0];  ss >> mat[1][1];
				ss >> mat[2][0];  ss >> mat[2][1];
				mTrans *= mat;
			} else
				std::cout << "Unknown transform: " << type << std::endl;
		}
	}
	void ReadStyle()
	{
		if (parent)
			shape.style = parent->shape.style;
		xml_attribute a = ndXml.attribute("style");
		if ( a.empty() ) return;
		TextReadSvg r( a.value() );
		while (r)
		{
			StrV name = r.ReadD(':');
			if (name == "fill")
			{	shape.style.col = r.ReadColor();
			} else if (name == "stroke")
			{	shape.style.colStroke = r.ReadColor();
			}
			r.SkipD(';');
		}
	}
	Vec2 ReadPos()
	{	Vec3 v;
		v.x = textRead.ReadD<Val>();
		v.y = textRead.ReadD<Val>();
		v.z = 1;
		v = mTrans * v;
		return Vec2(v.x, v.y);
	}
};

bool SvgRead(FilePath path, FunReadShape f)
{
	xml_document doc;
	xml_parse_result result = doc.load_file( path.c_str() );
	if (!result)
	{
		std::cerr << "Error load svg " << path << std::endl;
		return 0;
	}
	funRead = f;
	SvgReadNode r( doc.child("svg") );
	r.ReadChilds();
	return 1;
}

}

#endif
