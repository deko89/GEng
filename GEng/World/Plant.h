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
	ModelTrunk(const vector<Pos>& aKey, Os os = osX);
	void Update(); ///< Обновить модель (применить новые данные).
	vector<Pos>& GetKey(); ///< Получить ключевые точки (через которые проходит).
};

class ModelTree : public Model
{	public:
	Val h = 10;		///< Высота.
	Val w = h / 3;	///< Ширина.
	ModelTree();
	bool IsGroup() const override {return 1;}
	const Models& GetModels() const override {return models;}
	void Update() override; ///< Обновить модель (применить параметры).
protected:
	Models models;
	static Texture texBark;
	/** Создать ветку.
		@param[in] pos - позиция начала.
		@param[in] len - длина.
		@param[in] angleZ - угол по оси Z. От 0 до 2*pi.
		@param[in] angleU - угол верх / вниз. От -pi/4 до pi/4.
		@param[in] varSize - отклонение вариации относительно длины. От 0 до 0.3.
		@param[in] varStep - шаг вариации. От 0 до 1.
		*/
	void CreateBranch(Pos pos, Val len, Val angleZ, Val angleU,
		Val varSize = 0.1, Val varStep = 0.2);
};

}

#endif
