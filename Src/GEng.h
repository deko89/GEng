#ifndef GEng_H
#define GEng_H

#include "Base/Meta/Shader.h"
#include "Base/Context.h"
#include "View/Wnd/WndGui.h"
#include "World/World.h"

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

}

#endif
