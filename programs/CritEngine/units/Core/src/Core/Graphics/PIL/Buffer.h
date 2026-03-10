#pragma once
#include <string>
#include "../../Core/Base.h"
#include "../../Logging/Logger.h"

namespace Engine
{
    enum class ShaderDataType : uint8_t
    {
        None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
    };

    static uint32_t shaderDataTypeSize(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::None: ASSERT(false, "'None' is an Invalid ShaderDataType.");
            return 0;
        case ShaderDataType::Float: return 4;
        case ShaderDataType::Float2: return 4 * 2;
        case ShaderDataType::Float3: return 4 * 3;
        case ShaderDataType::Float4: return 4 * 4;
        case ShaderDataType::Mat3: return 4 * 3 * 3;
        case ShaderDataType::Mat4: return 4 * 4 * 4;
        case ShaderDataType::Int: return 4 * 1;
        case ShaderDataType::Int2: return 4 * 2;
        case ShaderDataType::Int3: return 4 * 3;
        case ShaderDataType::Int4: return 4 * 4;
        case ShaderDataType::Bool: return 1;
        }

        ASSERT(false, "Unknown ShaderDataType.")
        return 0;
    }


    struct BufferElement
    {
        std::string name;
        uintptr_t offset;
        uint32_t size;
        ShaderDataType type;
        bool normalized;

        BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
            : name(name), offset(0), size(shaderDataTypeSize(type)), type(type), normalized(normalized)
        {
        };

        uint32_t getComponentCount() const
        {
            switch (this->type)
            {
            case ShaderDataType::None: ASSERT(false, "'None' is an Invalid ShaderDataType.");
                return 0;
            case ShaderDataType::Float: return 1;
            case ShaderDataType::Float2: return 2;
            case ShaderDataType::Float3: return 3;
            case ShaderDataType::Float4: return 4;
            case ShaderDataType::Mat3: return 3 * 3;
            case ShaderDataType::Mat4: return 4 * 4;
            case ShaderDataType::Int: return 1;
            case ShaderDataType::Int2: return 2;
            case ShaderDataType::Int3: return 3;
            case ShaderDataType::Int4: return 4;
            case ShaderDataType::Bool: return 1;
            }

            ASSERT(false, "Unknown ShaderDataType.")
            return 0;
        }
    };

    class BufferLayout
    {
    public:
        BufferLayout()
        {
        };

        BufferLayout(const std::initializer_list<BufferElement>& element)
            : m_elements(element)
        {
            calculateOffsetsAndStride();
        }

        BufferLayout(const std::vector<BufferElement>& element)
            : m_elements(element)
        {
            calculateOffsetsAndStride();
        }

        std::vector<BufferElement> getElements() const { return this->m_elements; }
        uint32_t getStride() const { return this->m_stride; }

        std::vector<BufferElement>::iterator begin() { return this->m_elements.begin(); }
        std::vector<BufferElement>::iterator end() { return this->m_elements.end(); }
        std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
        std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }

    private:
        std::vector<BufferElement> m_elements;
        uint32_t m_stride = 0;

        void calculateOffsetsAndStride()
        {
            uint32_t offset = 0;
            this->m_stride = 0;
            for (auto& element : this->m_elements)
            {
                element.offset = offset;
                offset += element.size;
                this->m_stride += element.size;
            }
        }
    };

    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual const BufferLayout& getLayout() const = 0;
        virtual void setLayout(BufferLayout layout) = 0;


        ENGINE_API static std::shared_ptr<VertexBuffer> create(float* vertices, uint32_t size);
    };

    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual uint32_t getCount() const = 0;

        ENGINE_API static std::shared_ptr<IndexBuffer> create(uint32_t* indices, uint32_t size);
    };
}
