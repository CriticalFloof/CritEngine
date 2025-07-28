#pragma once
#include <vector>
#include <type_traits>

#include "../Base.h"

namespace Reflection {

	enum class FundamentalSemantic : uint8_t
	{
		Value,
		Pointer,
		Reference,
		TemporaryReference
	};

	enum class QualifierSemanticFlags : uint8_t
	{
		Const = 1 << 0,
	    Volatile = 1 << 1,
	};
	inline QualifierSemanticFlags operator|(QualifierSemanticFlags lhs, QualifierSemanticFlags rhs)
	{
		return static_cast<QualifierSemanticFlags>(
			static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs)
			);
	}
	inline QualifierSemanticFlags operator&(QualifierSemanticFlags lhs, QualifierSemanticFlags rhs)
	{
		return static_cast<QualifierSemanticFlags>(
			static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs)
			);
	}

	struct SemanticLayer
	{
		SemanticLayer(FundamentalSemantic fundamental, QualifierSemanticFlags qualifiers = static_cast<QualifierSemanticFlags>(0))
			: fundamental(fundamental), qualifiers(qualifiers) 
		{}

		FundamentalSemantic fundamental;
		QualifierSemanticFlags qualifiers;
	};

	class SemanticStack
	{
	public:
		template<typename T>
		static SemanticStack Create()
		{
			SemanticStack stack = SemanticStack();
			stack.ExtractLayer<T>();
			return stack;
		}

	private:

		template<typename T>
		void ExtractLayer()
		{
			// Extraction
			if constexpr (std::is_lvalue_reference_v<T>)
			{
				ASSERT(this->internalStack.size() == 0, "References can only be at the outermost semantic layer.");

				SemanticLayer layer = SemanticLayer(FundamentalSemantic::Reference);
				this->internalStack.push_back(layer);

				using U = std::remove_reference_t<T>;
				this->ExtractLayer<U>();
			}
			else if constexpr (std::is_rvalue_reference_v<T>)
			{
				ASSERT(this->internalStack.size() == 0, "References can only be at the outermost semantic layer.");

				SemanticLayer layer = SemanticLayer(FundamentalSemantic::TemporaryReference);
				this->internalStack.push_back(layer);

				using U = std::remove_reference_t<T>;
				this->ExtractLayer<U>();
			}
			else if constexpr (std::is_pointer_v<T>)
			{
				SemanticLayer layer = SemanticLayer(FundamentalSemantic::Pointer);
				this->ExtractQualifiers<T>(layer);
				this->internalStack.push_back(layer);

				using U = std::remove_pointer_t<std::remove_cv_t<T>>;
				this->ExtractLayer<U>();
			}
			else if constexpr (std::is_object_v<T> || std::is_void_v<T>)
			{
				SemanticLayer layer = SemanticLayer(FundamentalSemantic::Value);
				this->ExtractQualifiers<T>(layer);
				this->internalStack.push_back(layer);

				// If using value semantics we no longer recurse
			}
			else
			{
				ASSERT(false, "Encountered type that doesn't fit into any known semantic category.")
			}
		}

		template<typename T>
		void ExtractQualifiers(SemanticLayer& layer)
		{
			if constexpr (std::is_const_v<T>)
			{
				layer.qualifiers = layer.qualifiers | QualifierSemanticFlags::Const;
			}
			if constexpr (std::is_volatile_v<T>)
			{
				layer.qualifiers = layer.qualifiers | QualifierSemanticFlags::Volatile;
			}
		}

		std::vector<SemanticLayer> internalStack;
	};

}