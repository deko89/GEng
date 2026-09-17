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

	/// Поворот 2d вектора v на угол между единичными векторами a и b.
	/// (угол поворота = угол b - угол a)
	template<typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<2, T, Q> RotateAVN(
		vec<2, T, Q> v,
		vec<2, T, Q> a,
		vec<2, T, Q> b)
	{
		assert(glm::abs(glm::length(a) - T(1)) < T(GEng::epsBase));
		assert(glm::abs(glm::length(b) - T(1)) < T(GEng::epsBase));
		// Проекция v на a.
		const T x = a.x * v.x + a.y * v.y;
		const T y = a.x * v.y - a.y * v.x;
		// Проекция v на b.
		const vec<2, T, Q> vbx = b * x;
		Rotate90(b);
		const vec<2, T, Q> vby = b * y;
		return vbx + vby;
	}

	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, T, Q> Normalize(vec<L, T, Q> const v)
	{
		if ( GEng::IsApproxZero(v) )
			return vec<L, T, Q>(0);
		return glm::normalize(v);
	}
}

namespace GEng
{

using std::vector;

/**	@brief Длина ломаной.
	@param[in] aPos - массив позиций.
	@return Длина ломаной. */
Val LengthPolyline(const vector<Pos>& aPos);

/// Ключевая точка сплайна (в 3d).
struct KeySpline
{
	Pos pos;	///< Позиция.
	Vec3 t;		///< tangent / вперёд / ось x. Производная нормализованная.
	Vec3 b;		///< binormal / вправо / ось y.
	Val len;	///< Длина отрезка сплайна (после этой точки до следующей).
};

inline std::ostream& operator<<(std::ostream& os, const KeySpline& k)
{
	return os << "pos " << k.pos << "\td " << k.t;
}

/// Коэффициенты для уравнения отрезка сплайна (в 2d).
struct CoefficientSpline
{
	Val k0, k1;	///< Позиция в начале и конце.
	Val d0, d1;	///< Производные в начале и конце.
};

inline std::ostream& operator<<(std::ostream& os, const CoefficientSpline& c)
{
	return os	<< "k0="	<< c.k0 << "\tk1=" << c.k1
				<< "\td0="	<< c.d0 << "\td1=" << c.d1;
}

/// Калькулятор сплайна (в 3d).
struct SplineCalc
{
	const bool bPrint = 1;	///< Печать отладочной информации.
	/** @brief Конструктор.
		param[in] aKey - ключевые точки сплайна.
		param[in] osMain - основная ось, вдоль которой идёт цилиндр для сплайна. */
	SplineCalc(const vector<Pos>& aKey, Os osMain = osX);
	bool Check() const;		///< Проверить (правильные ли параметры на входе).
	void Calc();			///< Рассчитать необходимые общие параметры.
	Val GetLen();			///< Длина - сумма расстояний между ключевыми.
	/** @brief Получить позицию.
		@details Общая идея. Вдоль оси (X) есть цилиндр для сплайна.
1. Сперва рассчитываются позиция центральной линии сплайна.
2. Затем смещения вершин цилиндра. Сегмент цилиндра (кольцо) поворачивается под
угол направления (согласно производным в этой точке).
Размеры сегмента не меняются, он только вращается.
3. Длина цилиндра и сплайна (GetLen) должны совпадать.
		@note Для эффективности важно подряд передавать одинаковые vert.x.
		@param[in,out] vert - начальная позиция цилиндра (на оси X).
			В неё же запишется конечная позиция сплайна. */
	void CalcPos(Pos& vert);
private:
	vector<KeySpline> aKey;	///< Ключевые точки.
	Val len = 0;			///< Длина всего сплайна.
	size_t iKey = 0;		///< Текущая ключевая точка начала отрезка.
	Val x = vNaN;			///< Общая позиция сплайна 0..len.
	Val w;					///< Позиция в текущем отрезке 0..1.
	Pos vCenter;			///< Текущая позиция на сплайне.
	Vec3 vOy, vOz;			///< Оси ориентации в текущей позиции.
	void CalcDer();			///< Рассчитать производные.
	bool SelectLine();		///< Установить подходящий отрезок для значения x.
	/// Получить коэффициенты текущего отрезка для оси os.
	CoefficientSpline GetCoefficient(OsType os);
};

}

#endif



#ifdef M_IncludeCpp

#include <cmath>

namespace GEng
{

Val LengthPolyline(const vector<Pos>& aPos)
{
	if (aPos.size() < 2) return 0;
	Val len = 0;
	for (size_t i = 0; i < aPos.size() - 1; ++i)
		len += glm::distance(aPos[i], aPos[i + 1]);
	return len;
}
// SplineCalc ////////////////////////////////////////////////////////
SplineCalc::SplineCalc(const vector<Pos>& aKey, Os osMain)
{
	this->aKey.resize( aKey.size() );
	for (size_t k = 0; k < aKey.size(); ++k)
		this->aKey[k].pos = aKey[k];
}
bool SplineCalc::Check() const
{
	return aKey.size() > 1;
}
void SplineCalc::Calc()
{
	// Расчёт длины.
	len = 0;
	for (size_t k = 0; k < aKey.size() - 1; ++k)
	{
		aKey[k].len = glm::distance(aKey[k].pos, aKey[k + 1].pos);
		len += aKey[k].len;
	}
	// Расчёт производных в ключевых точках.
	CalcDer();
}
Val SplineCalc::GetLen()
{
	return len;
}
void SplineCalc::CalcPos(Pos& vert)
{
	if (bPrint) std::cout << "vert (begin) = " << vert << std::endl;
	if (x != vert.x)
	{
		x = vert.x;
		SelectLine(); // Полный поиск отрезка.
		// 1. Расчёт центра (и заодно производных).
		const Val w2 = w * w,  w3 = w2 * w;
		const Val	r1 = 2*w3 - 3*w2 + 1,
					r2 = -2*w3 + 3*w2,
					r3 = w3 - 2*w2 + w,
					r4 = w3 - w2;
		const Val	r5 = 6*w2 - 6*w,
					r6 = -6*w2 + 6*w,
					r7 = 3*w2 - 4*w + 1,
					r8 = 3*w2 - 2*w;
		Vec3 vOx;
		// 1.1. По осям X, Y, Z.
		for (OsType os = osX; os <= osZ; ++os)
		{
			CoefficientSpline c = GetCoefficient(os);
			if (bPrint) std::cout << "	" << c << std::endl;
			// Центр цилиндра.
			vCenter[os]	= c.k0 * r1 + c.k1 * r2 + c.d0 * r3 + c.d1 * r4;
			// Производная (за весь отрезок сплайна, пока по сути дельта).
			vOx[os]		= c.k0 * r5 + c.k1 * r6 + c.d0 * r7 + c.d1 * r8;
		}
		if (bPrint) std::cout << "	vCenter = " << vCenter << std::endl;
		// 2. Смещение на позицию вершины цилиндра (согласно направлению сплайна).
		// 2.1. Нахождение оси X`.
		vOx = Normalize(vOx);
		// 2.2. Нахождение оси Y`.
		Vec2 vDxy(vOx.x, vOx.y);
		if ( IsApproxZero(vDxy) )
		{
			vOy = {0,1,0};
		} else
		{
			vDxy = Normalize(vDxy);
			glm::Rotate90(vDxy);
			vOy.x = vDxy.x;  vOy.y = vDxy.y;  vOy.z = 0;
		}
		// 2.3. Нахождение оси Z`.
		vOz = Normalize(glm::cross(vOx, vOy));
		if (bPrint) std::cout << "	vOx = " << vOx << "; vOy = " << vOy << "; vOz = " << vOz << std::endl;
	}
	// 2.4. Получение искомого вектора.
	Vec3 vXy(vOy * vert.y); // Вектор смещения по оси Y`.
	Vec3 vXz(vOz * vert.z); // Вектор смещения по оси Z`.
	if (bPrint) std::cout << "	vXy = " << vXy << "; vXz = " << vXz << std::endl;
	vert = vCenter + vXy + vXz;
	if (bPrint) std::cout << "	vert (end) = " << vert << std::endl;
}
void SplineCalc::CalcDer()
{
	// Производная №0 (самая первая).
	aKey[0].t = Normalize(aKey[1].pos - aKey[0].pos);
	if (bPrint) std::cout << "Der 0 = " << aKey[0].t << std::endl;
	// Производная №1+ (стандартные).
	for (size_t i = 1; i < aKey.size() - 1; ++i)
	{
		// Ближайшие ключевые точки.
		const Vec3 k0(aKey[i - 1].pos);
		const Vec3 k1(aKey[i    ].pos);
		const Vec3 k2(aKey[i + 1].pos);
		// Прилежащие вектора.
		Vec3 v1 = k1 - k0; // До.
		Vec3 v2 = k2 - k1; // После.
		if (bPrint)
		{	std::cout << "	Calc Der " << i << std::endl;
			std::cout << "	k0 " << k0 << "; k1 " << k1 << "; k2 " << k2
					  << "\n	v1 " << v1 << "; v2 " << v2 << std::endl;
		}
		// Нормализация. Нужна для нахождения среднего по углу.
		v1 = Normalize(v1);
		v2 = Normalize(v2);
		// Нахождение среднего вектора.
		Vec3 v = v1 + v2;
		if (bPrint)
			std::cout << "	normalize v1: " << v1 << "; normalize v2: " << v2
					  << "\n	v: " << v << std::endl;
		// Средняя производная (между ней и ключевыми отрезками равные углы).
		aKey[i].t = Normalize(v);
		if (bPrint)
			std::cout << "Der " << i << " = " << aKey[i].t << std::endl;
	}
	// Производная №N (самая последняя).
	const size_t p = aKey.size() - 2;
	const size_t l = p + 1;
	aKey[l].t = Normalize(aKey[l].pos - aKey[p].pos);
	if (bPrint) std::cout << "Der " << l << " = " << aKey[l].t << std::endl;
}
bool SplineCalc::SelectLine()
{
	// Поиск начальной ключевой точки (отрезка сплайна).
	Val dist = 0;
	for (iKey = 0; iKey < aKey.size() - 1; ++iKey)
	{
		dist += aKey[iKey].len;
		if (x <= dist)
			break;
	}

	if (iKey >= aKey.size() - 1)
	{
		if (bPrint)
			std::cerr << "Не найден отрезок: x = " << x
					  << " len = " << GetLen() << std::endl;
		iKey = aKey.size() - 2;
		//return false;
	}

	// Переменные отрезка.
	const Val lineLen = aKey[iKey].len;
	dist -= lineLen;
	w = (x - dist) / lineLen;
	if (bPrint)
	{
		std::cout	<< "	x: " << x
			<< " iKey: " << iKey
			<< " beginKey: " << dist
			<< " lineLen: " << lineLen
			<< " w: " << w
			<< "\n	Key 1: " << aKey[iKey]
			<< "\n	Key 2: " << aKey[iKey + 1]
			<< std::endl;
		if (w < 0.0 || w > 1.0)
		{
			std::cerr << "w != 0..1, w = " << w << std::endl;
			assert(0); //db
		}
	}
	return true;
}
CoefficientSpline SplineCalc::GetCoefficient(OsType os)
{
	const Val lineLen = aKey[iKey].len;
	CoefficientSpline c;
	c.k0 = aKey[iKey].pos[os];
	c.k1 = aKey[iKey + 1].pos[os];
	c.d0 = aKey[iKey].t[os] * lineLen;
	c.d1 = aKey[iKey + 1].t[os] * lineLen;
	return c;
}

}

#endif
