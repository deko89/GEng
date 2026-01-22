#include "Texture.h"
#include "Std/Media/Image.h"

namespace GEng
{

const GLenum typeGl[]
{   GL_TEXTURE_2D,
    GL_TEXTURE_2D_ARRAY,
    GL_TEXTURE_CUBE_MAP,
};
const GLint wrapGl[]
{   GL_REPEAT,
    GL_MIRRORED_REPEAT,
    GL_CLAMP_TO_EDGE,
    GL_CLAMP_TO_BORDER,
};
const GLint filtLowGl[]
{   GL_NEAREST,
    GL_LINEAR,
};
const GLint filtHighGl[]
{   GL_NEAREST,
    GL_LINEAR,
    GL_NEAREST_MIPMAP_NEAREST,
    GL_LINEAR_MIPMAP_NEAREST,
    GL_NEAREST_MIPMAP_LINEAR,
    GL_LINEAR_MIPMAP_LINEAR,
};
// TexPar ///////////////////////////////////////////////////////////
TexPar texPar;
TexPar texParArray{.type = TexType::Array, .wX = TexWrap::Edge, .wY = TexWrap::Edge};
TexPar texParCube{ .type = TexType::Cube,  .wX = TexWrap::Edge, .wY = TexWrap::Edge, .wZ = TexWrap::Edge};
void TexPar::Set() const
{   GLenum texType = typeGl[(size_t)type];
    // Развёртка.
    if (wX != TexWrap::Repeat)
        glTexParameteri( texType, GL_TEXTURE_WRAP_S, wrapGl[(size_t)wX] );
    if (wY != TexWrap::Repeat)
        glTexParameteri( texType, GL_TEXTURE_WRAP_T, wrapGl[(size_t)wY] );
    if (wZ != TexWrap::Repeat && type == TexType::Cube)
        glTexParameteri( texType, GL_TEXTURE_WRAP_R, wrapGl[(size_t)wZ] );
    if (wX == TexWrap::Color || wY == TexWrap::Color || wZ == TexWrap::Color)
    {   float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	    glTexParameterfv(texType, GL_TEXTURE_BORDER_COLOR, borderColor);
    }
    // Фильтрация.
    if (filtLow != FiltLow::Line)
        glTexParameteri( texType, GL_TEXTURE_MIN_FILTER, filtLowGl[(size_t)filtLow] );
    if (filtHigh != FiltHigh::NearMpLine)
        glTexParameteri( texType, GL_TEXTURE_MAG_FILTER, filtHighGl[(size_t)filtHigh] );
}
// Texture //////////////////////////////////////////////////////////
Texture texNull;
Texture::Texture()
{
	glGenTextures(1, &id); // 1 - кол-во текстур для генерации. Второй параметр массив id.
}
Texture::~Texture()
{
	glDeleteTextures(1, &id);
}
void Texture::Load(const Str& path, const TexPar& par)
{	// Подготовка текстуры.
	texType = GL_TEXTURE_2D;
	glBindTexture(GL_TEXTURE_2D, id); // Использование своей текстуры.
	par.Set();

    // Загрузка изображения.
    Image img(path);
	GLenum formatImg = par.alpha? GL_RGBA: GL_RGB;

    // Создание текстуры.
	glTexImage2D( GL_TEXTURE_2D,
                  0,                            // Уровень мипмапа - авто.
                  formatImg,                    // Получаемый формат текстуры.
                  img.X(), img.Y(), 0,          // Ширина, высота, рамка (всегда 0).
                  formatImg, GL_UNSIGNED_BYTE,  // Формат данных исходного изображения.
                  img.Get() );                  // Пиксели изобрежния.
	glGenerateMipmap(GL_TEXTURE_2D);

	// Освобождение памяти.
	glBindTexture(GL_TEXTURE_2D, 0);
}
void Texture::LoadArray(const Str& pathF, size_t count, const TexPar& par)
{	// Подготовка текстуры.
	texType = GL_TEXTURE_2D_ARRAY;
	glBindTexture(GL_TEXTURE_2D_ARRAY, id); // Использование своей текстуры.
	par.Set();

    // Загрузка.
    GLenum formatImg = par.alpha? GL_RGBA: GL_RGB;
	for (size_t i = 0; i < count; ++i)
	{	// Загрузка изображения.
	    Str path = PrintF(pathF, i);
	    Image img(path);
		// Создание текстуры.
		glTexSubImage3D( GL_TEXTURE_2D_ARRAY,
		                 0,                             // Уровень мипмапа - авто.
		                 0, 0, i,                       // Смещение текселя по x, y, z в массивt текстур.
		                 img.X(), img.Y(), 1,           // Ширина, высота, глубина.
		                 formatImg, GL_UNSIGNED_BYTE,   // Формат данных исходного изображения.
		                 img.Get() );                   // Пиксели изобрежния.
	}
	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

	// Освобождение памяти.
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}
void Texture::LoadCube(const Str& pathF, const TexPar& par)
{	// Подготовка текстуры.
	texType = GL_TEXTURE_CUBE_MAP;
	glBindTexture(GL_TEXTURE_CUBE_MAP, id); // Использование своей текстуры.
	par.Set();

    // Загрузка.
    GLenum formatImg = par.alpha? GL_RGBA: GL_RGB;
	for (size_t i = 0; i < 6; ++i)
	{	// Загрузка изображения.
	    Str path = PrintF(pathF, i);
	    Image img(path);
		// Создание текстуры.
	    glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                      0,                            // Уровень мипмапа - авто.
                      formatImg,                    // Получаемый формат текстуры.
                      img.X(), img.Y(), 0,          // Ширина, высота, рамка (всегда 0).
                      formatImg, GL_UNSIGNED_BYTE,  // Формат данных исходного изображения.
                      img.Get() );                  // Пиксели изобрежния.
	}
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	// Освобождение памяти.
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
void Texture::Use() const
{
    glBindTexture(texType, id);
}
// TextureRef ////////////////////////////////////////////////////////
TextureRef::TextureRef(Texture* texture)
{
	pTex = texture;
}
TextureRef::TextureRef(Texture& texture)
{
	pTex = &texture;
}
void TextureRef::Set(Texture& texture)
{
	pTex = &texture;
}
void TextureRef::Use() const
{
	if (pTex)
		pTex->Use();
}

}
