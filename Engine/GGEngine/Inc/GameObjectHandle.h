#pragma once
namespace GGEngine
{
    class GameWorld;

    class GameObjectHandle
    {
    public:
        GameObjectHandle() = default;

    private:
        friend class GameWorld;

        int mIdx = -1;
        int mGeneration = -1;
    };
}