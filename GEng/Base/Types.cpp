#include "Types.h"

namespace GEng
{
// Box ///////////////////////////////////////////////////////////////
Box box0;
bool Box::IsIn(Pos p) const
{	return	p.x > a.x && p.y > a.y && p.z > a.z &&
			p.x < b.x && p.y < b.y && p.z < b.z;
}
bool Box::IsIntersect(const Ray& ray) const
{	// Проверка пересечения на плоскости XY.
	Rect2 rect2{.a.x = a.x, .a.y = a.y, .b.x = b.x, .b.y = b.y};
	Ray2  ray2{	.pos.x = ray.pos.x, .pos.y = ray.pos.y,
				.dir.x = ray.dir.x, .dir.y = ray.dir.y};
	if ( !rect2.IsIntersect(ray2) ) return false;
	// Проверка пересечения на плоскости XZ.
	rect2.a.y = a.z;
	rect2.b.y = b.z;
	ray2.pos.y = ray.pos.z;
	ray2.dir.y = ray.dir.z;
	if ( !rect2.IsIntersect(ray2) ) return false;
	// Проверка пересечения на плоскости YZ.
	rect2.a.x = a.y;
	rect2.b.x = b.y;
	ray2.pos.x = ray.pos.y;
	ray2.dir.x = ray.dir.y;
	return rect2.IsIntersect(ray2);
}
// Points ////////////////////////////////////////////////////////////
	// Data //////////////////////////////////////////////////////////
Points::Data::Data(Points& p) :
	aPoint(&p)
{}
	// Modifs ////////////////////////////////////////////////////////
		// Rotate ////////////////////////////////////////////////////
Points::Modifs::Rotate::Rotate(const Angle& angle) :
	angle(angle)
{}
void Points::Modifs::Rotate::Set(Points::Data& pData)
{
	pData.aPoint->Rotate(angle);
}
		// Taper /////////////////////////////////////////////////////
Points::Modifs::Taper::Taper(Val taper, Os osMain) :
	taper(taper), osMain(osMain)
{}
void Points::Modifs::Taper::Set(Points::Data& pData)
{
	pData.aPoint->Taper(taper, pData.sz[osMain], osMain);
}
		// Spline ////////////////////////////////////////////////////
Points::Modifs::Spline::Spline(const vector<Pos>& aKey, Os osMain) :
	aKey(aKey), osMain(osMain)
{}
void Points::Modifs::Spline::Set(Points::Data& pData)
{
	pData.aPoint->Spline(aKey, osMain);
}
		// ~Spline ///////////////////////////////////////////////////
void Points::Modifs::Set(Points::Data& pData)
{
	for (Modif* m : *this)
		m->Set(pData);
}
	// ~Modifs ///////////////////////////////////////////////////////
void Points::MakeQuad(Val szX, Val szY)
{	*(Base*) this =	{	{0,		0,		0},
						{szX,	0,		0},
						{0,		szY,	0},
						{szX,	szY,	0}
					};
}
void Points::MakeCube(Val s)
{   *(Base*) this = {   {-s,-s,-s},  { s,-s,-s},  {-s, s,-s},  {s,s,-s},     // Низ.
                        {-s,-s, s},  { s,-s, s},  {-s, s, s},  {s,s, s}   }; // Верх.
}
void Points::MakePlane(Val szX, Val szY, ValN sgmX, ValN sgmY)
{	// Вычисление размера и выделение памяти.
	Ind nVertX = sgmX + 1, nVertY = sgmY + 1;
	resize(nVertX * nVertY);
	Pos* pVert = data();
	Val	vStepX = szX / sgmX, vPosX,
		vStepY = szY / sgmY, vPosY = 0;
	// Вершины.
	for (Ind y = 0; y < nVertY; ++y, vPosY += vStepY)
	{	vPosX = 0;
		for (Ind x = 0; x < nVertX; ++x, vPosX += vStepX, ++pVert)
		{	pVert->x = vPosX;
			pVert->y = vPosY;
			pVert->z = 0;
		}
	}
}
void Points::MakeCylinder(Val d, Val len, ValN sgmC, ValN sgmL, bool bCloseB, bool bCloseE)
{	// Вычисление размера и выделение памяти.
	const ValN nVertC = sgmC * (sgmL + 1);
	resize(nVertC + bCloseB + bCloseE);
	Pos *pVert = data();
	const Pos *pEndC = pVert + sgmC,
			  *pEnd  = pVert + nVertC;
	// Цикл создания, по сегментам окружности.
	const Val rad = d * 0.5;
	const Val angleStep = 2 * pi / sgmC,	// Шаг угла.
			  lStep = len / sgmL;			// Шаг длины.
	Val angle = 0;
	for (; pVert < pEndC; ++pVert, angle += angleStep)
	{	// Позиция внизу (на остальных уровнях будет аналогично).
		pVert->y = cos(angle) * rad;
		pVert->z = sin(angle) * rad;
		pVert->x = 0;
		// Устанавливаем данные по вертикали (на уровнях выше).
		Val l = lStep;
		for (Pos* pVertH = pVert + sgmC; pVertH < pEnd; pVertH += sgmC, l += lStep)
		{	pVertH->y = pVert->y;
			pVertH->z = pVert->z;
			pVertH->x = l;
		}
	}
	// Создание дна.
	if (bCloseB)
	{	pVert = data() + (size() - (bCloseE? 2: 1));
		pVert->x = 0; pVert->y = 0; pVert->z = 0;
	}
	// Создание крышки.
	if (bCloseE)
	{	pVert = data() + (size() - 1);
		pVert->x = len; pVert->y = 0; pVert->z = 0;
	}
}
void Points::Move(const Vec3& v)
{	for (Pos& point: *this)
		point += v;
}
void Points::Rotate(const Angle& angle)
{
	Mat4 mat(1);
	mat = glm::rotate(mat, angle.z, Vec3(0, 0, 1));
	mat = glm::rotate(mat, angle.y, Vec3(0, 1, 0));
	mat = glm::rotate(mat, angle.x, Vec3(1, 0, 0));

	for (Pos& point: *this)
		point = mat * Vec4(point, 1.0);
}
void Points::Taper(Val t, Val length, Os osMain)
{
	Val dist = 1 - t; // Общая дистанция (на сколько всего уменьшаем).
	switch (osMain)
	{
		case osX:
			for (Pos& point: *this)
			{
				Val m = point.x / length; // Пройденная дистанция (коэф.).
				Val sz = 1 - m * dist; // Коэф. размера.
				point.y *= sz;
				point.z *= sz;
			}
			break;
		case osY:
			for (Pos& point: *this)
			{
				Val m = point.y / length; // Пройденная дистанция (коэф.).
				Val sz = 1 - m * dist; // Коэф. размера.
				point.z *= sz;
				point.x *= sz;
			}
			break;
		case osZ:
			for (Pos& point: *this)
			{
				Val m = point.z / length; // Пройденная дистанция (коэф.).
				Val sz = 1 - m * dist; // Коэф. размера.
				point.x *= sz;
				point.y *= sz;
			}
			break;
	}
}
void Points::Spline(const vector<Pos>& aKey, Os osMain)
{
	SplineCalc splineCalc(aKey, osMain);
	if ( !splineCalc.Check() ) return;
	splineCalc.Calc();
	for (Pos& p : *this)
		splineCalc.CalcPos(p);
}
// Mesh /////////////////////////////////////////////////////////////
void Mesh::MakeQuad(Val szX, Val szY)
{
	aVert.MakeQuad(szX, szY);
	aInd = {0, 1, 2, 1, 3, 2};
}
void Mesh::MakeCubeIn(Val s)
{
    aVert.MakeCube(s);
    aInd =  {   0,1,3, 0,3,2,  4,7,5, 4,6,7,    // Низ, верх.
                2,3,6, 3,7,6,  0,4,1, 1,4,5,    // Вперёд, назад
                0,2,4, 2,6,4,  1,7,3, 1,5,7,    // Лево, право.
            };
}
void Mesh::MakePlane(Val szX, Val szY, ValN sgmX, ValN sgmY)
{
	aVert.MakePlane(szX, szY, sgmX, sgmY);
	// Расстановка индексов.
	aInd.resize(sgmX * sgmY * 6);
	Ind* pInd = aInd.data();
	const Ind nVertWidth = sgmX + 1;
	for (Ind uVert = 0, nVertEnd = nVertWidth * sgmY; uVert < nVertEnd; ++uVert)
	{	for (Ind nVertLastX = uVert + sgmX; uVert < nVertLastX; ++uVert, ++pInd)
		{	Ind uVertNextLine = uVert + nVertWidth;
			*pInd = uVert;
			*(++pInd) = uVertNextLine + 1;
			*(++pInd) = uVertNextLine;
			*(++pInd) = uVert;
			*(++pInd) = uVert + 1;
			*(++pInd) = uVertNextLine + 1;
		}
	}
}
void Mesh::MakeCylinder(Val d, Val len, ValN sgmC, ValN sgmL, bool bCloseB, bool bCloseE)
{	// Создание вершин.
	aVert.MakeCylinder(d, len, sgmC, sgmL, bCloseB, bCloseE);
	// Вычисление размера и выделение памяти для индексов.
	aInd.resize(sgmC * sgmL * 6 + ((ValN)bCloseB + bCloseE) * sgmC * 3);
	Ind* pInd = aInd.data();
	for (ValN i = 0; i < sgmL; ++i)
	{	const Ind len  = sgmC * i,		// Текущая длина.
				  lenN = len + sgmC;	// Следующая длина.
		for (ValN s = 0; s < sgmC; ++s)
		{	const Ind n = (s + 1 == sgmC)? 0: s + 1; // Следующий сегмент.
			*(pInd++) = len	+ s;
			*(pInd++) = len	+ n;
			*(pInd++) = lenN	+ s;
			*(pInd++) = lenN	+ s;
			*(pInd++) = len	+ n;
			*(pInd++) = lenN	+ n;
		}
	}
	// Создание дна.
	if (bCloseB)
	{	const Ind iV = aVert.size() - (bCloseE? 2: 1);
		for (Ind x = 0; x < sgmC; ++x)
		{	*(pInd++) = iV;
			*(pInd++) = (x + 1 == sgmC)? 0: x + 1; // Следующий по x.
			*(pInd++) = x;
		}
	}
	// Создание крышки.
	if (bCloseE)
	{	const Ind iV = aVert.size() - 1, s = iV - bCloseB - sgmC, e = s + sgmC;
		for (Ind x = s; x < e; ++x)
		{	*(pInd++) = iV;
			*(pInd++) = x;
			*(pInd++) = (x + 1 == e)? s: x + 1; // Следующий по x.
		}
	}
}
// PlaceTex /////////////////////////////////////////////////////////
void PlaceTex::SetQuad()
{	*(Base*) this =	{	{0, 0},
						{1, 0},
						{0, 1},
						{1, 1}
					};
}
void PlaceTex::SetPlane(ValN sgmX, ValN sgmY, Val u, Val v)
{	const ValN	nVertX = sgmX + 1,
				nVertY = sgmY + 1;
	resize(nVertX * nVertY);
	PosTex* pPos = data();
	const Val	uStep = u / sgmX,
				vStep = v / sgmY;
	Val uPos, vPos = 0;
	// Вершины.
	for (ValN y = 0; y < nVertY; ++y, vPos += vStep)
	{	uPos = 0;
		for (ValN x = 0; x < nVertX; ++x, uPos += uStep, ++pPos)
		{	pPos->u = uPos;
			pPos->v = vPos;
		}
	}
}
void PlaceTex::SetCylinder(ValN sgmC, ValN sgmL, Val u, Val v)
{	assert(sgmC % 2 == 0);
	resize(sgmC * (sgmL + 1));
	PosTex* pPos = data();
	const PosTex *pEndC = pPos + sgmC,
				 *pEndH = pPos + sgmC / 2,
				 *pEnd  = pPos + size();
	// Цикл по сегментам окружности.
	const Val	uStep = u / sgmC, // Шаг u.
				vStep = v / sgmL; // Шаг v.
	u = 0;
	for (; pPos < pEndC; ++pPos)
	{	// Позиция внизу (на остальных уровнях будет аналогично).
		pPos->u = u;
		pPos->v = 0;
		// Устанавливаем данные по вертикали (на уровнях выше).
		v = vStep;
		for (PosTex* pPosH = pPos + sgmC; pPosH < pEnd; pPosH += sgmC, v += vStep)
		{	pPosH->u = u;
			pPosH->v = v;
		}
		// После середины зеркалим текстуру, чтобы к последней вершине u была 0 (первая и последняя это одна и та же вершина).
		u = pPos < pEndH ? u + uStep : u - uStep;
	}
}
}
