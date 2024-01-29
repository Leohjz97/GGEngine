#pragma once
namespace GGEngine::Graphic
{
	class ConstantBuffer
	{
	public:
		ConstantBuffer() = default;
		virtual ~ConstantBuffer();

		void Initialize(uint32_t bufferSize);
		void Terminate();

		void Update(const void* data) const;

		void BindVS(uint32_t slot) const;
		void BindPS(uint32_t slot) const;

	private:
		ID3D11Buffer* mConstantBuffer = nullptr;
	};

	template<class Datatype>
	class TypedConstantBuffer final : public ConstantBuffer
	{
	public:
		void Initialize()
		{
			static_assert(sizeof(Datatype) % 16 == 0, "Data type mush be 16 byte aligned");
			ConstantBuffer::Initialize(sizeof(Datatype));
		}

		void Update(const Datatype& data) const
		{
			ConstantBuffer::Update(&data);
		}
	};
}
