#include "World.h"

namespace GEng
{

void World::Save(pugi::xml_node ndWorld)
{	using namespace pugi;
    xml_node ndModels = ndWorld.append_child("models");
    models.Save(ndModels);
}
void World::Load(pugi::xml_node ndWorld)
{	using namespace pugi;
    xml_node ndModels = ndWorld.child("models");
    models.Load(ndModels);
}
void World::Delete()
{
    for (Model* m: sel.aMod)
        models.Del(m);
    sel.aMod.clear();
}

}
