#include "IndexBuffer.h"
#include <glad/glad.h>

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    : m_Count(count)
{
    glGenBuffers(1, &m_RendererId); // Generowanie buffora
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId); // Wiązanie wygenerowanego bufora jako arraya
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW); // Przypisanie danych związanemu array buforowi
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_RendererId);
}

void IndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId); // Wiązanie wygenerowanego bufora jako arraya
}

void IndexBuffer::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
