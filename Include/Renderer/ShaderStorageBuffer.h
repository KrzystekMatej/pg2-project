#pragma once
#include <cstdint>
#include <cstddef>

class ShaderStorageBuffer
{
public:
	static constexpr uint32_t Material = 0;

	ShaderStorageBuffer() = default;
	ShaderStorageBuffer(uint32_t bindingIndex, size_t capacity, size_t stride);
	~ShaderStorageBuffer();

	void Bind() const;
	void BindBase() const;
	void Unbind() const;

	void SetData(const void* data, size_t size, size_t offset = 0);
	void Resize(size_t newCapacity);

	uint32_t GetBufferId() const { return m_Id; }
	size_t GetCapacity() const { return m_Capacity; }
	size_t GetSize() const { return m_Size; }
	size_t GetStride() const { return m_Stride; }

private:
	uint32_t m_Id = 0;
	uint32_t m_BindingIndex = 0;
	size_t m_Capacity = 0;
	size_t m_Size = 0;
	size_t m_Stride = 0;
};
