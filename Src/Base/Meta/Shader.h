#ifndef Shader_H
#define Shader_H

#include <array>
#include "Base/Meta/MemG.h"

namespace GEng
{

/// Программа рисования для видеокарты.
class Shader
{	public:
	/// Конструктор считывает и собирает шейдер. 0 - нет шейдера.
	void Compile(StrCG sVertexShader, StrCG sGeometryShader, StrCG sFragmentShader);

	~Shader();

	void Use() const; ///< Использование программы.

	/** Привязать к глобальной памяти.
    	param[in] blockName - блок переменных в шейдере (uniform).
		param[in] uLinkPoint - точка привзяки. К ней же должен быть привязан и сам буфер (ubo). */
	void LinkMemG(StrCG blockName, SlotMemG uLinkPoint);

	/// Установить трансформацию (программа должна быть активной).
	void SetTrans(const Mat4& mat) const;
protected:
	GLuint id = 0; ///< Идентификатор программы.
	// Переменные.
	GLint iMatTrans = -1;	///< Переменная (uniform) - матрица трансформации.

	/** Собрать.
		param[in] typeShader - тип шейжера (GL_VERTEX_SHADER, ...). */
	bool CompileShader(StrCG pCode, GLenum const typeShader);
};

/// Тип шейдера.
enum ShaderType
{
	shPosTex,		///< Позиция (x,y,z) + текстура (u,v).
	shPosTexA,		///< Позиция (x,y,z) + прозрачная текстура (u,v).
	shCubeIn,		///< Куб изнутри (заднего плана, неба).
};

const size_t gShaderCount = 3; ///< Число шейдеров.

/// Стандартные шейдеры. Последовательность в массиве соответствует ShaderType.
class Shaders : public std::array<Shader, gShaderCount>
{   public:
	MemG	memCam;	///< Видеопамять камеры (позиция).
	/** Собрать. */
	void Compile();
};

/// Получить шейдеры.
Shaders& GetShaders(); // Реализовано в Engine.cpp

/// Сссылка на шеёдер.
class ShaderRef
{	public:
	ShaderRef(ShaderType shT = shPosTex);
	/// Установить.
	void Set(ShaderType shT);
	/// Сделать активным.
	void Use() const;
	void SetTrans(const Mat4& mat) const; ///< Установить трансформацию (программа должна быть активной).
protected:
	ShaderType t;
	Shader& Get() const;
};

}

#endif
