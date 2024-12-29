#pragma once

#include <Windows.h>

enum ColorCode {
    COLOR_A_BLACK       = 0x0000,
    COLOR_F_RED         = 0x0004,
    COLOR_F_GREEN       = 0x0002,
    COLOR_F_BLUE        = 0x0001,
    COLOR_F_WHITE       = 0x0007,
    COLOR_F_INTENSITY   = 0x0008,
    COLOR_B_RED         = 0x0040,
    COLOR_B_GREEN       = 0x0020,
    COLOR_B_BLUE        = 0x0010,
    COLOR_B_WHITE       = 0x0070,
    COLOR_B_INTENSITY   = 0x0080,
};

struct CharInfo {
    WORD character;
    WORD attribute;
    bool isMBCS;
};

enum TextAlignment {
    TEXTALIGNMENT_LEFT,
    TEXTALIGNMENT_CENTER,
    TEXTALIGNMENT_RIGHT
};

struct Point {
    int X;
    int Y;

    Point() : Point(0, 0) {}

    Point(int x, int y) : X(x), Y(y) {}
};

class FrameBuffer {
    friend class Render;

    private:
        int width, height;

        CharInfo* chars;
        char* buffer; 
        WORD* attributes;

        HANDLE hBuffer;

        Point cursor;
        bool blockSpace;
        WORD color;

        const WORD defaultColor = COLOR_F_WHITE;

        FrameBuffer(int width, int height);
        ~FrameBuffer();

        // 버퍼를 초기화합니다.
        void Clear();

        //현재 버퍼를 보여줍니다.
        void Show();

        static void ConvertStringChar2Word(WORD* str, const char src[], size_t size);
        static void ConvertStringCharInfo2Char(char* str, const CharInfo src[], size_t size);

    public:
        int GetWidth() { return width; }
        int GetHeight() { return height; }
        
        WORD ReadBufferCharacter(int x, int y);
        WORD ReadBufferAttribute(int x, int y);

        // 문자 하나를 출력합니다.
        int WriteBufferCharacter(int x, int y, WORD c);

        // 문자열을 출력합니다.
        void WriteBufferText(int x, int y, const char str[]);

        // 특정 위치로 커서를 옮깁니다.
        void SetCursor(Point cursor);
        void EndCursor();

        // 좌표를 멀티바이트 문자 기준(두 칸씩)으로 설정합니다.
        void SetBlockSpace();
        void EndBlockSpace();

        // 색상을 지정합니다.
        void SetColor(WORD _color);
        void EndColor();
};