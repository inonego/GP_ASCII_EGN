#include "Game.h"

#include "Render/Render.h"

void Game::Initialize() {

}

int a = 0;

void Game::Update() {
    FrameDrawer g(Render::GetCurrentBuffer());

    Point center = g.GetRectCenter(Rect(0, 0, g.GetWidth(), g.GetHeight()));
    
    g.DrawBox(Rect(center.X - 15, center.Y - 1, center.X + 15, center.Y + 1), false, false);
    g.PrintText(center, TextAlignment::TEXTALIGNMENT_CENTER, 40, "Hello, %d World!", a++);
}