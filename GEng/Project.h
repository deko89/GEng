#ifndef Project_H
#define Project_H

#include <filesystem>
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
    void AddModel();
    void AddGroupLine();
protected:
    std::filesystem::path path; ///< Путь до файла.
};

}

#endif
