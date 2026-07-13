#include "TreeGen.h"
#include <iostream>
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
	return menu;
}

}
