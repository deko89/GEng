#include "ClassModel.h"
#include <iostream>

namespace GEng
{

// ClassModel2d //////////////////////////////////////////////////////
bool ClassModel2d::SetClassPar(Model* m)
{
	if ( typeid(*m) != typeid(Model2d) )
		return 0;
	Model2d* m2d = (Model2d*) m;
	Base::SetClassPar(m2d);
	m2d->SetShader(shPosTexA);
	m2d->SetSize(size.x, size.y);
	return 1;
}
// ClassModels ///////////////////////////////////////////////////////
void ClassModels::Load(const std::filesystem::path& path)
{
	namespace fs = std::filesystem;
	using namespace pugi;
	for (const fs::directory_entry& dirEntry : fs::directory_iterator(path))
	{
		if ( dirEntry.is_directory() )
		{	// Открытие.
			fs::path p = dirEntry.path() / "set.xml";
			xml_document doc;
			xml_parse_result result = doc.load_file( p.c_str() );
			if (!result)
			{	std::cerr << "No load " << p << std::endl;
				continue;
			}
			xml_node ndSet = doc.child("set");
			// Разбор.
			Str type = ndSet.attribute("type").value();
			if (type == ClassModel2d::nameType)
			{	ClassModel2d* c = Make<ClassModel2d>();
				c->SetName( dirEntry.path().filename().string() );
				c->size.y = ndSet.child("size").attribute("h").as_double();
				Image img = c->tex.Load(
					(dirEntry.path() / "img.png").string(),
					TexPar{.alpha = true});
				c->size.x = c->size.y * img.X() / img.Y();
			}
		}
	}
}
ClassModel* ClassModels::Get(const Str& name) const
{
	auto isName = [&name](const ClassModel* mod)
	{	return mod->Name() == name;
	};
	auto it = std::find_if(begin(), end(), isName);
	if (it == end())
		return nullptr;
	return *it;
}

}
