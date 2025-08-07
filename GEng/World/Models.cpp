#include "Models.h"
#include <iostream>
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
Pos ModelStd::GetPos() const
{
	return pos;
}
void ModelStd::SetPos(const Pos& p)
{
	pos = p;
}
void ModelStd::Move(const Pos& p)
{
	pos += p;
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
// Model2d /////////////////////////////////////////////////////////////////////
Model2d::Model2d()
{
    Mesh mesh;
    mesh.MakePlane(1, 1, 1, 1);
	mesh.aVert.Rotate( Angle(0, pi2, 0) );
	mesh.aVert.Move( Vec3(0, -0.5, 1) );

    PlaceTex plTex;
    plTex.SetPlane(1, 1, 1, 1);

    ModelStd::Make(mesh, plTex);
}
void Model2d::Draw() const
{
	const Camera& cam = GetEng().GetCam();
	const Vec3 v = cam.pos - GetPos();
	//const Pos p = GetPos();
	// Небольшой наклон (если смотрим сверху).
	const Val l = glm::length(Vec2(v.x, v.y) );
	angle.y = atan2(v.z, l) / -5;
	// Поворот всегда к камере.
	angle.z = atan2(v.y, v.x);
	// Маленькое смещение к камере, когда идёт наклон.
	//v.z = 0;
	//v = glm::normalize(v);
	//v *= scale.y * (angle.y / -pi2) / 5;
	//pos += v;
	// Рисование.
	ModelStd::Draw();
	// Возврат позиции.
	//pos = p;
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
// BaseEngine ///////////////////////////////////////////////////////
void Models::Draw() const
{   for (Model* m : *this)
        m->Draw();
}

}
