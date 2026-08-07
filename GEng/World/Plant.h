#ifndef Plant_H
#define Plant_H

#include <random>
#include "Models.h"

namespace GEng
{

/// Модель ствола (или ветки).
class ModelTrunk : public ModelCylinder
{	public:
	static ValN	nSgmLen,	///< Число сегментов на 1 м длины.
				nSgmD;		///< Число сегментов на 1 м диаметра.
	Val rD = 0.04;			///< Коэффициент диаметра на 1 м длины.
	const uint8_t logLvl = 1;	///< Печать отладочной информации.
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
	uint32_t seed = 0;
	const uint8_t logLvl = 1;	///< Печать отладочной информации.
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
		@param[in] angleZ - угол по оси Z. Диапазон: 0 <= a < 2*pi.
		@param[in] angleU - угол верх / вниз. Диапазон: -pi/2 < a < pi/2.
		@param[in] bendAmplitude - максимальная дистанция отклонения изгибов.
		@param[in] bendRate - изогнутость, число изгибов на 1 метр.
		*/
	void CreateBranch(Pos pos, Val len, Val angleZ, Val angleU,
		Val bendAmplitude, Val bendRate,
		std::mt19937& gen);
};

}

#endif
