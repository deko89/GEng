# Переменные.
env = Environment(CC='clang', CXX='clang', CXXFLAGS='-std=c++20 -O2 -fPIC')
env.Append(
	CPPDEFINES = [
		'One_Translation_Unit',
		'IMGUI_IMPL_OPENGL_LOADER_CUSTOM',
		'IMGUI_DEFINE_MATH_OPERATORS',
	]
)

# Пути системных библиотек.
dirsIncLibSys = [
# C
   #'/usr/include',                         # /usr/include/math.h (libc GNU)
# C++
# SDL
    '/usr/include/SDL2',
]

dirsLibSys = [
]

# Исполняемые библотеки.
libsAll = [
    'dl',
    'GL',
    'libm',
    #'libc',
	'libstdc++',
    'libc++',
    'SDL2',
    'SDL2_image',
]

# Исходники библиотек.
filesSrcLib =	Glob(	'Lib/imgui/*.cpp') + \
				Split("""
						Lib/imgui/backends/imgui_impl_opengl3.cpp
						Lib/imgui/backends/imgui_impl_sdl2.cpp
						Lib/pugixml/src/pugixml.cpp
				""")
dirsIncProj = [
	'Src',
    'Lib',
    'Lib/imgui',
    'Lib/imgui/backends'
]

# Файлы кода.
filesSrcProj = Split("""
    Src/GEng.cpp
""")

# Запуск.
#env.SharedLibrary()
env.StaticLibrary(
	'GEng',

	# Исходники.
	filesSrcLib + filesSrcProj,

	# Папки поиска заголовочных файлов. Например #include "glm/glm.hpp" найдет в "Lib/glm/glm.hpp".
	# Также нужно для отслеживания изменений в .h файлах (неявные зависимости).
	CPPPATH = dirsIncLibSys + dirsIncProj,

	# Пути поиска исполняемых библиотек.
	LIBPATH = dirsLibSys,

	# Исполнямые библотеки.
	LIBS = libsAll
)
