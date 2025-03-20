#pragma once
#include <cstdint>

class IndexBuffer
{
public:
    IndexBuffer() = default;
    ~IndexBuffer();
    IndexBuffer(const void* data, uint32_t count);

    void Bind() const;
    void UnBind() const;
    uint32_t GetCount() const { return m_Count; }
private:
    uint32_t m_IBO;
    uint32_t m_Count;
};