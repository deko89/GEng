#include "TreeGen.h"
#include <iostream>
#include <algorithm>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/rotate_vector.hpp"
#include "GEng/View/Wnd/Dialog/DlgFile.h"

namespace GEng
{

TreeGen treeGen;
TreeGen& GetTreeGen() {return treeGen;}

void TreeGen::Load()
{
	DlgFile d{.path = path, .filter = "xml"};
	if ( d.Open() )
		Load(d.path);
}

void TreeGen::Load(const std::filesystem::path& path)
{	using namespace pugi;
    xml_document doc;
    xml_parse_result result = doc.load_file( path.c_str() );
    if (!result)
    {	std::cerr << "No load file " << path << std::endl;
        return;
    }
	this->path = path;
}

void TreeGen::Save()
{	using namespace pugi;
	if ( path.empty() )
	{	SaveAs();
		return;
	}
    xml_document doc;
}

void TreeGen::SaveAs()
{	DlgFile d{.path = path, .filter = "xml"};
	if ( d.Save() )
	{	path = d.path;
		Save();
	}
}

void TreeGen::Generate()
{
	if (!texBark)
		texBark.Load("Res/Bark.jpg"); //tmp

	Models& models = GetEng().proj.world.models;
	models.Del(tree);
	tree = &models.Make<ModelTree>();
	MakeBranch(Pos(0,0,0), 10, 0, pi2, 0.01, 1);
}

Menu* TreeGen::CreateMenu()
{
	Menu* menu = new Menu;
	Menu::Fold& mFile = menu->Add( _("Файл") );
	mFile.aItem.push_back( Menu::Item
	{	.name = _("Открыть"),
		.key = "Ctrl+O",
		.action = []() {GetTreeGen().Load();},
	} );
	mFile.aItem.push_back( Menu::Item
	{	.name = _("Сохранить"),
		.key = "Ctrl+S",
		.action = []() {GetTreeGen().Save();},
	} );
	mFile.aItem.push_back( Menu::Item
	{	.name = _("Сохранить как"),
		.action = []() {GetTreeGen().SaveAs();},
	} );
	mFile.aItem.push_back( Menu::Separator() );
	mFile.aItem.push_back( Menu::Item
	{	.name = _("Выход"),
		.action = []()
		{	GetEng().Exit();
		}
	} );
	Menu::Fold& mEdit = menu->Add( _("Правка") );
	mEdit.aItem.push_back( Menu::Item
	{	.name = _("Создать"),
		.action = []() {GetTreeGen().Generate();}
	} );
	return menu;
}

void TreeGen::MakeBranch(Pos pos, Val len, Val angleZ, Val angleU, Val bendAmplitude, Val bendRate)
{
	constexpr size_t minKeyPointCount = 3;
	static_assert(minKeyPointCount > 1);
	constexpr Val keyPointSpreadFactor = 0.3;

	if (logLvl >= LogDebug)
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
	keyCount = std::max(keyCount, minKeyPointCount);
	Val distKey = len / keyCount;

	// 3. Находим вектор направления и его плоскость.
	const Vec3 dir = glm::rotateZ(
		glm::rotateY(Vec3(1,0,0), -angleU),
		angleZ);
	const Vec3 oX = glm::rotateZ(Vec3(0,1,0), angleZ);
	const Vec3 oY = glm::cross(dir, oX);

	if (logLvl >= LogDebug)
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

	std::uniform_real_distribution<Val> distD(
		(1 - keyPointSpreadFactor) * distKey,
		(1 + keyPointSpreadFactor) * distKey);

	const Val amplitude = len * bendAmplitude;
	std::uniform_real_distribution<Val> ampD(-amplitude, amplitude);

	for (size_t k = 1; k < keyCount; ++k)
	{
		pos += distD(gen) * dir;
		const Val taper = 1 - (Val)(k - 1) / (keyCount - 2);
		aKey[k] = pos + oX * (ampD(gen) * taper) + oY * (ampD(gen) * taper);
		if (logLvl >= LogDebug)
			std::cout	<< " k: " << k
						<< " taper: " << taper
						<< " aKey[k]: " << aKey[k]
						<< std::endl;
	}

	// 5. Создание ветки.
	ModelTrunk& modBranch = tree->GetModels().Make<ModelTrunk>(aKey);
	modBranch.SetTexture(texBark);
}

}
