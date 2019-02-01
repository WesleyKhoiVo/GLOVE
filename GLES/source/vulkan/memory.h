/**
 * Copyright (C) 2015-2018 Think Silicon S.A. (https://think-silicon.com/)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public v3
 * License as published by the Free Software Foundation;
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 */

/**
 *  @file       memory.h
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      Device Memory Allocation Functionality in Vulkan
 *
 */

#ifndef __VKMEMORY_H__
#define __VKMEMORY_H__

#include <cmath>
#include "utils.h"
#include "context.h"
#include "memoryAllocator.h"

class CacheManager;

namespace vulkanAPI {

class Memory {

protected:

    const
    vkContext_t *                     mVkContext;

    VkDeviceMemory                    mVkMemory;
    const
    VkMemoryMapFlags                  mVkMemoryFlags;
    VkFlags                           mVkFlags;
    VkMemoryRequirements              mVkRequirements;

    CacheManager *                    mCacheManager;

public:
// Constructor
    Memory(const vkContext_t *vkContext = nullptr, VkFlags flags = 0);

// Destructor
    virtual ~Memory();

// Allocate Functions
    virtual bool                      Create(void);

// Release Functions
    virtual void                      Release(void);

// Bind Functions
    virtual bool                      BindBufferMemory(VkBuffer &buffer);
    virtual bool                      BindImageMemory(VkImage &image);

// Get Functions
    VkFlags                           GetFlags(void)                            { FUN_ENTRY(GL_LOG_DEBUG); return mVkFlags; }
    void                              GetImageMemoryRequirements(VkImage &image);
    bool                              GetBufferMemoryRequirements(VkBuffer &buffer);
    VkResult                          GetMemoryTypeIndexFromProperties(uint32_t *typeIndex);
    virtual bool                      GetData(VkDeviceSize size, VkDeviceSize offset, void *data) const;

// Set/Update Functions
    virtual bool                      SetData(VkDeviceSize size, VkDeviceSize offset, const void *data);
    virtual void                      UpdateData(VkDeviceSize size, VkDeviceSize offset, const void *data);
    virtual bool                      FlushData(void)                           { FUN_ENTRY(GL_LOG_DEBUG); return true; }

    inline void                       SetContext(const vkContext_t *vkContext)  { FUN_ENTRY(GL_LOG_TRACE); mVkContext = vkContext; }

    inline void                       SetCacheManager(CacheManager *manager)    { FUN_ENTRY(GL_LOG_TRACE); mCacheManager = manager; }
};

class UniformMemory : public Memory {

private:

    MemoryBlock                       mMemoryBlock;
    uint8_t *                         mSrcData;

public:
// Constructor
    UniformMemory(const vkContext_t *vkContext = nullptr, VkFlags flags = 0);

// Destructor
    virtual ~UniformMemory();

// Allocate Functions
    virtual bool                      Create(void) final;

// Release Functions
    virtual void                      Release(void) final;

// Bind Functions
    virtual bool                      BindBufferMemory(VkBuffer &buffer) final;

// Get Functions
    virtual bool                      GetData(VkDeviceSize size, VkDeviceSize offset, void *data) const final;
    inline uint8_t *                  GetDataPtr(VkDeviceSize offset)                                           { FUN_ENTRY(GL_LOG_TRACE); return mSrcData + offset;}

// Set/Update Functions
    virtual bool                      SetData(VkDeviceSize size, VkDeviceSize offset, const void *data) final;
    virtual bool                      FlushData(void) final;
    
    virtual void                      UpdateData(VkDeviceSize size, VkDeviceSize offset, const void *data) final { FUN_ENTRY(GL_LOG_TRACE);  SetData(size, offset, data); }

};

}

#endif // __VKMEMORY_H__
