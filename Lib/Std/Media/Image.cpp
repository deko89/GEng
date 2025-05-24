#include "Image.h"
#include <stdio.h>
#include "SDL2/SDL_image.h"

// Image ////////////////////////////////////////////////////////////
void Image::Init()
{
	if ( IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) !=
	             (IMG_INIT_JPG | IMG_INIT_PNG) )
    {   printf( _("Error IMG_Init") );
    }
}
void Image::DeInit()
{
    IMG_Quit();
}
Image::Image(const Str& path)
{   Load(path);
}
Image::~Image()
{	Clear();
}
void Image::Load(const Str& path)
{	//Clear();
    assert(pSurf == nullptr);
    pSurf = IMG_Load( path.c_str() );
	if (!pSurf)
		printf(_("Error: %s\n"), SDL_GetError());
    // Инверт по Y.
    MirrorY();
}
void Image::Clear()
{	if ( pSurf && SDL_MUSTLOCK(pSurf) )
		SDL_UnlockSurface(pSurf);
	SDL_FreeSurface(pSurf);
}
uint8_t* Image::Get() const
{	if ( pSurf && SDL_MUSTLOCK(pSurf) )
	{	if ( SDL_LockSurface(pSurf) )
		{	printf(_("Error: %s\n"), SDL_GetError());
			return nullptr;
		}
	}
	return (uint8_t*) pSurf->pixels;
}
size_t Image::X() const
{   return pSurf->w;
}
size_t Image::Y() const
{   return pSurf->h;
}
void Image::MirrorY()
{
    uint8_t* pImg = Get();
    if (!pImg) return;
    const size_t szRow = pSurf->pitch;
    const size_t nStep = pSurf->h / 2;
    uint8_t* pImgR1 = pImg;
    uint8_t* pImgR2 = pImg + (pSurf->h - 1) * szRow;
    // Инверт.
    for (size_t r = 0; r < nStep; ++r)
    {   std::swap_ranges(pImgR1, pImgR1 + szRow, pImgR2);
        pImgR1 += szRow;
        pImgR2 -= szRow;
    }
}
