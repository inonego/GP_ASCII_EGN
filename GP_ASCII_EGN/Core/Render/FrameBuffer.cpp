#include "Graphics.h"

FrameBuffer::FrameBuffer(int width, int height)
{
    this->width = width;
    this->height = height;

    chars = new CharInfo[width * height];
    buffer = new char[width * height * 2 + 1];
    attributes = new WORD[width * height];

    Clear();

    // 콘솔 스크린 버퍼 생성
    COORD size = { width, height };
    SMALL_RECT rect = { 0, 0, width - 1, height - 1 };
    CONSOLE_CURSOR_INFO info;

    hBuffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleScreenBufferSize(hBuffer, size);
    SetConsoleWindowInfo(hBuffer, TRUE, &rect);

    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(hBuffer, &info);
}

FrameBuffer::~FrameBuffer() {
    this->width = 0;
    this->height = 0;

    delete[] chars;
    delete[] buffer;
    delete[] attributes;

    // 콘솔 스크린 버퍼 해제
    CloseHandle(hBuffer);
}

void FrameBuffer::Clear() {
    for (int i = 0; i < width * height; i++) {
        chars[i].character = ' ';
        chars[i].attribute = defaultColor;
        chars[i].isMBCS = false;
    }

    EndCursor();
    EndBlockSpace();
    EndColor();
}

void FrameBuffer::Show() {
    int i;
    DWORD dw1, dw2;
    COORD co = { 0, 0 };

    // 화면에 출력하기 위해 CharInfo 배열을 char형의 문자열로 변환시킵니다.
    ConvertStringCharInfo2Char(buffer, chars, (size_t)width * height);

    for (i = 0; i < width * height; i++) {
        attributes[i] = chars[i].attribute;
    }

    // 콘솔 스크린 버퍼에 출력합니다.
    WriteConsoleOutputCharacterA(hBuffer, buffer, (DWORD)strlen(buffer), co, &dw1);
    WriteConsoleOutputAttribute(hBuffer, attributes, (size_t)width * height, co, &dw2);

    // 콘솔 스크린 버퍼를 활성화시킵니다.
    SetConsoleActiveScreenBuffer(hBuffer);
}

WORD FrameBuffer::ReadBufferCharacter(int x, int y) {
    Point position = { cursor.X + x * (blockSpace + 1), cursor.Y + y };
    return chars[cursor.X + width * cursor.Y].character;
}

WORD FrameBuffer::ReadBufferAttribute(int x, int y) {
    Point position = { cursor.X + x * (blockSpace + 1), cursor.Y + y };
    return attributes[cursor.X + width * cursor.Y];
}

int FrameBuffer::WriteBufferCharacter(int x, int y, WORD c) {
    Point position = { cursor.X + x * (blockSpace + 1), cursor.Y + y };
    int index = position.X + width * position.Y, step;

    if (0 <= position.X && position.X < width) {
        chars[index].character = c;

        if (c <= 0xFF) {
            chars[index].isMBCS = false;
            chars[index].attribute = color;
            step = 1;

        }
        else {
            chars[index].isMBCS = true;
            chars[index].attribute = color;
            chars[index + 1].attribute = color;
            step = 2;
        }

        if (0 <= x - 1) {
            if (chars[index - 1].isMBCS) {
                chars[index - 1].character = ' ';
                chars[index - 1].isMBCS = false;
            }
        }
        if (x + 1 < width) {
            if (chars[index + 1].isMBCS) {
                chars[index + 1].character = ' ';
                chars[index + 1].isMBCS = false;
            }
        }
    }
    else {
        if (c <= 0xFF) {
            step = 1;

        }
        else {
            step = 2;
        }
    }

    if (blockSpace) {
        return 1;
    }
    else {
        return step;
    }
}

void FrameBuffer::WriteBufferText(int x, int y, const char str[]) {

    WORD t = (WORD)'─';
    int i = 0, index = 0;

    WORD* words = new WORD[strlen(str) + 1];

    ConvertStringChar2Word(words, str, strlen(str));

    while (words[i] != 0) {
        index += WriteBufferCharacter(x + index, y, words[i++]);
    }

    delete[] words;
}

void FrameBuffer::SetCursor(Point _cursor) {
    cursor = _cursor;
}

void FrameBuffer::EndCursor() {
    cursor = Point(0, 0);
}

void FrameBuffer::SetBlockSpace() {
    blockSpace = true;
}

void FrameBuffer::EndBlockSpace() {
    blockSpace = false;
}

void FrameBuffer::SetColor(WORD _color) {
    color = _color;
}

void FrameBuffer::EndColor() {
    color = defaultColor;
}

//1byte char형의 문자열을 2byte WORD형의 문자열로 변환시킵니다.
void FrameBuffer::ConvertStringChar2Word(WORD* str, const char src[], size_t size) {
    unsigned int i, index = 0;

    for (i = 0; i < size; i++) {
        if (src[i] >= 0) {
            str[index++] = src[i];
        }
        else {
            str[index++] = (src[i] << 8) | (src[i + 1] << 0) + 0x0100; i++;
        }
    }

    str[index] = '\0';
}

/*
싱글바이트 문자는 1바이트를 차지하고 콘솔에서도 커서를 1칸 이동시킵니다.
특정 문자를 제외한 멀티바이트 문자는 2바이트를 차지하고 콘솔에서도 커서를 2칸 이동시킵니다.

 g_width * g_height 크기의 문자 배열에서 모든 문자를 그대로 출력하게되면 멀티바이트 문자를 출력할 때마다 한 칸씩 밀리는 현상이 발생하게 되므로
 멀티바이트 문자 한 칸 뒤의 문자는 무시하여(코드에서 i++를 이용해 넘어감) 이를 방지합니다.
*/

//CharInfo 배열을 1byte char형의 문자열로 변환시킵니다.
void FrameBuffer::ConvertStringCharInfo2Char(char* str, const CharInfo src[], size_t size) {
    unsigned int i, index = 0;

    for (i = 0; i < size; i++) {
        if (src[i].isMBCS) {
            //멀티바이트 문자인 경우 두 개의 바이트에 나눠 저장합니다.
            str[index++] = (src[i].character >> 8) & 0xFF;
            str[index++] = src[i].character & 0xFF;
            //특정 멀티바이트 문자는 커서를 한 칸만 이동시키므로 반복하거나 공백을 채워 커서를 두 칸 이동시키도록 합니다. 
            if ('─' <= src[i].character && src[i].character <= '─' + 72) {
                str[index++] = ' ';
            }

            i++;
        }
        else {
            //그 외 싱글바이트 문자는 그대로 저장합니다.
            if (src[i].character == '\0') {
                str[index++] = ' ';
            }
            else {
                str[index++] = src[i].character & 0xFF;
            }
        }
    }
    str[index] = '\0';
}