#include "View.h"
#include "imgui.h"

namespace GEng
{

View::View(World* w, const RectI& pos) :
    world(w),
    pos(pos)
{
}
const RectI& View::GetPos() const
{   return pos;
}
void View::SetPos(const RectI& r)
{   pos = r;
}
void View::Update(Val timeDelta)
{
}
void View::Draw()
{   glViewport(pos.x, pos.y, pos.w, pos.h);
	// Установка текущей камеры шейдерам.
	Shaders& shaders = GetShaders();
	shaders.memCam.Copy( cam.GetMatrix() );
	// Рисование.
    shaders[shCubeIn].Use();
    if (world)
    {
		if (world->sky)
			world->sky->Draw();
		world->models.Draw();
    }
	if (aShape.empty() == false)
	{
		ImGui::SetNextWindowPos( ImVec2(pos.x, pos.y) );
		ImGui::SetNextWindowSize( ImVec2(pos.w, pos.h) );
		ImGui::Begin("_", NULL,
			ImGuiWindowFlags_NoBackground |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			0 );
		aShape.Draw();
		ImGui::End();
	}
}
void View::ProcessEvent(SDL_Event& event)
{
	if (event.type == SDL_WINDOWEVENT)
	{
		if (event.window.event == SDL_WINDOWEVENT_RESIZED)
		{
			pos.w = event.window.data1; //tmp Пока 1 вид на весь экран. #View1
			pos.h = event.window.data2;
			cam.fAspect = (Val)pos.w / pos.h;
		}
	}
}
void View::ProcessEventInput(SDL_Event& event)
{
    cam.ProcessEventInput(event);
}
void View::ProcessStateInput(Val timeDelta)
{
    cam.ProcessStateInput(timeDelta);
}

}
