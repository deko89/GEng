#include "Models.h"
#include <iostream>
#include <map>
#include "glm/geometric.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "GEng/View/Camera.h"
#include "GEng/GEng.h"

namespace GEng
{

// ModelStd /////////////////////////////////////////////////////////
ModelStd::ModelStd(ShaderType shT, Texture* tex) :
	shader(shT),
	texture(tex)
{
}
void ModelStd::Make(const Mesh& mesh, const PlaceTex& placeTex)
{	ModelVi::Make(mesh, placeTex);
	boundBox = mesh.aVert.CalcBoundBox();
}
Pos ModelStd::GetPos() const
{
	return pos;
}
void ModelStd::SetPos(const Pos& p)
{	Pos d = p - pos;
	pos = p;
	boundBox.a += d;
	boundBox.b += d;
}
void ModelStd::Move(const Pos& p)
{
	pos += p;
	boundBox.a += p;
	boundBox.b += p;
}
Angle  ModelStd::GetAngle() const
{
	return angle;
}
void ModelStd::SetAngle(const Angle& a)
{
	angle = a;
}
Scale ModelStd::GetScale() const
{
	return scale;
}
void ModelStd::SetScale(const Scale& s)
{
	scale = s;
}
void ModelStd::SetTexture(Texture& tex)
{
	texture.Set(tex);
}
void ModelStd::Draw() const
{
	shader.Use();
	shader.SetTrans( GetMatTrans() );
	texture.Use();
	ModelVi::Draw();
}
void ModelStd::Print() const
{
	std::cout << "ModelStd " << this << ": pos " << pos << ", angle " << angle << ", scale " << scale << std::endl;
}
const Box& ModelStd::GetBoundBox() const
{	return boundBox;
}
bool ModelStd::IsIntersect(const Ray& ray) const
{
	return boundBox.IsIntersect(ray);
}
Mat4 ModelStd::GetMatTrans() const
{
	Mat4 mat(1);
	mat = glm::translate(mat, pos);
	mat = glm::rotate(mat, angle.z, Vec3(0, 0, 1));
	mat = glm::rotate(mat, angle.y, Vec3(0, 1, 0));
	mat = glm::rotate(mat, angle.x, Vec3(1, 0, 0));
	mat = glm::scale(mat, scale);
	return mat;
}
Mat4 ModelStd::GetMatTransPos() const
{
	Mat4 mat(1);
	mat = glm::translate(mat, pos);
	return mat;
}
// ModelPlane ////////////////////////////////////////////////////////
ModelPlane::ModelPlane(ShaderType shT, Texture* tex) :
	ModelStd(shT, tex)
{	Update();
}
void ModelPlane::Update()
{
    Mesh mesh;
    mesh.MakePlane(size.x, size.y, sgm.x, sgm.y);
    PlaceTex plTex;
    plTex.SetPlane(sgm.x, sgm.y, texN.u, texN.v);
    ModelStd::Make(mesh, plTex);
}
void ModelPlane::SetSize(Vec2 s)
{	size = s;
	Update();
}
void ModelPlane::SetSgm(Vec2N s)
{	sgm = s;
	Update();
}
void ModelPlane::SetTexN(PosTex n)
{	texN = n;
	Update();
}
// ModelBox //////////////////////////////////////////////////////////
ModelBox::ModelBox(ShaderType shT, Texture* tex) :
	ModelStd(shT, tex)
{	Update();
}
ModelBox::ModelBox(Pos a, Pos b) :
	a(a), b(b)
{	Update();
}
void ModelBox::Update()
{	Mesh mesh;
	mesh.MakeBox(a, b);
	PlaceTex plTex;
	plTex.SetBox();
	ModelStd::Make(mesh, plTex);
}
// Model2d /////////////////////////////////////////////////////////////////////
Model2d::Model2d(ShaderType shT, Texture* tex) :
	ModelStd(shT, tex)
{
    Mesh mesh;
    mesh.MakePlane(1, 1, 1, 1);
	mesh.aVert.Rotate( Angle(pi2, 0, pi2) );
	mesh.aVert.Move( Vec3(0, -0.5, 0) );

    PlaceTex plTex;
    plTex.SetPlane(1, 1, 1, 1);

    ModelStd::Make(mesh, plTex);
}
void Model2d::SetSize(Val x, Val y)
{	SetScale( Scale(1, x, y) );
	const Pos pos = GetPos();
	x *= 0.5;
	boundBox.a.x = pos.x - x;
	boundBox.a.y = pos.y - x;
	boundBox.a.z = pos.z;
	boundBox.b.x = pos.x + x;
	boundBox.b.y = pos.y + x;
	boundBox.b.z = pos.z + y;
}
void Model2d::Draw() const
{
	const Camera& cam = GetEng().GetCam();
	const Scale s = GetScale();
	// Вектор на камеру.
	const Vec3 v = cam.pos - GetPos()
		// Небольшой отступ назад, чтобы меньше вращалось, когда близко.
		- cam.GetLook() * (s.y / 10);
	// Небольшой наклон (если смотрим сверху).
		// Если смотрим на уровне модели, то не наклоняем.
	Val h;
	if		(v.z > s.z) h = v.z - s.z;
	else if	(v.z < 0  ) h = v.z;
	else h = 0;
		// Вычисление угла.
	const Val l = glm::length( Vec2(v.x, v.y) );
	angle.y = atan2(h, l) / -9;
	// Поворот всегда к камере.
	angle.z = atan2(v.y, v.x);
	// Рисование.
	ModelStd::Draw();
}
// ModelCylinder /////////////////////////////////////////////////////
ModelCylinder::ModelCylinder(Val d, Val len, ValN sgmC, ValN sgmL, bool bCloseB, bool bCloseE) :
	d(d), len(len), sgmC(sgmC), sgmL(sgmL), bCloseB(bCloseB), bCloseE(bCloseE)
{
    Update();
}
void ModelCylinder::Update()
{
    Mesh mesh;
    mesh.MakeCylinder(d, len, sgmC, sgmL, bCloseB, bCloseE);

    Points::Data pointsData(mesh.aVert);
    pointsData.sz = {len, d, d};
    modif.Set(pointsData);

    PlaceTex plTex;
    plTex.SetCylinder(sgmC, sgmL, 2, 1);

    ModelStd::Make(mesh, plTex);
}
// Models ////////////////////////////////////////////////////////////
void Models::Save(pugi::xml_node ndModels)
{	using namespace pugi;
	// Группировка.
	std::map<std::string, std::vector<Model*>> mapMod;
    for (Model* m : *this)
    {	ClassModel* c = m->Class();
		if (c == nullptr) continue;
		mapMod[c->Name()].push_back(m);
    }
	// Запись.
    for (const auto& p : mapMod)
    {	auto& [nameMod, aMod] = p;
		xml_node ndGrMod = ndModels.append_child(nameMod);
		for (Model* m : aMod)
		{	xml_node ndM = ndGrMod.append_child("m");
			GEng::Save(m->GetPos(), ndM.append_child("pos"));
		}
    }
}
void Models::Load(pugi::xml_node ndModels)
{	using namespace pugi;
	clear();
    for (xml_node ndGrMod : ndModels.children())
    {   ClassModel* c = GetEng().aClass.Get( ndGrMod.name() );
        if (!c)
        {	std::cerr << "No class " << ndGrMod.name() << std::endl;
            continue;
        }
        for (xml_node ndM : ndGrMod.children("m"))
        {   Model* m = c->Instance(*this);
            m->SetPos( GEng::Load<Pos>( ndM.child("pos") ) );
        }
    }
}
void Models::Draw() const
{   for (Model* m : *this)
        m->Draw();
}
// ClassModel2d //////////////////////////////////////////////////////
Model* ClassModel2d::Instance(Models& models)
{	Model2d* m = (Model2d*) Base::Instance(models);
	m->SetShader(shPosTexA);
	m->SetSize(size.x, size.y);
	return m;
}
// ClassModels ///////////////////////////////////////////////////////
void ClassModels::Load(const std::filesystem::path& path)
{
	namespace fs = std::filesystem;
	using namespace pugi;
	for (const fs::directory_entry& dirEntry : fs::directory_iterator(path))
	{
		if ( dirEntry.is_directory() )
		{	// Открытие.
			fs::path p = dirEntry.path() / "set.xml";
			xml_document doc;
			xml_parse_result result = doc.load_file( p.c_str() );
			if (!result)
			{	std::cerr << "No load " << p << std::endl;
				continue;
			}
			xml_node ndSet = doc.child("set");
			// Разбор.
			Str type = ndSet.attribute("type").value();
			if (type == ClassModel2d::nameType)
			{	ClassModel2d* c = Make<ClassModel2d>();
				c->SetName( dirEntry.path().filename().string() );
				c->size.y = ndSet.child("size").attribute("h").as_double();
				Image img = c->tex.Load(
					(dirEntry.path() / "img.png").string(),
					TexPar{.alpha = true});
				c->size.x = c->size.y * img.X() / img.Y();
			}
		}
	}
}
ClassModel* ClassModels::Get(const Str& name) const
{
	auto isName = [&name](const ClassModel* mod)
	{	return mod->Name() == name;
	};
	auto it = std::find_if(begin(), end(), isName);
	if (it == end())
		return nullptr;
	return *it;
}

}
