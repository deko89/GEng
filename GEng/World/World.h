#ifndef World_H
#define World_H

#include <memory>
#include "Models.h"
#include "SkyBox.h"

namespace GEng
{

using std::unique_ptr;
using std::make_unique;

/// Выделение.
class Selection
{	public:
    vector<Model*> aMod;    ///< Выделенные модели.
	/// Трансформация выделения.
    Pos pos;
	Angle angle;
	Scale scale;
};

/// Мир.
class World
{	public:
    unique_ptr<SkyBox> sky;     ///< Небо.
    Models models;              ///< Модели.
    Selection sel;              ///< Выделенные модели.
    std::filesystem::path path; ///< Путь до файла.
    void Save();
    void Load();
    void Delete();              ///< Удалить выделенные модели.
};

}

#endif
