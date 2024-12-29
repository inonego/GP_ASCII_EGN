#pragma once

#include "Render/Graphics.h"

class GameObject
{
    public:
        bool isActive = true;

        Point position = Point(0, 0);

        virtual void Update() { };
        virtual void Render(FrameDrawer& g) { };
};