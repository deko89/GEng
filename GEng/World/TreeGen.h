#ifndef TreeGen_H
#define TreeGen_H

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
};

TreeGen& GetTreeGen();

}

#endif
