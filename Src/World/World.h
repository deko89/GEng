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
    Models models; ///< Модели.
    unique_ptr<SkyBox> sky;    ///< Небо.
};

}

#endif
