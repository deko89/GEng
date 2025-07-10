#include "Shapes.h"
#include "imgui.h"

namespace GEng
{

ImDrawList* gDrawL;
/// Позиция начала View (0,0 - с учётом полей и т.п.). Глобальные координаты.
Vec2 gViewBegPos;

/// Координаты из логических в глобальные.
Vec2 ToGlobC(const Vec2& p)
{
	return gViewBegPos + p;
}

// Line //////////////////////////////////////////////////////////////
void Line::Draw() const
{
	gDrawL->AddLine(ToGlobC(a), ToGlobC(b), c, w);
}
// ShPath ////////////////////////////////////////////////////////////
void ShPath::Draw() const
{
	using namespace Svg;
	auto Draw = [this](int flag = 0)
	{	if (style.col)
		{	gDrawL->PathFillConvex(style.col);
		} else
			gDrawL->PathStroke(style.colStroke, flag, style.w);
	};
	for (size_t pos = 0; pos < aCmd.size();)
	{	CmdType type = aCmd.Read<CmdType>(pos);
		switch (type)
		{
			case CmdType::cmdM:
			{	Draw();
				CmdM& c = aCmd.Read<CmdM>(pos);
				gDrawL->PathLineTo( ToGlobC(c.p) );
			}	break;
			case CmdType::cmdL:
			{	CmdL& c = aCmd.Read<CmdL>(pos);
				gDrawL->PathLineTo( ToGlobC(c.p) );
			}	break;
			case CmdType::cmdC:
			{	CmdC& c = aCmd.Read<CmdC>(pos);
				gDrawL->PathBezierCubicCurveTo( ToGlobC(c.a),
												ToGlobC(c.b),
												ToGlobC(c.c) );
			}	break;
			case CmdType::cmdZ:
			{	Draw(ImDrawFlags_Closed);
			}	break;
		}
	}
	Draw();
}
// Shapes ////////////////////////////////////////////////////////////
void Shapes::LoadSvg(FilePath path)
{
	using namespace Svg;
	auto ReadShape = [this](const ShapeData& d)
	{	switch ( d.vShape.index() )
		{
			case stLine:
			{
				//Add( new Line(d.a, d.b, d.style.w, d.style.colStroke) );
			}	break;
			case stPath:
			{
				ShPath* p = new ShPath;
				p->ShapePath::operator=( std::get<ShapePath>(d.vShape) );
				p->style = d.style;
				Add(p);
			}	break;
		}
	};
	SvgRead(path, ReadShape);
}
void Shapes::Draw() const
{
	gDrawL = ImGui::GetWindowDrawList();
	gViewBegPos = ImGui::GetCursorScreenPos();
	for (Shape* s : *this)
		s->Draw();
}

}
