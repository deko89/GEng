#ifndef Models_H
#define Models_H

#include <filesystem>
#include "pugixml/src/pugixml.hpp"
#include "Std/MassivPtr.h"
#include "GEng/Base/Meta/Meta.h"
#include "GEng/Base/Meta/Shader.h"
#include "GEng/Base/Meta/Texture.h"
#include "GEng/Base/Meta/Model.h"

namespace GEng
{

/// Стандартная модель.
class ModelStd: public ModelVi
{	public:
	ModelStd(ShaderType shT = shPosTex, Texture* tex = nullptr);
	void Make(const Mesh& mesh, const PlaceTex& placeTex = PlaceTex());
	ClassModel* Class() const override {return pClass;}
	void SetClass(ClassModel* c) override {pClass = c;}
	void SetShader(ShaderType shT) {shader.Set(shT);}
	virtual Pos   GetPos() const override;					///< Получить позицию.
	virtual void  SetPos(const Pos& p) override;			///< Установить позицию.
	virtual void  Move(const Vec3& v) override;				///< Передвинуть.
	virtual Angle GetAngle() const override;				///< Получить углы.
	virtual void  SetAngle(const Angle& a) override;		///< Установить углы.
	virtual Scale GetScale() const override;				///< Получить масштаб.
	virtual void  SetScale(const Scale& s) override;		///< Установить масштаб.
	void SetTexture(Texture& tex);
	virtual void Draw() const override;
	void Print() const;
	const Box& GetBoundBox() const override;
	bool IsIntersect(const Ray& ray) const override;
	Mat4 GetMatTrans() const; ///< Получить матрицу трансформации.
	Mat4 GetMatTransPos() const; ///< Получить матрицу трансформации (только позиция).
protected:
	ClassModel* pClass = nullptr; ///< Класс/тип модели.
	Pos pos = Pos(0, 0, 0);
	mutable Angle angle = Angle(0, 0, 0);
	Scale scale = Scale(1, 1, 1);
	ShaderRef shader;
	TextureRef texture;
	Box boundBox; ///< Ограничивающая область.
};

/// Модель плоскости.
class ModelPlane: public ModelStd
{	public:
	Vec2 size{1, 1};			///< Размеры.
	Vec2N sgm{1, 1};			///< Число сегментов.
	PosTex texN{1, 1};			///< Число повторений текстуры.
	ModelPlane(ShaderType shT = shPosTex, Texture* tex = nullptr);
	void Update() override;
	void SetSize(Vec2 s);
	void SetSgm(Vec2N s);		///< Задать число сегментов.
	void SetTexN(PosTex n);		///< Задать число повторений текстуры.
};

/// Модель параллелепипеда.
class ModelBox : public ModelStd
{	public:
	ModelBox(ShaderType shT = shPosTex, Texture* tex = nullptr);
	ModelBox(Pos a, Pos b);
	void Update() override; ///< Обновить модель (применить параметры, модификаторы).
protected:
	Pos a = Pos(0, 0, 0),	///< Позиция начала.
		b = Pos(1, 1, 1);	///< Позиция конца.
};

/** Плоская модель из текстуры, которая всегда развёрнута к камере (например растение).
	Точка привязки у текстуры - снизу по центру.
	@note Метод SetAngle здесь не работает. */
class Model2d: public ModelStd
{	public:
	Model2d(ShaderType shT = shPosTex, Texture* tex = nullptr);
	void SetSize(Val x, Val y);
	void Draw() const override;
};

/// Модель цилиндра.
class ModelCylinder : public ModelStd
{	public:
	Val d = 0.1;	///< Диаметр.
	Val len = 1;	///< Длина.
	ValN sgmC = 16;	///< Число сегментов в окружности.
	ValN sgmL = 1;	///< Число сегментов по длине.
	bool bCloseB = 1;	///< Закрыто начало.
	bool bCloseE = 1;	///< Закрыт конец.
	Points::Modifs modif;	///< Модификаторы.
	ModelCylinder() {}
	ModelCylinder(Val d, Val len, ValN sgmC, ValN sgmL, bool bCloseB = 1, bool bCloseE = 1);
	void Update(); ///< Обновить модель (применить параметры, модификаторы).
protected:
};

/// Модели.
class Models : public MassivPtr<Model>
{	public:
    typedef MassivPtr<Model> Base;
    void Save(pugi::xml_node ndModels);
    void Load(pugi::xml_node ndModels);
	void Draw() const;
};

/// Группа моделей в виде линии.
class GroupLine : public Model
{	public:
	Pos a = Pos(0, 0, 0);	///< Позиция начала.
	Pos b = Pos(1, 0, 0);	///< Позиция конца.
	size_t n = 3;			///< Количество моделей.
	ClassModel* pClass = nullptr; ///< Класс/тип модели.
	void Update() override; ///< Обновить модель (применить параметры).
	void UpdatePos();		///< Обновить только позиции.
    void Save(pugi::xml_node ndGroup);
    void Load(pugi::xml_node ndGroup);
	Pos GetPos() const override;		///< Получить позицию.
	void SetPos(const Pos& p) override;	///< Установить позицию.
	void Draw() const override;
	bool IsIntersect(const Ray& ray) const override;
protected:
	Models models;
};

/// Класс модели.
class ClassModel
{	public:
	virtual ~ClassModel() {}
	/// Создать экземпляр класса.
	virtual Model* Instance(Models& models) = 0;
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
		m->SetTexture(tex);
		return m;
	}
};

/// Класс модели 2d.
class ClassModel2d : public ClassModelT<Model2d>
{	public:
	typedef ClassModelT<Model2d> Base;
	static constexpr Str nameType = "2d";
	Vec2 size;
	Model* Instance(Models& models) override;
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
