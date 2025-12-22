#pragma once 

#include "../GraphicsEntry.h"

namespace Engine {

	class OpenGLGraphicsEntry : public GraphicsEntry
	{

		OpenGLGraphicsEntry();
		virtual ~OpenGLGraphicsEntry() override {};

		virtual void Init() override;

	};

}