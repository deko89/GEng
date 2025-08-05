#ifndef GEng_H
#define GEng_H

#include "GEng/Base/Meta/Shader.h"
#include "GEng/Base/Context.h"
#include "GEng/View/Wnd/WndGui.h"
#include "GEng/World/World.h"

namespace GEng
{

/// Движок.
class Engine : public Environment
{	public:
    Engine();
    ~Engine();
    bool Init();                ///< Инициализация.
    View* CreateViewWorld();    ///< Создать мир и вид по умолчанию.
    void Clear();               ///< Очистка.
    int Execute();              ///< Запуск.
    WndGui& GetWnd();           ///< Получить активное окно.
    Camera& GetCam();           ///< Получить текущую камеру.
protected:
	// Переменные.
    bool bRun = false;	///< Запущен ли.
    Context context;
    Shaders shaders;	///< Шейдеры.
    MassivPtr<World> aWorld;
    WndGui wnd;
    friend Context& GetContext();
    friend Shaders& GetShaders();
};

extern Engine eng;
/// Получить текущий движок.
inline Engine& GetEng() {return eng;}

}

#endif
