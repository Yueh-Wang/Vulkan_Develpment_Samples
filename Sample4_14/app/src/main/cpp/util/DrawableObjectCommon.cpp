#include "DrawableObjectCommon.h"
#include "mylog.h"
#include <assert.h>
#include "HelpFunction.h"
#include "MatrixState3D.h"
#include <string.h>

DrawableObjectCommonLight::DrawableObjectCommonLight(float* vdataIn,int dataByteCount,int vCountIn,VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties)
{
    this->devicePointer=&device;
    this->vdata=vdataIn;
    this->vCount=vCountIn;

    VkBufferCreateInfo buf_info = {};
    buf_info.sType =VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buf_info.pNext = NULL;
    buf_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    buf_info.size = dataByteCount;
    buf_info.queueFamilyIndexCount = 0;
    buf_info.pQueueFamilyIndices = NULL;
    buf_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    buf_info.flags = 0;

    VkResult result = vk::vkCreateBuffer(device,&buf_info, NULL, &vertexDatabuf);
    assert(result == VK_SUCCESS);

    VkMemoryRequirements mem_reqs;
    vk::vkGetBufferMemoryRequirements(device, vertexDatabuf, &mem_reqs);
    assert(dataByteCount<=mem_reqs.size);

    VkMemoryAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.pNext = NULL;
    alloc_info.memoryTypeIndex = 0;
    alloc_info.allocationSize = mem_reqs.size;

    VkFlags requirements_mask=VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT| VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    bool flag=memoryTypeFromProperties(memoryroperties,mem_reqs.memoryTypeBits,requirements_mask,&alloc_info.memoryTypeIndex);
    if(flag){
        LOGE("确定内存类型成功 类型索引为%d",alloc_info.memoryTypeIndex);
    }else{
        LOGE("确定内存类型失败!");
    }
    result = vk::vkAllocateMemory(device,&alloc_info, NULL, &vertexDataMem);

    assert(result == VK_SUCCESS);
    uint8_t *pData;
    result = vk::vkMapMemory(device, vertexDataMem, 0, mem_reqs.size, 0, (void **)&pData);
    assert(result == VK_SUCCESS);
    memcpy(pData, vdata, dataByteCount);
    vk::vkUnmapMemory(device, vertexDataMem);
    result = vk::vkBindBufferMemory(device,vertexDatabuf, vertexDataMem, 0);

    assert(result == VK_SUCCESS);
    vertexDataBufferInfo.buffer = vertexDatabuf;
    vertexDataBufferInfo.offset = 0;
    vertexDataBufferInfo.range = mem_reqs.size;
}

DrawableObjectCommonLight::~DrawableObjectCommonLight()
{
    delete vdata;
    vk::vkDestroyBuffer(*devicePointer, vertexDatabuf, NULL);
    vk::vkFreeMemory(*devicePointer, vertexDataMem, NULL);

}

void DrawableObjectCommonLight::drawSelf(VkCommandBuffer& cmd,VkPipelineLayout& pipelineLayout,VkPipeline& pipeline,VkDescriptorSet* desSetPointer)
{
    vk::vkCmdBindPipeline(cmd,VK_PIPELINE_BIND_POINT_GRAPHICS,pipeline);
    vk::vkCmdBindDescriptorSets(cmd,VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1,desSetPointer, 0, NULL);
    const VkDeviceSize offsetsVertex[1] = {0};
    vk::vkCmdBindVertexBuffers(
            cmd,				
            0,					
            1,					
            &(vertexDatabuf),	
            offsetsVertex		
    );
    vk::vkCmdDraw(cmd, vCount, 1, 0, 0);

}