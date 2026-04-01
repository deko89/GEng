import sys

default_platform = 'windows' if sys.platform == 'cygwin' else 'linux'
target_platform = ARGUMENTS.get('target', default_platform)

envCxxFlags = [
	'-std=c++23',
	'-O2',
	'-fPIC',
	'-Wno-c99-designator',
]

envLinkFlags = [
]

# Define.
envCppDef = [
	'One_Translation_Unit',
	'IMGUI_IMPL_OPENGL_LOADER_CUSTOM',
	'IMGUI_DEFINE_MATH_OPERATORS',
]

# Исходники библиотек.
filesSrcLib =	Glob(	'Lib/imgui/*.cpp') + \
				Split("""
						Lib/nativefiledialog/src/nfd_common.c
						Lib/imgui/backends/imgui_impl_opengl3.cpp
						Lib/imgui/backends/imgui_impl_sdl2.cpp
						Lib/ImGuizmo/ImGuizmo.cpp
						Lib/pugixml/src/pugixml.cpp
				""")

dirsIncProj = [
	'.',
    'Lib',
	'Lib/nativefiledialog/src/include',
    'Lib/imgui',
    'Lib/imgui/backends'
]

# Файлы кода.
filesSrcProj = Split("""
    GEng/GEng.cpp
""")

# Готовые библиотеки.
dirsLibSys = [
]

libsAll = [
	'SDL2',
	'SDL2_image',
]

# Настройки ОС.
if target_platform == 'windows':
	envCppDef += [
		'SDL_MAIN_HANDLED',
		'__WINDOWS__',
	]
	filesSrcLib += [
		'Lib/Galogen/gl.c',
		'Lib/nativefiledialog/src/nfd_win.cpp'
	]
	libsAll += [
		'SDL2main',
		'opengl32',
	]
	if sys.platform == 'cygwin':
		envCC = 'clang'
		envCxx = 'clang++'
		# Пути системных библиотек.
		dirsIncLibSys = [
			'/mingw64/include/SDL2',
	    ]
	else:
		envCC = 'x86_64-w64-mingw32-gcc'
		envCxx = 'x86_64-w64-mingw32-g++'
		envLinkFlags = [
			'-static',
		]
		# Пути системных библиотек.
		dirsIncLibSys = [
			Dir('Lib/SDL2/x86_64-w64-mingw32/include/SDL2').abspath,
			Dir('Lib/SDL2_image/x86_64-w64-mingw32/include').abspath,
		]
		dirsLibSys = [
			Dir('Lib/SDL2/x86_64-w64-mingw32/lib').abspath,
			Dir('Lib/SDL2_image/x86_64-w64-mingw32/lib').abspath,
		]
		# Исполняемые библиотеки.
		libsAll += [
			'gdi32',	# Требуется для OpenGL на Windows.
			'winmm',	# Требуется для SDL2.
			'imm32',
			'ole32',
			'oleaut32',
			'version',
			'uuid',
			'setupapi',
		]
else:
	envCC = 'clang'
	envCxx = 'clang'
	filesSrcLib += [
		'Lib/nativefiledialog/src/nfd_zenity.c'
	]
	# Пути системных библиотек.
	dirsIncLibSys = [
		'/usr/include/SDL2',
	]
	# Исполняемые библиотеки.
	libsAll += [
		'dl',
		'GL',
		'libm',
		#'libc',
		'libstdc++',
		#'libc++',
	]

# Среда.
env = Environment(
    CC = envCC,
    CXX = envCxx,
	CXXFLAGS = envCxxFlags,
	LINKFLAGS = envLinkFlags,
    CPPDEFINES = envCppDef,
)

# Экспорт переменным сборкам выше (в SConstruct).
exportVars = {
	"envGEng": env,
	"dirsIncLibGEng": dirsIncLibSys,
	"dirsLibGEng": dirsLibSys,
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

	# Исполняемые библиотеки.
	LIBS = libsAll
)
