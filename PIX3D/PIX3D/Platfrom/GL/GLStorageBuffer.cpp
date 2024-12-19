#include "GLStorageBuffer.h"
#include <glad/glad.h>
#include <Core/Core.h>

namespace PIX3D
{
	namespace GL
	{

		void GLStorageBuffer::Create(uint32_t Bindingpoint)
		{
			glCreateBuffers(1, &m_Handle);
			PIX_ASSERT_MSG(m_Handle, "failed to create buffer!");

			glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_Handle);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, Bindingpoint, m_Handle);
		}

		void GLStorageBuffer::Update(const BufferData& data)
		{
			glNamedBufferStorage(m_Handle, data.GetSize(), data.GetData(), GL_DYNAMIC_STORAGE_BIT);
		}

		void GLStorageBuffer::Destroy()
		{
			glDeleteBuffers(1, &m_Handle);
		}
	}
}
