#pragma once

#include "GameObject.h"

#include <vector>

using namespace std;

class GameScene
{
    public:
        vector<GameObject*> gameObjects;

        virtual void Initialize();
        virtual void Update();
        virtual void Render(FrameDrawer& g);
};

class Game
{
    public:
        GameScene* currentScene;

        void Initialize();
        void Update();
        void Render(FrameBuffer& buffer);
};