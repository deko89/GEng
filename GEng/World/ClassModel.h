#ifndef ClassModel_H
#define ClassModel_H

#include <filesystem>
#include "GEng/World/Models.h"

namespace GEng
{

/// Класс модели.
class ClassModel
{	public:
	virtual ~ClassModel() {}
	/// Создать экземпляр класса.
	virtual Model* Instance(Models& models) = 0;
	/**	\brief Установить параметры класса, но не сам класс.
		\param[in] m - модель, куда устанавливается.
		\return 1 - установлено; 0 - не удалось. */
	virtual bool SetClassPar(Model* m) = 0;
	/// Удалить экземпляр класса.
	//void Delete(Model* m)	{models.Del(m);}
	const Str& Name() const {return name;}
	void SetName(const Str& n) {name = n;}
protected:
	Str name;
};

/// Класс модели (шаблон).
template<typename TypeModel>
class ClassModelT : public ClassModel
{	public:
	Texture tex;
	/// Создать экземпляр класса.
	Model* Instance(Models& models) override
	{	TypeModel* m = models.Make<TypeModel>();
		m->SetClass(this);
		return m;
	}
	bool SetClassPar(Model* m) override
	{	m->SetTexture(tex);
		return 1;
	}
};

/// Класс модели 2d.
class ClassModel2d : public ClassModelT<Model2d>
{	public:
	typedef ClassModelT<Model2d> Base;
	static constexpr Str nameType = "2d";
	Vec2 size;
	bool SetClassPar(Model* m) override;
};

/// Классы моделей.
class ClassModels : public MassivPtr<ClassModel>
{	public:
    typedef MassivPtr<ClassModel> Base;
	/// Загрузить из папки.
	void Load(const std::filesystem::path& path);
	/// Получить класс по имени.
	ClassModel* Get(const Str& name) const;
};

}

#endif
