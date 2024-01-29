#pragma once
namespace GGEngine::Graphic
{
    class Samlper
    {
    public:
        enum class Filter { Point, Linear, Anisotropic };
        enum class AddressMode { Border, Clamp, Mirror, Wrap };

        Samlper() = default;
        ~Samlper();

        Samlper(const Samlper&) = delete;
        Samlper& operator =(const Samlper&) = delete;

        void Initialize(Filter filter, AddressMode addressMode);
        void Terminate();

        void BindVS(uint32_t slot)const;
        void BindPS(uint32_t slot)const;

    private:
        ID3D11SamplerState* mSamlper = nullptr;
    };
}