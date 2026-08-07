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
	const bool bPrint = 1;	///< Печать отладочной информации.
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
3. Длина цилиндра и сплайна (GetLen) должны совпадать.
		@note Для эффективности важно подряд передавать одинаковые vert.x.
		@param[in,out] vert - начальная позиция цилиндра.
			В неё же запишется конечная позиция сплайна. */
	void CalcPos(Pos& vert);
private:
	vector<Pos> aKey;	///< Ключевые точки.
	vector<Val> aLen;	///< Длины отрезков (между ключевыми точками).
	Val len = 0;		///< Длина.
	/// Условные оси X, Y - относительно которых строится сплайн.
	/// Функции всегда думают что работают с осям XY.
	/// По факту их можно менять местами XYZ, YZX, ZXY. При этом совершенно ничего не меняется, всё остаётся также как будто XYZ.
	/// Можно просто думать что всегда "XYZ" = XYZ, всё так же.
	Os oX = osX, oY = osY, oZ = osZ;
	bool bXZ = 0;				///< 0 - установлена плоскость XY, 1 - XZ. См. SetPlaneXY.
	/// Производные в ключевых точках (в условных плоскостях "XY", "XZ").
	/// Y компонента единичного вектора указывающего направление производной.
	vector<Val> aDerY, aDerZ;
	size_t iKey = 0;			///< Текущая ключевая точка начала отрезка.
	/// Переменные в текущем отрезке (для формулы сплайна).
	Val w, lineLen, k0, k1, d0, d1;
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
	aLen.resize(  aKey.size() - 1 );
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
	// Расчёт длины.
	len = 0;
	for (size_t k = 0; k < aLen.size(); ++k)
	{
		aLen[k] = glm::distance(aKey[k], aKey[k + 1]);
		len += aLen[k];
	}
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
	return len;
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
		const Val w2 = w * w,  w3 = w2 * w;
		const Val	r1 = 2*w3 - 3*w2 + 1,
					r2 = -2*w3 + 3*w2,
					r3 = w3 - 2*w2 + w,
					r4 = w3 - w2;
		const Val	r5 = 6*w2 - 6*w,
					r6 = -6*w2 + 6*w,
					r7 = 3*w2 - 4*w + 1,
					r8 = 3*w2 - 2*w;
		if (bPrint)
			std::cout	<< " k0: " << k0
				<< " k1: " << k1
				<< " d0: " << d0
				<< " d1: " << d1
				<< std::endl;
		vCenter[oY] = k0 * r1 + k1 * r2 + d0 * r3 + d1 * r4; // Центр цилиндра.
		Val dy		= k0 * r5 + k1 * r6 + d0 * r7 + d1 * r8; // Производная (за lineLen).
		// 1.2. По оси Z (Y при ZXY).
		SetPlaneXZ(); // (При ZXY - это плоскость ZY.)
		GetKD(); // Отрезок тот-же, просто берём переменные.
		SetPlaneXY();
		vCenter[oZ] = k0 * r1 + k1 * r2 + d0 * r3 + d1 * r4; // Центр цилиндра.
		Val dz		= k0 * r5 + k1 * r6 + d0 * r7 + d1 * r8;
		if (bPrint) std::cout << "vCenter = " << vCenter << std::endl;
		// 2. Смещение на позицию вершины цилиндра (согласно направлению сплайна).
		// 2.1. Нахождение оси X`.
		const Val x0 = aKey[iKey    ][oX];
		const Val x1 = aKey[iKey + 1][oX];
		const Val dx = x1 - x0;
		Vec3 vOx;  vOx[oX] = dx;  vOx[oY] = dy;  vOx[oZ] = dz;
		vOx = glm::normalize(vOx);
		// 2.2. Нахождение оси Y`.
		Vec2 vDxy(dx, dy);
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
	aDer[0] = glm::normalize(k1 - k0).y;
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
		aDer[i] = glm::normalize(v).y;
		if (bPrint)
			std::cout << "Der " << i << " = " << aDer[i] << std::endl;
	}
	// Производная №N (самая последняя).
	const size_t p = aKey.size() - 2;
	const size_t l = p + 1;
	const Vec2 kp( aKey[p][oX],	aKey[p][oY]	);
	const Vec2 kl( aKey[l][oX],	aKey[l][oY]	);
	aDer[l] = glm::normalize(kl - kp).y;
	if (bPrint) std::cout << "Der " << l << " = " << aDer[l] << std::endl;
}
bool SplineCalc::SelectLine(Val x)
{
	// Поиск начальной ключевой точки (отрезка сплайна).
	const Val dist = x * GetLen();
	Val distKey = 0;
	for (iKey = 0; iKey < aLen.size(); ++iKey)
	{
		distKey += aLen[iKey];
		if (dist <= distKey)
			break;
	}

	if ( iKey >= aLen.size() )
	{
		if (bPrint)
			std::cerr << "Не найден отрезок: dist = " << dist
					  << " len = " << GetLen() << std::endl;
		iKey = aKey.size() - 2;
		//return false;
	}

	// Переменные отрезка.
	lineLen = aLen[iKey];
	distKey -= lineLen;
	w = (dist - distKey) / lineLen;
	if (bPrint)
	{
		std::cout	<< " x: " << x
			<< " iKey: " << iKey
			<< " distKey: " << distKey
			<< " lineLen: " << lineLen
			<< " w: " << w
			<< std::endl;
		if (w < 0.0 || w > 1.0)
		{
			std::cerr << "w != 0..1, w = " << w << std::endl;
			assert(0);
		}
	}

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
