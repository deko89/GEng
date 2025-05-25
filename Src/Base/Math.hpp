#ifndef EnGreen_Math_H
#define EnGreen_Math_H

#include <vector>
#include "GEng/Base/Meta/Meta.h"

namespace glm
{
	/// Вращение 2d вектора на 90°.
	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER void Rotate90(vec<2, T, Q>& v)
	{
		T x = v.x;
		v.x = -v.y;
		v.y = x;
	}
}

namespace GEng
{

using std::vector;

/// Калькулятор сплайна (в 3d).
struct SplineCalc
{
	const bool bPrint = 0;	///< Печать отладочной информации.
	/** @brief Конструктор.
		param[in] aKey - ключевые точки сплайна. Каждая следующая должна быть больше по X.
		param[in] osMain - основная ось, вдоль которой идёт цилиндр для сплайна. */
	SplineCalc(const vector<Pos>& aKey, Os osMain = osX);
	bool Check() const;		///< Проверить (правильные ли параметры на входе).
	void Calc();			///< Рассчитать необходимые общие параметры.
	Val GetLen();			///< Получить длину (по текущей оси X).
	/** @brief Получить позицию.
		@details Общая идея. Вдоль оси (X) есть цилиндр для сплайна.
1. Сперва рассчитываются позиция центральной линии сплайна. По двум плоскостям.
2. Затем смещения вершин цилиндра. Сегмент цилиндра (кольцо) поворачивается под
угол направления (согласно производным в этой точке).
Размеры сегмента не меняются, он только вращается.
		param[in,out] vert - начальная позиция точки. В неё же запишется конечная позиция. */
	void CalcPos(Pos& vert);
private:
	const vector<Pos>& aKey;	///< Ключевые точки.
	/// Условные оси X, Y - относительно которых строится сплайн.
	/// Функции всегда думают что работают с осям XY.
	/// По факту их можно менять местами XYZ, YZX, ZXY. При этом совершенно ничего не меняется, всё остаётся также как будто XYZ.
	/// Можно просто думать что всегда "XYZ" = XYZ, всё так же.
	Os oX = osX, oY = osY, oZ = osZ;
	bool bXZ = 0;				///< 0 - установлена плоскость XY, 1 - XZ. См. SetPlaneXY.
	vector<Val> aDerY, aDerZ;	///< Производные в ключевых точках (в условных плоскостях "XY", "XZ").
	size_t iKey = 0;			///< Текущая ключевая точка начала отрезка.
	/// Переменные в текущем отрезке (для формулы сплайна).
	Val x0, lineLen, k0, k1, d0, d1;
	Pos vCenter{vNaN, vNaN, vNaN};	///< Последний рассчитаный центр. Если x равны то пересчитывать не надо.
	Vec3 vOy, vOz;	///< Оси производной у последней вершины.
	/// Дополнительная смена осей при переключении плоскостей. Чтобы функции работали только с XY.
	/// Сделать условную плоскость "XY" / "XZ" текущей.
	/// Основная ось "X" не меняется (фактически она может быть Y, Z).
	void SetPlaneXY();
	void SetPlaneXZ();
	void CalcDer();			///< Рассчитать производные (в текущей плоскости "XY").
	bool SelectLine(Val x);	///< Установить подходящий отрезок для значения x (и все переменные). Вызывает также GetKD.
	void GetKD();			///< Получить только переменные для другой плоскости (k0, k1, d0, d1) текущего отрезка.
};

}

#endif



#ifdef M_IncludeCpp

#include <cmath>

namespace GEng
{

// SplineCalc ////////////////////////////////////////////////////////
SplineCalc::SplineCalc(const vector<Pos>& aKey, Os osMain) :
	aKey(aKey)
{
	aDerY.resize( aKey.size() );
	aDerZ.resize( aKey.size() );
	// Установка осей.
	oX = osMain;
	SetPlaneXY();
}
bool SplineCalc::Check() const
{
	return aKey.size() > 1;
}
void SplineCalc::Calc()
{
	// Расчёт производных в ключевых точках.
		// Плоскость "XY".
	SetPlaneXY();
	CalcDer();
		// Плоскость "XZ".
	SetPlaneXZ();
	CalcDer();
	SetPlaneXY();
}
Val SplineCalc::GetLen()
{
	return aKey.back()[oX];
}
void SplineCalc::CalcPos(Pos& vert)
{
	if ( vCenter[oX] != vert[oX] )
	{
		vCenter[oX] = vert[oX];
		if (bPrint) std::cout << "vert (begin) = " << vert << std::endl;
		// 1. Расчёт центра (и заодно производных).
		// 1.1. По оси Y (при "XYZ" = ZXY это X).
		assert(bXZ == 0);
		SelectLine(vert[oX]); // Полный поиск отрезка.
		const Val w = (vert[oX] - x0) / lineLen,  w2 = w * w,  w3 = w2 * w;
		const Val	r1 = 2*w3 - 3*w2 + 1,
					r2 = -2*w3 + 3*w2,
					r3 = w3 - 2*w2 + w,
					r4 = w3 - w2;
		const Val	r5 = 6*w2 - 6*w,
					r6 = -6*w2 + 6*w,
					r7 = 3*w2 - 4*w + 1,
					r8 = 3*w2 - 2*w;
		vCenter[oY] = k0 * r1 + k1 * r2 + d0 * r3 + d1 * r4; // Центр цилиндра.
		Val dy		= k0 * r5 + k1 * r6 + d0 * r7 + d1 * r8; // Производная (за lineLen).
		dy /= lineLen; // Приводим к обычной производной за 1.
		// 1.2. По оси Z (Y при ZXY).
		SetPlaneXZ(); // (При ZXY - это плоскость ZY.)
		GetKD(); // Отрезок тот-же, просто берём переменные.
		SetPlaneXY();
		vCenter[oZ] = k0 * r1 + k1 * r2 + d0 * r3 + d1 * r4; // Центр цилиндра.
		Val dz		= k0 * r5 + k1 * r6 + d0 * r7 + d1 * r8;
		dz /= lineLen;
		if (bPrint) std::cout << "vCenter = " << vCenter << std::endl;
		// 2. Смещение на позицию вершины цилиндра (согласно направлению сплайна).
		// 2.1. Нахождение оси X`.
		Vec3 vOx;  vOx[oX] = 1;  vOx[oY] = dy;  vOx[oZ] = dz;
		vOx = glm::normalize(vOx);
		// 2.2. Нахождение оси Y`.
		Vec2 vDxy(1, dy);
		vDxy = glm::normalize(vDxy);
		glm::Rotate90(vDxy);
		vOy[oX] = vDxy.x;  vOy[oY] = vDxy.y;  vOy[oZ] = 0;
		// 2.3. Нахождение оси Z`.
		vOz = glm::cross(vOx, vOy);
		if (bPrint) std::cout << "vOx = " << vOx << "; vOy = " << vOy << "; vOz = " << vOz << std::endl;
	}
	// 2.4. Получение искомого вектора.
	Vec3 vXy(vOy * vert[oY]); // Вектор смещения по оси Y`.
	Vec3 vXz(vOz * vert[oZ]); // Вектор смещения по оси Z`.
	if (bPrint) std::cout << "vXy = " << vXy << "; vXz = " << vXz << std::endl;
	vert = vCenter + vXy + vXz;
	if (bPrint) std::cout << "vert (end) = " << vert << std::endl;
}
void SplineCalc::SetPlaneXY()
{
	bXZ = 0;
	switch (oX)
	{	case osX:	oY = osY;	oZ = osZ;	break;
		case osY:	oY = osZ;	oZ = osX;	break;
		case osZ:	oY = osX;	oZ = osY;	break;
	}
}
void SplineCalc::SetPlaneXZ()
{
	bXZ = 1;
	switch (oX)
	{
		case osX:	oY = osZ;	oZ = osY;	break;
		case osY:	oY = osX;	oZ = osZ;	break;
		case osZ:	oY = osY;	oZ = osX;	break;
	}
}
void SplineCalc::CalcDer()
{
	vector<Val>& aDer = bXZ? aDerZ: aDerY;
	if (bPrint) std::cout << "CalcDer oX = " << oX << ", oY = " << oY << ", oZ = " << oZ << std::endl;
	// Производная №0 (самая первая).
	const Vec2 k0( aKey[0][oX],	aKey[0][oY]	);
	const Vec2 k1( aKey[1][oX],	aKey[1][oY]	);
	aDer[0] = (k1.y - k0.y) / (k1.x - k0.x);
	if (bPrint) std::cout << "Der 0 = " << aDer[0] << std::endl;
	// Производная №1+ (стандартные).
	for (size_t i = 1; i < aKey.size() - 1; ++i)
	{
		// Ближайшие ключевые точки.
		const Vec2 k0( aKey[i - 1][oX],	aKey[i - 1][oY]	);
		const Vec2 k1( aKey[i    ][oX],	aKey[i    ][oY]	);
		const Vec2 k2( aKey[i + 1][oX],	aKey[i + 1][oY]	);
		// Прилежащие вектора.
		Vec2 v1 = k1 - k0; // До.
		Vec2 v2 = k2 - k1; // После.
		if (bPrint)
		{	std::cout << "Calc Der " << i << std::endl;
			std::cout << "\tk0 " << k0 << "; k1 " << k1 << "; k2 " << k2
					  << "\n\tv1 " << v1 << "; v2 " << v2 << std::endl;
		}
		// Нормализация. Нужна для нахождения среднего по углу.
		v1 = glm::normalize(v1);
		v2 = glm::normalize(v2);
		// Нахождение среднего вектора.
		Vec2 v = v1 + v2;
		if (bPrint)
			std::cout << "\tnormalize v1: " << v1 << "; normalize v2: " << v2
					  << "\n\tv: " << v << std::endl;
		// Средняя производная (между ней и ключевыми отрезками равные углы).
		aDer[i] = v.y / v.x;
		if (bPrint)
			std::cout << "Der " << i << " = " << aDer[i] << std::endl;
	}
	// Производная №N (самая последняя).
	const size_t p = aKey.size() - 2;
	const size_t l = p + 1;
	const Vec2 kp( aKey[p][oX],	aKey[p][oY]	);
	const Vec2 kl( aKey[l][oX],	aKey[l][oY]	);
	aDer[l] = (kl.y - kp.y) / (kl.x - kp.x);
	if (bPrint) std::cout << "Der " << l << " = " << aDer[l] << std::endl;
}
bool SplineCalc::SelectLine(Val x)
{
	// Поиск начальной ключевой точки (отрезка сплайна).
	for (iKey = 1; iKey < aKey.size(); ++iKey)
		if ( x <= aKey[iKey][oX] )
			break;

	if ( iKey >= aKey.size() )
	{
		if (bPrint)
			std::cout << "Не найден отрезок: x = " << x
					  << " X_back = " << aKey.back()[oX] << std::endl;
		iKey = aKey.size() - 2;
		//return false;
	} else
		--iKey;

	// Переменные отрезка.
	x0      = aKey[iKey][oX];
	Val x1  = aKey[iKey + 1][oX];
	lineLen = x1 - x0;

	GetKD();

	return true;
}
void SplineCalc::GetKD()
{
	k0 = aKey[iKey][oY];
	k1 = aKey[iKey + 1][oY];
	vector<Val>& aDer = bXZ? aDerZ: aDerY;
	d0 = aDer[iKey] * lineLen;
	d1 = aDer[iKey + 1] * lineLen;
}

}

#endif
