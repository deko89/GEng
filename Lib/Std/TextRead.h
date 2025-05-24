#ifndef TextRead_H
#define TextRead_H

#include "Base.h"

struct TextRead
{
	Str s; ///< Читаемая строка.
	size_t pos = 0; ///< Текущая позиция.
	TextRead() {}
	TextRead(Sym* p) : s(p) {}
	TextRead(StrR v) : s(v) {}
	void Set(StrR v);
	void End() {pos = s.size();} ///< Завершить чтение.
	void SkipD(Sym d);	///< Пропустить до разделителя d включительно.
	/// Чтение до d (без d). Поставить позицию после d.
	StrV ReadD(Sym d);
	template<typename V>
	V ReadD(Sym d = ' ')
	{	StrV s = ReadD(d);
		return ToVal<V>(s);
	}
	Sym ReadSym()
	{	SkipSpace();
		return pos < s.size()? s[pos++] : 0;
	}
	void UnReadSym() {if (pos) --pos;}
	void SkipSpace(); ///< Пропустить пробелы.
	bool Cmp(StrR v) const; ///< Текущий текст s?
	void GoTo(Sym c); ///< Перейти к символу c.
	operator bool() const {return pos < s.size();}
};

#endif



#ifdef M_IncludeCpp

void TextRead::Set(StrR v)
{
	s = v;
	pos = 0;
}
void TextRead::SkipD(Sym d)
{
	SkipSpace();
	GoTo(d);
	if ( pos < s.size() ) ++pos;
}
StrV TextRead::ReadD(Sym d)
{
	SkipSpace();
	const size_t posBegin = pos;
	GoTo(d);
	StrV v(s.data() + posBegin, pos - posBegin);
	if ( pos < s.size() ) ++pos;
	return v;
}
void TextRead::SkipSpace()
{
	for (;	pos < s.size() &&
			std::isspace(s[pos]);
			++pos) {}
}
void TextRead::GoTo(Sym c)
{	for (; pos < s.size() && s[pos] != c; ++pos) {}
}
bool TextRead::Cmp(StrR v) const
{
	return s.compare(pos, v.size(), v) == 0;
}

#endif
