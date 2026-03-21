#include "GEng/Project.h"
#include <iostream>
#include "GEng/View/Wnd/Dialog/DlgFile.h"

namespace GEng
{

// Project ///////////////////////////////////////////////////////////
void Project::Load()
{
	DlgFile d{.path = path, .filter = "xml"};
	if ( d.Open() )
		Load(path);
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
	xml_node ndProj = doc.child("project");
	xml_node ndWorld = ndProj.child("world");
    world.Load(ndWorld);
}

void Project::Save()
{	using namespace pugi;
	if ( path.empty() )
	{	SaveAs();
		return;
	}
    xml_document doc;
    xml_node ndProj = doc.append_child("project");
	xml_node ndWorld = ndProj.append_child("world");
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

}
