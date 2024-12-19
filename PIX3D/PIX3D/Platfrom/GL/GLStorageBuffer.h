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
			void Create(uint32_t Bindingpoint);
			void Update(const BufferData& data);
			void Destroy();
		private:

		};
	}
}