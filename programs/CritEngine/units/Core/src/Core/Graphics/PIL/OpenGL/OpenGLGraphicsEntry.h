#pragma once

#include "../GraphicsEntry.h"

namespace Engine
{
    class OpenGLGraphicsEntry : public GraphicsEntry
    {
        ~OpenGLGraphicsEntry() override = default;

        void init() override;
    };
}
