#ifndef Project_H
#define Project_H

#include "GEng/World/World.h"

namespace GEng
{

class Project
{	public:
    //MassivPtr<World> aWorld;
    World world;
    void Load();
    void Load(const std::filesystem::path& path);
    void Save();
    void SaveAs();
    void Clear();               ///< Очистка.
protected:
    std::filesystem::path path; ///< Путь до файла.
};

}

#endif
