#include "GEng/Project.h"
#include <iostream>
#include "GEng/View/Wnd/Dialog/DlgFile.h"
#include "GEng/GEng.h"

namespace GEng
{

// Project ///////////////////////////////////////////////////////////
void Project::Load()
{
	DlgFile d{.path = path, .filter = "xml"};
	if ( d.Open() )
		Load(d.path);
}

void Project::Load(const std::filesystem::path& path)
{	using namespace pugi;
    xml_document doc;
    xml_parse_result result = doc.load_file( path.c_str() );
    if (!result)
    {	std::cerr << "No load file " << path << std::endl;
        return;
    }
	this->path = path;
	xml_node ndWnd = doc.child("wnd");
    GetEng().GetWnd().Load(ndWnd);
	xml_node ndWorld = doc.child("world");
    world.Load(ndWorld);
}

void Project::Save()
{	using namespace pugi;
	if ( path.empty() )
	{	SaveAs();
		return;
	}
    xml_document doc;
	xml_node ndWnd = doc.append_child("wnd");
	GetEng().GetWnd().Save(ndWnd);
	xml_node ndWorld = doc.append_child("world");
	world.Save(ndWorld);
    doc.save_file(path.c_str());
    std::cout << _("Сохранено в ") << path << std::endl;
}

void Project::SaveAs()
{	DlgFile d{.path = path, .filter = "xml"};
	if ( d.Save() )
	{	path = d.path;
		Save();
	}
}

void Project::Clear()
{

}

void Project::AddGroupLine()
{
	if ( GetEng().aClass.empty() ) return;
	ClassModel* pClass = GetEng().aClass.front();
	GroupLine* group = world.models.Make<GroupLine>();
	group->pClass = pClass;
	group->Update();
}

}
