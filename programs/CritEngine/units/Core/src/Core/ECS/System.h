#pragma once

namespace ECS
{
    class Context;

    class System
    {
    public:
        System()
        {
        };

        virtual ~System()
        {
        };

        void setContext(Context* scene)
        {
            this->context = scene;
        }

        virtual void update() = 0;

        Context* context = nullptr;
    };
}
