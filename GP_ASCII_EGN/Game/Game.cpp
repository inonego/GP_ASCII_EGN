#include "Game.h"

#include "Render/Render.h"

void GameScene::Initialize() {
    
}

void GameScene::Update() {
    for (GameObject* obj : gameObjects){
        obj->Update();
    }
}

void GameScene::Render(FrameDrawer& g) {
    for (GameObject* obj : gameObjects){
        obj->Render(g);
    }
}

#include "GUI/GUI.h"

void Game::Initialize() {
    currentScene = new GameScene();

    if (currentScene != nullptr) currentScene->Initialize();

    TextField* textField = new TextField("Hello, World!");
    textField->position = Point(10, 10);
    textField->isSelected = true;

    currentScene->gameObjects.push_back(textField);

    CheckBox* checkBox = new CheckBox();
    checkBox->position = Point(10, 20);
    checkBox->isSelected = true;

    currentScene->gameObjects.push_back(checkBox);

    Button* button = new Button();
    button->position = Point(10, 30);
    button->text = "Click";
    button->width = 10;
    button->height = 5;

    button->isSelected = true;

    currentScene->gameObjects.push_back(button);
}

void Game::Update() {
    if (currentScene != nullptr) currentScene->Update();
}

void Game::Render(FrameBuffer& buffer) {
    FrameDrawer g(&buffer);

    if (currentScene != nullptr) currentScene->Render(g);
}