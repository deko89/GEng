/** \file
	\brief Базовые типы.
	Самые простые и часто используемые типы, константы, функции...
*/

#ifndef BASE_H
#define BASE_H

#include <string_view>
#include <string>
#include <sstream>
#include <cstdio>

/// Макрос для перевода текста.
#define _(x) x

/// Привести к строке.
#define ToStr(x) std::to_string(x)

// Строки.
using std::string;
typedef std::string Str;
typedef const Str& StrR;
typedef std::string_view StrV;
typedef char Sym;

// Константы.
const Sym gSep = '/';	///< Разделитель путей.

// Функции.

template<class C> void Limit(C& cVal, const C cMin, const C cMax)
{   if (cVal < cMin)        cVal = cMin;
    else if (cVal > cMax)   cVal = cMax;
}

template<class C> C Max(C a, C b)
{   return a > b ? a : b;
}

template<typename V> V ToVal(StrR s) {std::istringstream ss(s);  V v;  ss >> v;  return v;}
template<typename V> V ToVal(StrV s) {Str a(s);  return ToVal<V>(a);}

/// Печать в строку (обёртка printf).
template<typename... Arg> Str PrintF(const Str& fmt, Arg... arg)
{
    size_t size = std::snprintf(0, 0, fmt.c_str(), arg...);
    Str s;  s.resize(size + 1);
    std::sprintf(s.data(), fmt.c_str(), arg...);
    s.resize(size);
    return s;
}

#endif
