#pragma once
namespace GGEngine::Core
{
    class BlockAllocator
    {
    public: 
        BlockAllocator(const char* name,size_t blocksize, size_t cap);
        virtual ~BlockAllocator();

        BlockAllocator(const BlockAllocator&) = delete;
        BlockAllocator(const BlockAllocator&&) = delete;
        BlockAllocator operator=(const BlockAllocator&) = delete;
        BlockAllocator operator=(const BlockAllocator&&) = delete;

        void* Allocate();
        void Free(void* ptr);

    private:
        std::string mName;
        std::vector<void*> mFreeBlocks;
        
        void* mData = nullptr;
        size_t mBlockSize = 0;
        size_t mCap = 0;
        size_t mBlockAllocatorCurrent = 0;
        size_t mBlockAllocatorTotal = 0;
        size_t mBlockFreed = 0;
        size_t mBlockHighest = 0;
    };
}