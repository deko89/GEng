#ifndef Plant_H
#define Plant_H

#include "Models.h"

namespace GEng
{

/// Модель ствола (или ветки).
class ModelStvol : public ModelCylinder
{	public:
	static ValN	nSgmLen,	///< Число сегментов на 1 м длины.
				nSgmD;		///< Число сегментов на 1 м диаметра.
	Val rD = 0.04;			///< Коэффициент диаметра на 1 м длины.
	/** Конструктор.
		param[in] aKey - ключевые точки (через которые проходит). */
	ModelStvol(const vector<Pos>& aKey, Os os = osX);
	void Update(); ///< Обновить модель (применить новые данные).
	vector<Pos>& GetKey(); ///< Получить ключевые точки (через которые проходит).
};

class ModelTree : public Model
{	public:
	Val h = 10;		///< Высота.
	Val w = 0.2;	///< Ширина.
	ModelTree();
	bool IsGroup() const override {return 1;}
	const Models& GetModels() const override {return models;}
	void Update() override; ///< Обновить модель (применить параметры).
protected:
	Models models;
	static Texture texBark;
};

}

#endif
