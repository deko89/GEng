#include "Model.h"

namespace GEng
{

// ModelVi //////////////////////////////////////////////////////////
ModelVi::ModelVi()
{	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vb);
	glGenBuffers(1, &ib);
}
ModelVi::~ModelVi()
{	glDeleteBuffers(1, &ib);
	glDeleteBuffers(1, &vb);
	glDeleteVertexArrays(1, &vao);
}
void ModelVi::Make(const Mesh& mesh, const PlaceTex& placeTex)
{	// Привязываем VAO (он запоминает следующие вызовы и хранит).
	glBindVertexArray(vao);
	// 1. Копируем вершины в буфер OpenGL.
	glBindBuffer(GL_ARRAY_BUFFER, vb); // Привязываем буфер.
	const GLsizeiptr sizeVert = mesh.aVert.size() * sizeof(Pos),
					 sizeTex = placeTex.size() * sizeof(PosTex);
	glBufferData(GL_ARRAY_BUFFER, sizeVert + sizeTex, NULL, GL_STATIC_DRAW); // Выделяем память.
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeVert, mesh.aVert.data()); // Copy.
	// 1.1. Устанавливаем указатели на вершинные атрибуты.
	glEnableVertexAttribArray(0);	// Включаем (location = 0?).
	glVertexAttribPointer(0,		// Позиция "layout (location = 0)" в коде шейдера вершин.
		3,							// Количество чисел.
		ValId,						// Формат чисел.
		GL_FALSE,					// Нужно ли нормализовать.
		sizeof(Pos),				// Шаг между вершинами.
		(GLvoid*)0);				// Смещение начала данных.
	// 2. Устанавливаем данные текстуры.
	if (placeTex.size())
	{	glBufferSubData(GL_ARRAY_BUFFER, sizeVert, sizeTex, placeTex.data());
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, ValId, GL_FALSE, sizeof(PosTex), (GLvoid*)sizeVert);
	}
	// 3. Копируем индексы в буфер OpenGL
	nInd = mesh.aInd.size();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, nInd * sizeof(Ind), mesh.aInd.data(), GL_STATIC_DRAW);
	// Не отвязываем VAO. Чтобы можно было использовать дальше другим функциям. Отвяжется сам, при установке нового.
	// Отвязка делается так: glBindVertexArray(0);
}
//void ModelVi::UpdateVert(const Points& aVert)
//{
//	glBindVertexArray(vao);
//	glBindBuffer(GL_ARRAY_BUFFER, vb);
//	glBufferSubData(GL_ARRAY_BUFFER, 0, aVert.size() * sizeof(Pos), aVert.data());
//}
void ModelVi::Draw() const
{	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, nInd, IndId, 0);
}

}
