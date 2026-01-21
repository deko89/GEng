#ifndef World_H
#define World_H

#include <memory>
#include "Models.h"
#include "SkyBox.h"

namespace GEng
{

using std::unique_ptr;
using std::make_unique;

/// Мир.
class World
{	public:
    unique_ptr<SkyBox> sky;     ///< Небо.
    Models models;              ///< Модели.
};

}

#endif
