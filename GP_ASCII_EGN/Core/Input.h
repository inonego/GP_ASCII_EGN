#pragma once

static class Input
{
    private:
        friend class Core;
        
        static void Initialize();

        static void Update();

    public:
        static bool GetKey(int key);
        static bool GetKeyDown(int key);
        static bool GetKeyUp(int key);
};