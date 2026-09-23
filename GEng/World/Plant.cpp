#include "Plant.h"
#include "GEng/Base/Meta/Meta.h"
#include <algorithm>
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/rotate_vector.hpp"

namespace GEng
{

// ModelTrunk ////////////////////////////////////////////////////////
ValN ModelTrunk::nSgmLen = 30;
ValN ModelTrunk::nSgmD = nSgmLen * pi;
const size_t iModifSpline = 1; // Позиция Modifs::Spline.
ModelTrunk::ModelTrunk(const vector<Pos>& aKey, Os os)
{
	ModelCylinder::os = os;
	modif.Make<Modifs::Taper>(0.01, os);
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
	len = LengthPolyline(aKey);
	d = rD * len;
	sgmL = len * nSgmLen;
	sgmC = d * nSgmD;
	if (sgmC < 3) sgmC = 3;

	if (logLvl)
		std::cout	<< "ModelTrunk::Update"
					<< " len: " << len
					<< " d: " << d
					<< " sgmL: " << sgmL
					<< " sgmC: " << sgmC
					<< std::endl;

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
	Val hStep = h / 2;
	vector<Pos> aTrunk
	{
		{0,		0,		0},
		{0,		0,		hStep},
		{0,		0,		hStep * 2},
		{0.1,	0.1, 	hStep * 3},
		{0,		0, 		hStep * 4}
	};

/*	SplineCalc trunkCalc(aTrunk, osZ);
	if ( !trunkCalc.Check() )
	{
		std::cerr << "trunkCalc.Check" << std::endl;
		return;
	}
	trunkCalc.Calc();
*/
	ModelTrunk* mod = models.Make<ModelTrunk>(aTrunk);
	mod->SetTexture(texBark);

	// Ветки.
/*	size_t nB = 1;
	std::mt19937 gen(seed);
	std::uniform_real_distribution<Val> distZ(0, 2*pi);
	std::uniform_real_distribution<Val> distU(-pi4, pi4);
	for (size_t b = 0; b < nB; ++b)
	{
		Val hV = h * b / nB;
		Val wV = w * (nB - b) / nB;
		Pos p{0, 0, hV};
		trunkCalc.CalcPos(p);
		if (logLvl)
			std::cout << "Branch: " << b;
		CreateBranch(p, wV, distZ(gen), distU(gen), wV*0.3, 4, gen);
	}
*/
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
void ModelTree::CreateBranch(Pos pos, Val len, Val angleZ, Val angleU,
		Val bendAmplitude, Val bendRate,
		std::mt19937& gen)
{
	if (logLvl)
		std::cout	<< " pos: " << pos
					<< " len: " << len
					<< " angleZ: " << angleZ
					<< " angleU: " << angleU
					<< " bendAmplitude: " << bendAmplitude
					<< " bendRate: " << bendRate
					<< std::endl;
	// 1. Рассчитываем число ключевых точек на 1 метр.
	const Val keyCount1m = bendRate;

	// 2. Находим общее число точек и среднее расстояние между ними.
	size_t keyCount = len * keyCount1m;
	keyCount = std::max(keyCount, 5zu);
	Val distKey = len / keyCount;

	// 3. Находим вектор направления и его плоскость.
	const Vec3 dir = glm::rotateZ(
		glm::rotateY(Vec3(1,0,0), -angleU),
		angleZ);
	const Vec3 oX = glm::rotateZ(Vec3(0,1,0), angleZ);
	const Vec3 oY = glm::cross(dir, oX);

	if (logLvl)
		std::cout	<< " keyCount: " << keyCount
					<< " distKey: " << distKey
					<< " dir: " << dir
					<< " oX: " << oX
					<< " oY: " << oY
					<< std::endl;

	// 4. Создаём ключевые точки.
	vector<Pos> aKey;
	aKey.resize(keyCount);
	aKey[0] = pos;

	const Val deltaPos = 0.3;
	std::uniform_real_distribution<Val> distD(
		(1 - deltaPos) * distKey,
		(1 + deltaPos) * distKey);

	const Val amplitude = bendAmplitude / sqrt(2);
	std::uniform_real_distribution<Val> distA(-amplitude, amplitude);

	for (size_t k = 1; k < keyCount; ++k)
	{
		pos += distD(gen) * dir;
		const Val taper = (keyCount - k + 1) / (Val)keyCount;
		aKey[k] = pos + oX * distA(gen) * taper + oY * distA(gen) * taper;
		if (logLvl)
			std::cout	<< " k: " << k
						<< " taper: " << taper
						<< " aKey[k]: " << aKey[k]
						<< std::endl;
	}

	// 5. Создание ветки.
	Os os = angleZ > 7.0/4*pi && angleZ < pi4 ||
			angleZ > 3.0/4*pi && angleZ < 5.0/4*pi ?
				osX : osY;
	if (logLvl)
		std::cout	<< " os: " << os
					<< std::endl;
	ModelTrunk* modBranch = models.Make<ModelTrunk>(aKey, os);
	modBranch->SetTexture(texBark);
}

}
