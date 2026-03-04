#include "World.h"
#include <iostream>

namespace GEng
{

void World::Save()
{	using namespace pugi;
    xml_document doc;
    xml_node ndWorld = doc.append_child("world");
    xml_node ndModels = ndWorld.append_child("models");
    models.Save(ndModels);
    doc.save_file(path.c_str());
}
void World::Load()
{	using namespace pugi;
    xml_document doc;
    xml_parse_result result = doc.load_file( path.c_str() );
    if (!result)
    {	std::cerr << "No load file " << path << std::endl;
        return;
    }
    xml_node ndWorld = doc.child("world");
    xml_node ndModels = ndWorld.child("models");
    models.Load(ndModels);
}

}
