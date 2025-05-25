#ifndef Model_H
#define Model_H

#include "Base/Types.h"

namespace GEng
{

/// Базовый 3d объект.
class Model
{	public:
	virtual ~Model() {}
	// Позиция.
	virtual Pos   GetPos() const {return Pos();}		///< Получить позицию.
	virtual void  SetPos(const Pos& p) {}				///< Установить позицию.
	virtual void  Move(const Vec3& v) {}				///< Передвинуть.
	virtual Angle GetAngle() const {return Angle();}	///< Получить углы.
	virtual void  SetAngle(const Angle& a) {}			///< Установить углы.
	virtual Scale GetScale() const {return Scale();}	///< Получить масштаб.
	virtual void  SetScale(const Scale& s) {}			///< Установить масштаб.
	virtual void Update() {};		///< Обновить в видеокарте.
	virtual void Draw() const = 0;	///< Рисование.
};

/** Модель с вершинами, и 3 индексами на треугольник.
	\details Это позволят отсекать невидимую сторону. А с GL_TRIANGLE_STRIP стороны меняются (по часовй/против).*/
class ModelVi: public Model
{	public:
	ModelVi();
	virtual ~ModelVi() override;
	void Make(const Mesh& mesh, const PlaceTex& placeTex = PlaceTex());
	//void UpdateVert(const Points& aVert); ///< Обновить вершины (число должно соответствовать).
	virtual void Draw() const override;
protected:
	GLuint vao;		///< Установщик атрибутов объекта перед рисованием.
	GLuint vb, ib;	///< Буфер вершин, и индексы порядка рисования.
	GLsizei nInd;	///< Число индексов.
};

}

#endif
