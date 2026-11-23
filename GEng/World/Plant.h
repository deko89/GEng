#ifndef Plant_H
#define Plant_H

#include "Models.h"

namespace GEng
{

/// Модель ствола (или ветки).
class ModelTrunk : public ModelCylinder
{	public:
	static ValN	nSgmLen,	///< Число сегментов на 1 м длины.
				nSgmD;		///< Число сегментов на 1 м диаметра.
	Val rD = 0.04;			///< Коэффициент диаметра на 1 м длины.
	/** Конструктор.
		param[in] aKey - ключевые точки (через которые проходит). */
	ModelTrunk(const vector<Pos>& aKey);
	void Update(); ///< Обновить модель (применить новые данные).
	SplineCalc& GetSpline();
};

class ModelTree : public Model
{	public:
	Val h = 10;		///< Высота.
	Val w = 0.2;	///< Ширина.
	uint32_t seed = 0;
	bool IsGroup() const override {return 1;}
	Models& GetModels() override {return models;}
protected:
	Models models;
	static Texture texBark;
};

}

#endif
