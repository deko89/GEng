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
Vec2I View::WndToView(Vec2I p) const
{	return p - pos.pos;
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
	if (event.type == SDL_MOUSEBUTTONDOWN)
    {	if (event.button.button == SDL_BUTTON_LEFT)
		{	if (world)
			{	world->sel.aMod.clear();
				for (Model* m : world->models)
				{	const Box& box = m->GetBoundBox();
					Vec2 p = WndToView(Vec2I(event.button.x, event.button.y));
					p.x /= pos.w; p.y /= pos.h;
					Ray ray {.pos = cam.pos, .dir = cam.CalcVecPoint(p)};
					if ( box.IsIntersect(ray) )
					{	world->sel.aMod.push_back(m);
						world->sel.pos = m->GetPos();
						world->sel.angle = m->GetAngle();
						world->sel.scale = m->GetScale();
						break;
					}
				}
			}
		}
    }
    cam.ProcessEventInput(event);
}
void View::ProcessStateInput(Val timeDelta)
{
    cam.ProcessStateInput(timeDelta);
}

}
