#include "Core/Core.h"

int main()
{
    Core::Initialize(L"GP_ASCII_EGN");

    while (Core::IsRunning())
    {
        Core::Update();
    }

    return 0;
}