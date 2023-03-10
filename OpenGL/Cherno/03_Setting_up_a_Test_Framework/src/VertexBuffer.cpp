#include "VertexBuffer.h"
#include <glad/glad.h>

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    glGenBuffers(1, &m_RendererId); // Generowanie buffora
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererId); // Wiązanie wygenerowanego bufora jako arraya
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW); // Przypisanie danych związanemu array buforowi
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_RendererId);
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererId); // Wiązanie wygenerowanego bufora jako arraya
}

void VertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
