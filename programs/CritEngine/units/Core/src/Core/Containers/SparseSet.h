#pragma once
#include <vector>
#include <type_traits>

namespace Engine {

	template<typename T>
	class SparseSet
	{
	public:
		SparseSet()
		{
			static_assert(std::is_integral_v<T>, "Type T must be integral");
		}

		void Insert(T entry)
		{
			this->sparse[entry] = this->packed.size();
			this->packed.push_back(entry);
		}

		void Remove(T entry)
		{
			this->packed[this->sparse[entry]] = this->packed.back();
			sparse[this->packed.back()] = this->sparse[entry];

			packed.pop_back();
			sparse[entry] = static_cast<T>(UINT64_MAX);
		}

		bool Contains(T entry)
		{
			return this->packed.size() > 0 && this->packed[this->sparse[entry]] == entry;
		}

		T Get(T entry)
		{
			if (this->packed[this->sparse[entry]] == entry)
			{
				return this->sparse[entry];
			}
			else
			{
				return static_cast<T>(UINT64_MAX);
			}
		}

		std::vector<T>::size_type Size()
		{
			return this->packed.size();
		}

		void Clear()
		{
			for (T i = this->packed.size() - 1; i >= 0; i--)
			{
				this->sparse[this->packed[i]] = static_cast<T>(UINT64_MAX);
				this->packed.pop_back();
			}
		}

	private:
		std::vector<T> sparse;
		std::vector<T> packed;
	};
}