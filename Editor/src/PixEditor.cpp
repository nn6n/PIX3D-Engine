#include "PixEditor.h"
#include <Platfrom/Vulkan/VulkanHelper.h>

void PixEditor::OnStart()
{
	auto* Context = (VK::VulkanGraphicsContext*)Engine::GetGraphicsContext();
	auto specs = Engine::GetApplicationSpecs();
	{ // Create Command Buffers

        m_NumImages = Context->m_SwapChainImages.size();
		m_CommandBuffers.resize(m_NumImages);
		VK::VulkanHelper::CreateCommandBuffers(Context->m_Device, Context->m_CommandPool, m_NumImages, m_CommandBuffers.data());

		m_Renderpass = VK::VulkanHelper::CreateSimpleRenderPass(Context->m_Device, Context->m_SwapChainSurfaceFormat.format);
		m_FrameBuffers = VK::VulkanHelper::CreateSwapChainFrameBuffers(Context, m_Renderpass, specs.Width, specs.Height);
	}

    { // Record Command Buffers

		VkClearColorValue ClearColor = { 1.0f, 0.0f, 0.0f, 0.0f };
		VkClearValue ClearValue;
		ClearValue.color = ClearColor;

		VkRenderPassBeginInfo RenderPassBeginInfo = 
		{
			.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
			.pNext = NULL,
			.renderPass = m_Renderpass,
			.renderArea = {
				.offset = {
					.x = 0,
					.y = 0
				},
				.extent = {
					.width = specs.Width,
					.height = specs.Height
				}
			},
			.clearValueCount = 1,
			.pClearValues = &ClearValue
		};

		for (uint32_t i = 0; i < m_CommandBuffers.size(); i++)
		{

			VK::VulkanHelper::BeginCommandBuffer(m_CommandBuffers[i], VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);

			RenderPassBeginInfo.framebuffer = m_FrameBuffers[i];

			vkCmdBeginRenderPass(m_CommandBuffers[i], &RenderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
			vkCmdEndRenderPass(m_CommandBuffers[i]);

			VkResult res = vkEndCommandBuffer(m_CommandBuffers[i]);
			VK_CHECK_RESULT(res, "vkEndCommandBuffer");

			/*
			VkImageMemoryBarrier PresentToClearBarrier =
			{
				.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
				.pNext = NULL,
				.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT,
				.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
				.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
				.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
				.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
				.image = Context->m_SwapChainImages[i],
				.subresourceRange = ImageRange
			};

			// Change layout of image to be optimal for presenting
			VkImageMemoryBarrier ClearToPresentBarrier =
			{
				.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
				.pNext = NULL,
				.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
				.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT,
				.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
				.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
				.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
				.image = Context->m_SwapChainImages[i],
				.subresourceRange = ImageRange
			};

				vkCmdPipelineBarrier(m_CommandBuffers[i], VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
				0,  // dependency flags
				0, NULL, // memory barriers
				0, NULL, // buffer memory barriers
				1, &PresentToClearBarrier); // image memory barriers

				vkCmdPipelineBarrier(m_CommandBuffers[i], VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
				0, 0, NULL, 0, NULL, 1, &ClearToPresentBarrier);
		

            VK::VulkanHelper::BeginCommandBuffer(m_CommandBuffers[i], VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);

            vkCmdClearColorImage(m_CommandBuffers[i], Context->m_SwapChainImages[i],
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                &ClearColor, 1, &ImageRange);

            VkResult res = vkEndCommandBuffer(m_CommandBuffers[i]);
            VK_CHECK_RESULT(res, "vkEndCommandBuffer");
			*/

        }

        PIX_DEBUG_INFO("Command buffers recorded");
    }
}

void PixEditor::OnUpdate(float dt)
{
    auto* Context = (VK::VulkanGraphicsContext*)Engine::GetGraphicsContext();

    uint32_t ImageIndex = Context->m_Queue.AcquireNextImage();
    Context->m_Queue.SubmitAsync(m_CommandBuffers[ImageIndex]);
    Context->m_Queue.Present(ImageIndex);
}

void PixEditor::OnDestroy()
{
}

void PixEditor::OnResize(uint32_t width, uint32_t height)
{

}

void PixEditor::OnKeyPressed(uint32_t key)
{
}
