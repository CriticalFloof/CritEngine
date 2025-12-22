#pragma once

namespace ECS {

    class Context;

    class System {
    public:
        System() {};
        virtual ~System() {};

        void SetContext(Context* scene)
        {
            this->context = scene;
        }
        virtual void Update() = 0;

        Context* context = nullptr;
    };

}