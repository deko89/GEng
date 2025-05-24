#ifndef ArDif_H
#define ArDif_H

#include <vector>

static_assert(sizeof(uint8_t) == 1);

/// Массив разных типов. Непрерывный блок памяти.
class ArDif: public std::vector<uint8_t>
{	public:
	/**	\brief Добавить элемент типа T.
		\param[in] Параметры для конструктора T.
		\return Ссылка на элемент. Временная (до перераспределения). */
	template<typename T, typename... Arg>
	T& Add(Arg... arg)
	{	const size_t pos = size();
		resize( pos + sizeof(T) );
		T& e = Get<T>(pos);
		e = T(arg...);
		return e;
	}
	/**	\brief Получить элемент типа T.
		\param[in] pos - Позиция откуда взять, в байтах.
		\return Ссылка на элемент. Временная (до перераспределения). */
	template<typename T>
	T& Get(const size_t pos) const
	{	assert( pos + sizeof(T) <= size() );
		return * (T*) (data() + pos);
	}
	/**	\brief Получить элемент типа T, и передвинуть позицию на следующий.
		\param[in] pos - Позиция откуда взять, в байтах.
		\return Ссылка на элемент. Временная (до перераспределения). */
	template<typename T>
	T& Read(size_t& pos) const
	{	T& t = Get<T>(pos);
		pos += sizeof(T);
		return t;
	}
};

#endif
