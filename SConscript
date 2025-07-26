import sys

# Среда.
if sys.platform == 'cygwin':	EnvCxx = 'clang++'
else:							EnvCxx = 'clang'
env = Environment(CC='clang', CXX=EnvCxx, CXXFLAGS='-std=c++20 -O2 -fPIC')

# Define.
EnvCppDef = [
	'One_Translation_Unit',
	'IMGUI_IMPL_OPENGL_LOADER_CUSTOM',
	'IMGUI_DEFINE_MATH_OPERATORS',
]
if sys.platform == 'cygwin':
	EnvCppDef += [
		'SDL_MAIN_HANDLED',
		'__WINDOWS__'
	]
env.Append(CPPDEFINES = EnvCppDef)

# Пути системных библиотек.
if sys.platform == 'cygwin':
	dirsIncLibSys = [
	# SDL
		'/mingw64/include/SDL2',
	]
else:
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
	'SDL2',
	'SDL2_image',
]

if sys.platform == 'cygwin':
	libsAll += [
		'opengl32',
		'SDL2main',
	]
else:
	libsAll += [
		'dl',
		'GL',
		'libm',
		#'libc',
		'libstdc++',
		'libc++',
	]

# Исходники библиотек.
filesSrcLib =	Glob(	'Lib/imgui/*.cpp') + \
				Split("""
						Lib/imgui/backends/imgui_impl_opengl3.cpp
						Lib/imgui/backends/imgui_impl_sdl2.cpp
						Lib/pugixml/src/pugixml.cpp
				""")
if sys.platform == 'cygwin':
	filesSrcLib += ['Lib/Galogen/gl.c']

dirsIncProj = [
	'.',
    'Lib',
    'Lib/imgui',
    'Lib/imgui/backends'
]

# Файлы кода.
filesSrcProj = Split("""
    GEng/GEng.cpp
""")

# Экспорт переменным сборкам выше (в SConstruct).
exportVars = {
	"envGEng": env,
	"dirsIncLibGEng": dirsIncLibSys,
	"libsGEng": libsAll
}
Export(exportVars)

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
