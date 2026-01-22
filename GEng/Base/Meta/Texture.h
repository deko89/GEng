#ifndef Texture_H
#define Texture_H

#include "GEng/Base/Meta/Meta.h"

namespace GEng
{

/// Тип.
enum class TexType
{   Img,        ///< Обычное 2d изображение.
    Array,      ///< Массив текстур.
    Cube,       ///< Кубическая текстура (Cubemap, например для заднего фона).
};

/// Режим развёртки (повтора) при выходе за границы.
enum class TexWrap
{   Repeat,         ///< Повтор.
    Mirror,         ///< Повтор с отражением.
    Edge,           ///< Используется крайний пиксель.
    Color,          ///< Используется заданный цвет.
};

/// Режим фильтрации при низком разрешении текстуры (интерполяция).
enum class FiltLow
{   Near,           ///< Ближайший пиксель (нет фильтрации).
    Line,           ///< Линейная интерполяция. Вычисляется средний цвет ближайших (4-ых) пикселей.
};

/// Режим фильтрации при высоком разрешении текстуры (сведение нескольких пикселей в 1; MipMap).
enum class FiltHigh
{   Near,           ///< Ближайший пиксель (без MipMap)
    Line,           ///< Линейная интерполяция 4х пиксеоей (без MipMap).
    NearMpNear,     ///< Ближайший пиксель. Ближайший MipMap.
    LineMpNear,     ///< Линейная интерполяция. Ближайший MipMap.
    NearMpLine,     ///< Ближайший пиксель. Интерполяция между (2-мя) ближайшими MipMap.
    LineMpLine,     ///< Линейная интерполяция. Интерполяция между (2-мя) ближайшими MipMap.
};

/// Параметры текстуры.
struct TexPar
{	TexType type = TexType::Img;
    TexWrap wX = TexWrap::Repeat,               ///< Развёртка (повтор) по X.
            wY = TexWrap::Repeat,               ///< Развёртка (повтор) по Y.
            wZ = TexWrap::Repeat;               ///< Развёртка (повтор) по Z.
    FiltLow  filtLow  = FiltLow::Line;          ///< Фильтрация при низком разрешении.
    FiltHigh filtHigh = FiltHigh::NearMpLine;   ///< Фильтрация при высоком разрешении.
    bool alpha = 0;                             ///< Текстура будет с прозрачным каналом - RGBA.
    void Set() const; ///< Установить параметры на текущую обычную 2d текстуру.
};

extern TexPar texPar, texParArray, texParCube;

/// Текстура.
class Texture
{	public:
	Texture();
	~Texture();

	/** Загрузить текстуру (массив, кубическую).
	    @details Для загрузки прозрачной текстуры надо установить alpha в 1.
При этом изображение должно быть в формате RGBA. Иначе в RGB.
		param[in] path - путь полный.
		param[in] pathF - шаблон пути нумерованных текстур. Формат: "<Path>/<Name>%zu.<Ext>".
		param[in] count - число текстур.
		param[in] par - параметры создания текстуры. */
	void Load(const Str& path, const TexPar& par = texPar);
    void LoadArray(const Str& pathF, size_t count, const TexPar& par = texParArray);
    void LoadCube(const Str& pathF, const TexPar& par = texParCube);

    /// Сделать текстуру активной.
	void Use() const;
protected:
	GLenum texType = GL_TEXTURE_2D; ///< Тип текстуры.
	GLuint id = 0; ///< Идентификатор текустуры.
    Texture& operator=(const Texture& t) = delete;
};

extern Texture texNull;

/// Ссылка на текстуру.
class TextureRef
{	public:
	TextureRef() {}
	TextureRef(Texture* texture);
	TextureRef(Texture& texture);
	/// Установить.
	void Set(Texture& texture);
	/// Сделать текстуру активной.
	void Use() const;
protected:
	Texture* pTex = nullptr;
};

}

#endif
