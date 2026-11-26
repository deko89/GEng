#ifndef TreeGen_H
#define TreeGen_H

#include <random>
#include "GEng/GEng.h"
#include "GEng/World/Plant.h"

using namespace GEng;

namespace GEng
{

class TreeGen
{	public:
	static Menu* CreateMenu();
	void Load();
    void Load(const std::filesystem::path& path);
    void Save();
    void SaveAs();
	void Generate(); ///< Создать дерево / куст по параметрам.
protected:
    std::filesystem::path path; ///< Путь до растения.
    ModelTree* tree = nullptr;
	Texture texBark; //tmp
    std::mt19937 gen;
	///	Создать ветку.
	///	@param[in] pos - позиция начала.
	///	@param[in] len - длина.
	///	@param[in] angleZ - угол по оси Z. Диапазон: 0 <= a < 2*pi.
	///	@param[in] angleU - угол верх / вниз. Диапазон: -pi/2 <= a <= pi/2.
	///	@param[in] bendAmplitude - максимальная дистанция отклонения изгибов, доля от длины.
	///	@param[in] bendRate - изогнутость, число ключевых точек изгиба на 1 метр.
    void MakeBranch(Pos pos, Val len, Val angleZ, Val angleU, Val bendAmplitude, Val bendRate);
};

TreeGen& GetTreeGen();

}

#endif
