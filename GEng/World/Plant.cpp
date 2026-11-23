#include "Plant.h"
#include <iostream>
#include "GEng/Base/Meta/Meta.h"

namespace GEng
{

// ModelTrunk ////////////////////////////////////////////////////////
ValN ModelTrunk::nSgmLen = 30;
ValN ModelTrunk::nSgmD = nSgmLen * pi;
const size_t iModifSpline = 1; // Позиция Modifs::Spline.
ModelTrunk::ModelTrunk(const vector<Pos>& aKey)
{
	modif.Make<Modifs::Taper>(0.008 / d);
	modif.Make<Modifs::Spline>(aKey); // Соответствует iModifSpline.
	Update();
}
void ModelTrunk::Update()
{
	// Расчёт данных.
	len = GetSpline().GetLen();
	d = rD * len;
	sgmL = len * nSgmLen;
	sgmC = d * nSgmD;
	if (sgmC < 2) sgmC = 2;

	if (logLvl >= LogDebug)
		std::cout	<< "ModelTrunk::Update:"
					<< " len = " << len
					<< ", d = " << d
					<< ", sgmL = " << sgmL
					<< ", sgmC = " << sgmC
					<< std::endl;

	// Построение.
	ModelCylinder::Update();
}
SplineCalc& ModelTrunk::GetSpline()
{
	Modifs::Spline* s = dynamic_cast<Modifs::Spline*>( modif[iModifSpline] );
	assert(s);
	return s->spline;
}
// ModelTree /////////////////////////////////////////////////////////

}
