#include "Plant.h"
#include <iostream>

namespace GEng
{

// ModelTrunk ////////////////////////////////////////////////////////
ValN ModelTrunk::nSgmLen = 30;
ValN ModelTrunk::nSgmD = nSgmLen * pi;
const size_t iModifSpline = 1; // Позиция Modifs::Spline.
ModelTrunk::ModelTrunk(const vector<Pos>& aKey, Os os)
{
	ModelCylinder::os = os;
	modif.Make<Modifs::Taper>(0.008 / d, os);
	modif.Make<Modifs::Spline>(aKey, os); // Соответствует iModifSpline.
	Update();
}
void ModelTrunk::Update()
{
	vector<Pos>& aKey = GetKey();

	// Проверка.
	if (aKey.size() < 2)
	{
		assert(0);
		return;
	}

	// Расчёт данных.
	len = aKey.back()[os];
	d = rD * len;
	sgmL = len * nSgmLen;
	sgmC = d * nSgmD;
	if (sgmC < 2) sgmC = 2;

	// Построение.
	ModelCylinder::Update();
}
vector<Pos>& ModelTrunk::GetKey()
{
	Modifs::Spline* s = dynamic_cast<Modifs::Spline*>( modif[iModifSpline] );
	assert(s);
	return s->aKey;
}
// ModelTree /////////////////////////////////////////////////////////
Texture ModelTree::texBark;
ModelTree::ModelTree()
{
	if (!texBark)
		texBark.Load("Res/Bark.jpg"); //tmp
	Update();
}
void ModelTree::Update()
{
	// Ствол.
	Val hStep = h / 4;
	vector<Pos> aTrunk
	{
		{0,		0,		0},
		{0.1,	0,		hStep},
		{0,		0.1,	hStep * 2},
		{0.1,	0, 		hStep * 3},
		{0.1,	0, 		hStep * 4}
	};

	SplineCalc trunkCalc(aTrunk, osZ);
	if ( !trunkCalc.Check() )
	{
		std::cerr << "trunkCalc.Check" << std::endl;
		return;
	}
	trunkCalc.Calc();
	Pos p{0, 0, hStep};
	trunkCalc.CalcPos(p);

	ModelTrunk* mod = models.Make<ModelTrunk>(aTrunk, osZ);
	mod->SetTexture(texBark);

	// Ветка.
	Val wStep = w / 4;
	Val branchVar = hStep / 2 / 4;
	vector<Pos> aBranch
	{
		p,
		{p.x += wStep,	p.y,				p.z += branchVar},
		{p.x += wStep,	p.y + branchVar, 	p.z += branchVar * 0.3},
		{p.x += wStep,	p.y,				p.z += branchVar},
		{p.x += wStep,	p.y - branchVar,	p.z += branchVar * 0.2},
	};

	ModelTrunk* modBranch = models.Make<ModelTrunk>(aBranch, osX);
	modBranch->SetTexture(texBark);

	// SplineCalc branchCalc(aTrunk, osZ);
	// if ( !branchCalc.Check() )
	// {
	// 	std::cerr << "branchCalc.Check" << std::endl;
	// 	return;
	// }
	// branchCalc.Calc();
	// Pos b{0, 0, hStep};
	// branchCalc.CalcPos(b);
}
void ModelTree::CreateBranch(Pos pos, Val len, Val angleZ, Val angleU)
{

}

}
