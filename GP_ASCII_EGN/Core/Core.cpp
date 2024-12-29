#include "Core.h"

#include <Windows.h>

bool Core::isRunning = false;

Game* Core::game = nullptr;

void Core::Initialize(LPCWSTR name)
{
    DisableMaximize();
    DisableUserSelection();

    SetConsoleTitle(name);

    isRunning = true;

    Time::Initialize();
    Input::Initialize();
    Render::Initialize();
    
    game = new Game();

    game->Initialize();
}

Core::~Core()
{
    delete game;
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

        FrameBuffer* buffer = Render::GetCurrentBuffer();

        // 게임 업데이트
        game->Update();
        game->Render(*buffer);

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

