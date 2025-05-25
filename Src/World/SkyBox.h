#ifndef SKYBOX_H
#define SKYBOX_H

#include "Base/Meta/Texture.h"

namespace GEng
{

/// Задний план (небо).
class SkyBox
{   public:
    /** Создать.
        param[in] path - шаблон пути до текстур. Формат: "Path/Name%zu.Ext".
    */
    SkyBox(const Str& path);
    void Draw() const;
private:
    ModelVi model;  ///< Модель.
    Texture tex;    ///< Текстура.
};

}

#endif
