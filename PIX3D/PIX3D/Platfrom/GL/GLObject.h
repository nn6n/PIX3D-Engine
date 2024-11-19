#pragma once
#include <stdint.h>

namespace PIX3D
{
	namespace GL
	{
		class GLObject
		{
		public:
			uint32_t GetHandle() { return m_Handle; }
		protected:
			uint32_t m_Handle = 0;
		};
	}
}
