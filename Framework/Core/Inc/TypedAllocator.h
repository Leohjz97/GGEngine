#pragma once
#include"BlockAllocator.h"

namespace GGEngine::Core
{
    template<class DataType>
    class TypedAllocter : private BlockAllocator
    {
    public:
        TypedAllocter(const char* name, size_t cap)
            :BlockAllocator(name, sizeof(DataType), cap)
        {
        }

        template<class...Args>
        DataType* New(Args&&...args)
        {
            auto* instance = static_cast<DataType*>(Allocate());
            //Placement new, calls constructor on a desginedted location
            new(instance) DataType(std::forward<Args>(args)...);

            return instance;
        }

        void Delete(DataType* ptr)
        {
            if (ptr == nullptr)
            {
                return;
            }

            ptr->~DataType();
            Free(ptr);
        }
    };
}