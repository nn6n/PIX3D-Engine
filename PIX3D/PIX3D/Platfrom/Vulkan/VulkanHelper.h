#pragma once
#include "VulkanGraphicsContext.h"

namespace PIX3D
{
	namespace VK
	{
		class VulkanHelper
		{
		public:
			static VkImageView CreateImageView(VkDevice Device, VkImage Image, VkFormat Format, VkImageAspectFlags AspectFlags,
				VkImageViewType ViewType, uint32_t LayerCount, uint32_t mipLevels)
			{
				VkImageViewCreateInfo ViewInfo =
				{
					.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
					.pNext = NULL,
					.flags = 0,
					.image = Image,
					.viewType = ViewType,
					.format = Format,
					.components = {
						.r = VK_COMPONENT_SWIZZLE_IDENTITY,
						.g = VK_COMPONENT_SWIZZLE_IDENTITY,
						.b = VK_COMPONENT_SWIZZLE_IDENTITY,
						.a = VK_COMPONENT_SWIZZLE_IDENTITY
					},
					.subresourceRange = {
						.aspectMask = AspectFlags,
						.baseMipLevel = 0,
						.levelCount = mipLevels,
						.baseArrayLayer = 0,
						.layerCount = LayerCount
					}
				};

				VkImageView ImageView;
				VkResult res = vkCreateImageView(Device, &ViewInfo, NULL, &ImageView);
				VK_CHECK_RESULT(res, "vkCreateImageView");
				return ImageView;
			}
		private:

		};
	}
}
