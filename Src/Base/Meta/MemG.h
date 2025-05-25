#ifndef MemG_H
#define MemG_H

#include "Meta.h"

namespace GEng
{

/// Глобальная видео память (uniform буфер в OpenGl).
class MemG
{	public:
	/// Создать память размером size байт.
	void Create(size_t size);

	~MemG();

	/// Сделать активным.
	void Bind();

	/// Копировать данные.
	template<typename T> void Copy(const T& v)
	{	glBindBuffer(GL_UNIFORM_BUFFER, id);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(T), &v);
	}

	/** Установить привязку к слоту (точке обмена данными).
		param[in] s - точка привязки. */
	void SetSlot(SlotMemG s);

	/// Получить слот.
	SlotMemG GetSlot() const {return slot;}
private:
	GLuint id = 0;		///< id.
	SlotMemG slot = 0;	///< Слот (точка обмена данными).
};

}

#endif
