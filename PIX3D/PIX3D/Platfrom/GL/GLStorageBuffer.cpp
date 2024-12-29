#include "GLStorageBuffer.h"
#include <glad/glad.h>
#include <Core/Core.h>

namespace PIX3D
{
	namespace GL
	{

		void GLStorageBuffer::Create(uint32_t bindingpoint, uint32_t size)
		{
			m_Size = size;

			glCreateBuffers(1, &m_Handle);
			PIX_ASSERT_MSG(m_Handle, "failed to create buffer!");

			glNamedBufferStorage(m_Handle, size, nullptr, GL_MAP_WRITE_BIT);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingpoint, m_Handle);
		}

		void* GLStorageBuffer::MapBuffer()
		{
			return (void*)glMapNamedBuffer(m_Handle, GL_WRITE_ONLY);
		}

		void GLStorageBuffer::UnMapBuffer()
		{
			glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		}

		void GLStorageBuffer::Destroy()
		{
			glDeleteBuffers(1, &m_Handle);
		}
	}
}
