#ifndef Models_H
#define Models_H

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
	Mat4 GetMatTrans() const; ///< Получить матрицу трансформации.
protected:
	Pos pos = Pos(0, 0, 0);
	mutable Angle angle = Angle(0, 0, 0);
	Scale scale = Scale(1, 1, 1);
	ShaderRef shader;
	TextureRef texture;
	Box boundBox; ///< Ограничивающая область.
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
    void Draw() const;
};

}

#endif
