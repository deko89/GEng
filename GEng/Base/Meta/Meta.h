#ifndef META_H
#define META_H

/** \file
	\brief Абстрактные типы и данные.
	Обобщение типов, переменных и т.д. для разных платформ.
*/

#define _USE_MATH_DEFINES

#include <ostream>
#include <chrono>

#ifdef __WINDOWS__
	#include "Galogen/gl.h"
#endif

#if defined(IMGUI_IMPL_OPENGL_ES2)
	#include <SDL_opengles2.h>
#else
	#define GL_GLEXT_PROTOTYPES
	#include <SDL_opengl.h>
	#include "SDL_opengl_glext.h"
#endif

#include "glm/glm.hpp"
#include "Std/Base.h"

namespace GEng
{

using namespace std::chrono;

// Системные константы.
#ifdef __linux__
	const char* const cPathFont = "/usr/share/fonts/truetype/ubuntu/Ubuntu-R.ttf";
#elif __WINDOWS__
	const char* const cPathFont = "C:\\Windows\\Fonts\\Arial.ttf";
#elif __EMSCRIPTEN__
	const char* const cPathFont = "fonts/DroidSans.ttf";
#endif

// Простейшие типы.
typedef GLfloat         Val;            ///< Число.
typedef GLint           ValI;           ///< Целое число.
typedef GLuint          ValN;           ///< Натуральное число 0, 1, 2...
typedef GLsizei         Size;           ///< Тип размеров в памяти (массивов). //GLsizeiptr
typedef GLushort        Ind;            ///< Тип индексов вершин (номеров).
typedef const GLchar*   StrCG;          ///< Строка (для видеокарты).
typedef time_point<system_clock> Time;  ///< Время.

// Специализированные типы.
typedef GLuint		SlotMemG;			///< Слот глобальной памяти (uniform bufer).
const GLenum ValId = GL_FLOAT;			///< Код типа чисел (для команд рисования).
const GLenum IndId = GL_UNSIGNED_SHORT;	///< Код типа индексов вершин (для команд рисования).

// Общие типы.
/// Цвет.
struct Color
{
	union
	{
		struct {uint8_t r, g, b, a;};
		uint8_t m[4];
		uint32_t col;
	};
	Color() {}
	Color(uint32_t c) : col(c) {}
	operator uint32_t() const {return col;}
};

// Простые 2d типы.
typedef glm::vec<2, ValI> Vec2I;	///< Вектор 2d целочисленный.
typedef glm::vec2 Vec2;		///< Вектор с 2 числами.

	/// Прямоугольник.
template<typename T>
struct RectT
{	union
	{	struct {T x, y, w, h;}; ///< Позиция, ширина, высота.
		struct {glm::vec<2, T> pos, size;};
	};
};

using RectI = RectT<ValI>;
using Rect = RectT<Val>;

	/// Координата текстуры.
struct PosTex
{	Val u, v;
};

// Простые 3d типы.
typedef glm::vec3 Pos;		///< Позиция.
typedef glm::vec3 Angle;	///< Угол (по локальным осям объекта yaw, pitch, roll).
typedef glm::vec3 Scale;	///< Масштаб.
typedef glm::vec3 Vec3;		///< Вектор с 3 числами.
typedef glm::vec4 Vec4;		///< Вектор с 4 числами.
typedef glm::mat3 Mat3;		///< Матрица 3х3.
typedef glm::mat4 Mat4;		///< Матрица 4х4.
typedef glm::vec3::length_type OsType;	///< Тип индекса оси (X, Y, Z).

std::ostream& operator<<(std::ostream& os, const Vec2& v);
std::ostream& operator<<(std::ostream& os, const Vec3& v);
std::ostream& operator<<(std::ostream& os, const Vec4& v);
std::ostream& operator<<(std::ostream& os, const Mat4& m);

// Константы.
static_assert(std::numeric_limits<Val>::has_quiet_NaN);
static_assert(std::numeric_limits<Val>::has_infinity);
const Val vNaN = std::numeric_limits<Val>::quiet_NaN();
const Val vInf = std::numeric_limits<Val>::infinity();
const Val pi = (Val) M_PI, pi2 = (Val) M_PI_2, pi4 = (Val) M_PI_4;

// Оси.
enum Os : OsType {osX, osY, osZ};

enum class PolygonMode {Point, Line, Fill};
void SetPolygonMode(PolygonMode mode);

// Функции glm.
//#include "glm/geometric.hpp"
//	glm::length(Vec v) ///< Длина вектора.
//	glm::distance(Vec v0, Vec v1) ///< length(v0 - v1).
//	normalize(vec)	///< Нормализация вектора (то же направление, длина 1).
//	float f = glm::dot(v1, v2);		///< Скалярное произведение.
//	vec3  v = glm::cross(v1, v2);	///< Векторное произведение
//#include "glm/gtx/rotate_vector.hpp"
//	glm::rotate(Vec v, angle) ///< Вращение вектора.
//#include "glm/trigonometric.hpp"
//	glm::radians(Val) ///< Преобразование градусов в радианы.
//	glm::degrees(Val) ///< Преобразование радиан в градусы.

static_assert( sizeof(Val) == 4, "" );
static_assert( sizeof(PosTex) == 8, "" );

}

#endif
