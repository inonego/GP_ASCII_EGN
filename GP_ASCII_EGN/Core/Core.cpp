#include "Core.h"

#include <Windows.h>

bool Core::isRunning = false;

void Core::Initialize(LPCWSTR name)
{
    DisableMaximize();
    DisableUserSelection();

    SetConsoleTitle(name);

    isRunning = true;

    Time::Initialize();
    Input::Initialize();
    Render::Initialize();
    
    Game::Initialize();
}

void Core::Quit()
{
    isRunning = false;
}

void Core::Update()
{
    Time::AddTick();

    if (Time::CheckUpdate())
    {
        // 입력 처리
        Input::Update();

        // 게임 업데이트
        Game::Update();

        // 렌더링
        Render::Update();
    }
}

void Core::DisableMaximize() {
    HWND hwnd = GetConsoleWindow();

    SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
}

void Core::DisableUserSelection() {
    DWORD prev_mode;

    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);

    GetConsoleMode(hInput, &prev_mode);
    SetConsoleMode(hInput, prev_mode & ~ENABLE_QUICK_EDIT_MODE);
}

