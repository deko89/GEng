#include "MemG.h"

namespace GEng
{

void MemG::Create(size_t size)
{	glGenBuffers(1, &id);
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW); // Выделение памяти.
}

MemG::~MemG()
{	glDeleteBuffers(1, &id);
}

void MemG::Bind()
{	glBindBuffer(GL_UNIFORM_BUFFER, id);
}

void MemG::SetSlot(SlotMemG s)
{	slot = s;
	glBindBufferBase(GL_UNIFORM_BUFFER, slot, id);
}

}
