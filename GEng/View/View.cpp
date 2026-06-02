#include "View.h"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "ImGuizmo/ImGuizmo.h"
#include "GEng/Base/Meta/Meta.h"
#include "GEng/World/Models.h"
#include "GEng/GEng.h"

namespace GEng
{

// ClassModelComboBox ////////////////////////////////////////////////
void ClassModelComboBox::Set(ClassModel* c)
{
	ClassModels& aClass = GetClassModels();
	iClass = FindIndex(aClass, c, 0);
}
ClassModel* ClassModelComboBox::Get() const
{
	ClassModels& aClass = GetClassModels();
	return aClass[iClass];
}
void ClassModelComboBox::Draw()
{
	ClassModels& aClass = GetClassModels();
	const char* combo_preview_value = aClass[iClass]->Name().c_str();
	if (ImGui::BeginCombo(_("Модель"), combo_preview_value, 0))
	{
		static ImGuiTextFilter filter;
		if (ImGui::IsWindowAppearing())
		{
			ImGui::SetKeyboardFocusHere();
			filter.Clear();
		}
		ImGui::SetNextItemShortcut(ImGuiMod_Ctrl | ImGuiKey_F);
		filter.Draw("##Filter", -FLT_MIN);

		for (size_t i = 0; i < aClass.size(); ++i)
		{
			const bool is_selected = (iClass == i);
			if (filter.PassFilter(aClass[i]->Name().c_str()))
				if (ImGui::Selectable(aClass[i]->Name().c_str(), is_selected))
				{	iClass = i;
					if (onChange) onChange();
				}
		}
		ImGui::EndCombo();
	}
}
ClassModels& ClassModelComboBox::GetClassModels() const
{	return GetEng().aClass;
}
// WndProperties /////////////////////////////////////////////////////
WndProperties::WndProperties(Selection& sel) :
	sel(sel)
{
	cbClass.onChange = [this] () {SetClassToModelFromComboBox();};
}
void WndProperties::OnChangeObj()
{
	if ( sel.aMod.empty() ) return;
	Model* mod = sel.aMod.front();
	cbClass.Set(mod? mod->Class(): 0);
}
void WndProperties::Draw()
{
	if ( sel.aMod.empty() ) return;
	Model* mod = sel.aMod.front();
	// Рисование окна.
	ImGui::Begin(_("Свойства"), NULL, ImGuiWindowFlags_AlwaysAutoResize);
	cbClass.Draw();
	if ( ImGui::InputFloat2(_("Позиция"), &sel.pos.x) )
		mod->SetPos(sel.pos);
	//ImGui::InputFloat3(_("Вращение"), &sel.angle.x);
	//ImGui::InputFloat3(_("Масштаб"), &sel.scale.x);
	mod->DrawUi();
	sel.UpdateData();
	ImGui::End();
}
void WndProperties::SetClassToModelFromComboBox()
{
	if ( sel.aMod.empty() ) return;
	Model* mod = sel.aMod.front();
	mod->SetClass(cbClass.Get());
}
// View //////////////////////////////////////////////////////////////
View::View(World* w, const RectI& pos) :
    world(w),
    pos(pos),
	wndProperties(w->sel)
{	assert(w);
}
void View::Save(pugi::xml_node ndView)
{	using namespace pugi;
    xml_node ndCamera = ndView.append_child("camera");
    cam.Save(ndCamera);
}
void View::Load(pugi::xml_node ndView)
{	using namespace pugi;
    xml_node ndCamera = ndView.child("camera");
    cam.Load(ndCamera);
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
		if (world->ground)
			world->ground->Draw();
		world->models.Draw();
		wndProperties.Draw();
		DrawTransform();
    }
	if (aShape.empty() == false)
	{
		ImGui::SetNextWindowPos( ImVec2(pos.x, pos.y) );
		//ImGui::SetNextWindowSize( ImVec2(pos.w, pos.h) );
		ImGui::Begin("_", NULL,
			ImGuiWindowFlags_NoBackground |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoFocusOnAppearing |
			ImGuiWindowFlags_NoNavInputs |
			ImGuiWindowFlags_MenuBar |
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
void View::ProcessEventMouse(SDL_Event& event)
{
	if (event.type == SDL_MOUSEBUTTONDOWN)
    {	if (event.button.button == SDL_BUTTON_LEFT)
		{	if ( world && (world->sel.aMod.empty() || !ImGuizmo::IsOver()) )
			{	world->sel.aMod.clear();
				for (Model* m : world->models)
				{
					Vec2 p = WndToView(Vec2I(event.button.x, event.button.y));
					p.x /= pos.w; p.y /= pos.h;
					Ray ray {.pos = cam.pos, .dir = cam.CalcVecPoint(p)};
					if ( m->IsIntersect(ray) )
					{	world->sel.aMod.push_back(m);
						world->sel.pos = m->GetPos();
						world->sel.angle = m->GetAngle();
						world->sel.scale = m->GetScale();
						wndProperties.OnChangeObj();
						break;
					}
				}
			}
		}
    }
    cam.ProcessEventMouse(event);
}
void View::ProcessEventKeyboard(Val timeDelta)
{
    cam.ProcessEventKeyboard(timeDelta);
}
void View::DrawTransform()
{
	static_assert(	sizeof(float) == sizeof(Val) &&
					sizeof(float[3]) == sizeof(Pos), "" );
	Selection& sel = world->sel;
	if ( sel.aMod.empty() ) return;
	Model* mod = sel.aMod.front();
	// Рисование ImGuizmo.
	Mat4 mTransform;
	float* mTrans = glm::value_ptr(mTransform);
	ImGuizmo::RecomposeMatrixFromComponents(&sel.pos.x, &sel.angle.x,
											&sel.scale.x, mTrans);
	//ImGuizmo::SetDrawlist();
	ImGuizmo::SetRect(View::pos.x, View::pos.y, View::pos.w, View::pos.h);
	Mat4 mView = cam.GetView();
	Mat4 mProj = cam.GetProjection();
	ImGuizmo::Manipulate(glm::value_ptr(mView),
						 glm::value_ptr(mProj),
						 ImGuizmo::TRANSLATE_X | ImGuizmo::TRANSLATE_Y,
						 ImGuizmo::WORLD,
						 mTrans);
	ImGuizmo::DecomposeMatrixToComponents(mTrans, &sel.pos.x, &sel.angle.x,
										  &sel.scale.x);
	// Обновление позиции.
	mod->SetPos(sel.pos);
	//mod->SetAngle(sel.angle);
	//mod->SetScale(sel.scale);
}

}
