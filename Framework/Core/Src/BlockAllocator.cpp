#include"Precompiled.h"
#include"BlockAllocator.h"

#include"DebugUtil.h"

using namespace GGEngine;
using namespace GGEngine::Core;

BlockAllocator::BlockAllocator(const char* name, size_t blocksize, size_t cap)
    :mName(name), mBlockSize(blocksize),mCap(cap),mFreeBlocks(cap)
{
    ASSERT(blocksize > 0, "BlockAllocator: invalid block Size");
    ASSERT(cap > 0, "BlockAllocator: invalid capacity");

    mData = std::malloc(blocksize * cap);

    for (size_t i = 0; i < cap; ++i)
    {
        mFreeBlocks[i] = static_cast<uint8_t*>(mData) + (i * mBlockSize);
    }

    LOG("%s allocated %zu blocks, blocks size:%zu", mName.c_str(), cap, blocksize);
}

BlockAllocator::~BlockAllocator()
{
    ASSERT(mFreeBlocks.size() == mCap, "BlockAllocator: not all blocks are freed, potential memory leak");
    ASSERT(mBlockAllocatorTotal == mBlockFreed, "BlockAllocator: not all blocks are freed, potential memory leak");
    std::free(mData);

    LOG("%s destructed. Allocated:%zu, freed:%zu, HighestCout:%zu", mName.c_str(), mBlockAllocatorCurrent, mBlockFreed, mBlockHighest);
}

void* BlockAllocator::Allocate()
{
    if (mFreeBlocks.empty())
    {
        LOG("%s no free blocks avaoable!", mName.c_str());
        return nullptr;
    }
    
    void* freeBlock = mFreeBlocks.back();
    mFreeBlocks.pop_back();

    ++mBlockAllocatorTotal;
    ++mBlockAllocatorCurrent;
    mBlockHighest = std::max(mBlockHighest, mBlockAllocatorCurrent);

    LOG("%s allocated blocks at %p, Allocated:%zu, HighestCount: %zu",
        mName.c_str(),
        freeBlock,
        mBlockAllocatorCurrent,
        mBlockHighest);

    return freeBlock;
}

void BlockAllocator::Free(void* ptr)
{
    if (ptr == nullptr)
    {
        return;
    }

    const auto start = static_cast<uint8_t*>(mData);
    const auto end = static_cast<uint8_t*>(mData) + (mBlockSize * mCap);
    const auto current = static_cast<uint8_t*>(ptr);
    const auto diff = current - start;
    ASSERT(current >= start && current < end&& static_cast<size_t>(diff) % mBlockSize == 0,
        "BlockAllocator: invalid address being freed!");

    LOG("%s free %p", mName.c_str(), ptr);
    --mBlockAllocatorCurrent;
    ++mBlockFreed;
    mFreeBlocks.emplace_back(ptr);
}
