#pragma once

#include "Graphics.h"

#define MAX_BUFFER_COUNT 2

static class Render
{
    friend class Core;

    private:
        static int width, height;

        static int currentBufferIndex;

        static FrameBuffer* buffer[MAX_BUFFER_COUNT];

        static void Initialize();
        
        ~Render();
        
        static void Update();

        static void ShowBuffer();
        static void FlipBuffer();
        static void ClearBuffer();

    public:
        static int GetWidth()  { return width; }
        static int GetHeight() { return height; }

        static FrameBuffer* GetCurrentBuffer() { return buffer[currentBufferIndex]; }
};