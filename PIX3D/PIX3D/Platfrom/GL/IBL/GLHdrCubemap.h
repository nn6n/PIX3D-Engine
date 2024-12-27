#pragma once
#include <filesystem>
#include "../GLObject.h"

namespace PIX3D
{
	namespace GL
	{
		class GLHdriCubemap : public GLObject
		{
		public:
            GLHdriCubemap() = default;
			void Load(uint32_t cubemaphandle);
            void LoadHdrToCubemapCPU(const std::filesystem::path& path, uint32_t size);
			void LoadHdrToCubemapGPU(const std::filesystem::path& path, uint32_t size);
			void Test();
			void Destroy();
			void Bind() const;
		private:

		};
	}
}
