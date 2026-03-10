#pragma once
#include <vector>
#include <type_traits>

namespace Engine
{
    template <typename T>
    class SparseSet
    {
    public:
        SparseSet()
        {
            static_assert(std::is_integral_v<T>, "Type T must be integral");
        }

        void insert(T entry)
        {
            this->m_sparse[entry] = this->m_packed.size();
            this->m_packed.push_back(entry);
        }

        void remove(T entry)
        {
            this->m_packed[this->m_sparse[entry]] = this->m_packed.back();
            m_sparse[this->m_packed.back()] = this->m_sparse[entry];

            m_packed.pop_back();
            m_sparse[entry] = static_cast<T>(UINT64_MAX);
        }

        bool contains(T entry)
        {
            return this->m_packed.size() > 0 && this->m_packed[this->m_sparse[entry]] == entry;
        }

        T get(T entry)
        {
            if (this->m_packed[this->m_sparse[entry]] == entry)
            {
                return this->m_sparse[entry];
            }
            return static_cast<T>(UINT64_MAX);
        }

        std::vector<T>::size_type size()
        {
            return this->m_packed.size();
        }

        void Clear()
        {
            for (T i = this->m_packed.size() - 1; i >= 0; --i)
            {
                this->m_sparse[this->m_packed[i]] = static_cast<T>(UINT64_MAX);
                this->m_packed.pop_back();
            }
        }

    private:
        std::vector<T> m_sparse;
        std::vector<T> m_packed;
    };
}
