#pragma once
#include "GLObject.h"
#include <string>

namespace PIX3D
{
	namespace GL
	{
		class GLTexture : public GLObject
		{
		public:
			GLTexture() = default;
			~GLTexture();

			void LoadFromFile(const std::string& path);
			void Bind();
			
			void Destroy();

			uint64_t GetVramTextureID() { return m_TextureID; }

		private:
			uint64_t m_TextureID = 0; // Vram Texture ID
		};
	}
}
