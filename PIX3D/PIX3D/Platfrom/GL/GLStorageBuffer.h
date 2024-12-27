#pragma once
#include "GLObject.h"
#include <Core/Buffer.hpp>

namespace PIX3D
{
	namespace GL
	{
		class GLStorageBuffer : public GLObject
		{
		public:
			void Create(uint32_t bindingpoint, uint32_t size);
			void* MapBuffer();
			void UnMapBuffer();
			void Destroy();
		private:
			uint32_t m_Size; // size in bytes
		};
	}
}
