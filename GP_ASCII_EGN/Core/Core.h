#pragma once

#include "Time.h"
#include "Input.h"

#include "Render/Render.h"

#include "Game.h"

static class Core
{
    private:
        static bool isRunning;
    public:
        static const bool IsRunning() { return isRunning; }

        static void Initialize(LPCWSTR name);

        static void Quit();

        static void Update();

        // 오작동 방지를 위해 콘솔창 사이즈 조정, 최대화, 마우스 선택 기능을 비활성화 시킵니다.
        static void DisableMaximize();
        static void DisableUserSelection();
};