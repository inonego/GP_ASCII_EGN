#pragma once

#include "GameObject.h"
#include "Input.h"

#include <conio.h>

#include <string>

using namespace std;

class GUI : public GameObject
{
    public:
        bool isSelected = false;
        bool isEnabled = true;
};

class CheckBox : public GUI {
    public:
        bool isChecked = false;

        void (*OnValueChanged)() = nullptr;

        void Update() override {
            if (isSelected) {
                if(Input::GetKeyDown(VK_RETURN)) {
                    SetValue(!isChecked);
                }
            }
        }

        void SetValue(bool value) {
            isChecked = value;

            if (OnValueChanged != nullptr) OnValueChanged();
        }

        void Render(FrameDrawer& g) override {
            g.PrintCharacter(position, isChecked ? '▣' : '□');
        }
};

class Button : public GUI {
    public:
        bool isSelected = false;

        string text;

        int width = 0;
        int height = 0;

        void (*OnClick)() = nullptr;

        Rect GetRect() {
            return Rect(position.X - width / 2, position.Y - height / 2, position.X + width / 2, position.Y + height / 2);
        }

        void Update() override {
            if (isSelected) {
                if(Input::GetKeyDown(VK_RETURN)) {
                    if (OnClick != nullptr) OnClick();
                }
            }
        }

        void Render(FrameDrawer& g) override {
            g.SetColor(COLOR_F_WHITE | (COLOR_B_RED) * isSelected);
            g.DrawBox(GetRect(), true, isSelected);
            g.PrintTextAligned(text.c_str(), position, TEXTALIGNMENT_CENTER); 
            g.EndColor();
        }
};

class TextField : public GUI {
    private:
        int cursor = 0;

        void SetCursor(int cursor) {
            this->cursor = min(max(0, cursor), text.length());
        }

    public:
        TextField(string text = "") : text(text), cursor(text.length()) {}

        string text;

        WORD textColor = COLOR_F_WHITE;
        WORD backgroundColor = COLOR_A_BLACK;

        WORD cursorTextColor = COLOR_A_BLACK;
        WORD cursorBackgroundColor = COLOR_B_WHITE;

        void Update() override {
            if (isSelected) {
                while (_kbhit()) {
                    int key = _getch();
                    
                    if (key > 127) {
                        key = _getch();
                    }
                    else
                    // 백스페이스 키는 무시합니다.
                    if (key == '\b')
                    {
                        continue;
                    }
                    else
                    //
                    if (key == 27)
                    {
                        continue;
                    }
                    else text.insert(cursor++, 1, (char)key);
                }

                if (Input::GetKeyDown(VK_LEFT)) {
                    SetCursor(cursor - 1);
                }
                
                if (Input::GetKeyDown(VK_RIGHT)) {
                    SetCursor(cursor + 1);
                }

                if (Input::GetKeyDown(VK_BACK)) {
                    if (cursor > 0) {
                        text.erase(cursor - 1, 1);

                        SetCursor(cursor - 1);
                    }
                }

                if (Input::GetKeyDown(VK_ESCAPE)) {
                    text.clear();

                    SetCursor(0);
                }

                if (Input::GetKeyDown(VK_DELETE)) {
                    if (cursor < text.length()) {
                        text.erase(cursor, 1);
                    }
                }
            }
        }

        void Render(FrameDrawer& g) override {
            g.SetColor(textColor | backgroundColor);
            g.PrintText(position, text.c_str());
            g.EndColor();

            g.PrintCharacterColored(Point(position.X + cursor, position.Y), text[cursor], cursorTextColor | cursorBackgroundColor);
        }
};
