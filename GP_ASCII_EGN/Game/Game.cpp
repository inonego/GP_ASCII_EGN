#include "Game.h"

#include "Render/Render.h"

void Game::Initialize() {

}

void Game::Update() {
	Render::GetCurrentBuffer()->SetColor(ColorCode::COLOR_F_WHITE | ColorCode::COLOR_B_BLUE);
    Render::GetCurrentBuffer()->WriteBufferText(0, 0, "Hello, World!");
	Render::GetCurrentBuffer()->EndColor();
}