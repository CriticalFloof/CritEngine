#pragma once

#include <stdint.h>

namespace Engine {

	enum GraphicsFeatureSupport : uint8_t
	{
		Unsupported,
		Emulated,
		Available
	};

	class GraphicsFeature
	{
	public:
		GraphicsFeature(GraphicsFeatureSupport supportLevel = GraphicsFeatureSupport::Unsupported);

		GraphicsFeatureSupport GetSupportLevel();
	private:
		GraphicsFeatureSupport supportLevel;
	};
}