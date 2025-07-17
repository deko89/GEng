#include "SkyBox.h"

namespace GEng
{

// SkyBox ///////////////////////////////////////////////////////////
SkyBox::SkyBox(const Str& path)
{
    Mesh mesh;  mesh.MakeCubeIn(1);
    model.Make(mesh);
    tex.LoadCube(path);
}

void SkyBox::Draw() const
{
	tex.Use();
	glEnable(GL_DEPTH_CLAMP);   // Отключить отсечение по расстоянию от камеры.
	glDisable(GL_DEPTH_TEST);   // Отключить тест глубины.
	model.Draw();
	glEnable(GL_DEPTH_TEST);    // Включить тест глубины.
	glDisable(GL_DEPTH_CLAMP);  // Включить отсечение по расстоянию от камеры.
}

}
