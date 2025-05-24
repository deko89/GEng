#ifndef IMAGE_H
#define IMAGE_H

#include "SDL.h"
#include "Std/Base.h"

/// Изображение.
class Image
{	public:
    static void Init();
    static void DeInit();
    Image() {}
    Image(const Str& path);     ///< Загрузить из файла.
	~Image();
	void Load(const Str& path); ///< Загрузить из файла.
	void Clear();
    uint8_t* Get() const;       ///< Получить пиксели.
    size_t X() const;   ///< Ширина.
    size_t Y() const;   ///< Высота.
private:
    void MirrorY();     ///< Отразить по вертикали.
	SDL_Surface* pSurf = nullptr;
};

#endif
