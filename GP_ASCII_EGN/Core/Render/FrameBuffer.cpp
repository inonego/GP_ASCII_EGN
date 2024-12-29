#include "Graphics.h"

FrameBuffer::FrameBuffer(int width, int height)
{
    this->width = width;
    this->height = height;

    chars = new CharInfo[width * height];
    buffer = new char[width * height * 2 + 1];
    attributes = new WORD[width * height];

    Clear();

    // �ܼ� ��ũ�� ���� ����
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

    // �ܼ� ��ũ�� ���� ����
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

    // ȭ�鿡 ����ϱ� ���� CharInfo �迭�� char���� ���ڿ��� ��ȯ��ŵ�ϴ�.
    ConvertStringCharInfo2Char(buffer, chars, (size_t)width * height);

    for (i = 0; i < width * height; i++) {
        attributes[i] = chars[i].attribute;
    }

    // �ܼ� ��ũ�� ���ۿ� ����մϴ�.
    WriteConsoleOutputCharacterA(hBuffer, buffer, (DWORD)strlen(buffer), co, &dw1);
    WriteConsoleOutputAttribute(hBuffer, attributes, (size_t)width * height, co, &dw2);

    // �ܼ� ��ũ�� ���۸� Ȱ��ȭ��ŵ�ϴ�.
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

    WORD t = (WORD)'��';
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

//1byte char���� ���ڿ��� 2byte WORD���� ���ڿ��� ��ȯ��ŵ�ϴ�.
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
�̱۹���Ʈ ���ڴ� 1����Ʈ�� �����ϰ� �ֿܼ����� Ŀ���� 1ĭ �̵���ŵ�ϴ�.
Ư�� ���ڸ� ������ ��Ƽ����Ʈ ���ڴ� 2����Ʈ�� �����ϰ� �ֿܼ����� Ŀ���� 2ĭ �̵���ŵ�ϴ�.

 g_width * g_height ũ���� ���� �迭���� ��� ���ڸ� �״�� ����ϰԵǸ� ��Ƽ����Ʈ ���ڸ� ����� ������ �� ĭ�� �и��� ������ �߻��ϰ� �ǹǷ�
 ��Ƽ����Ʈ ���� �� ĭ ���� ���ڴ� �����Ͽ�(�ڵ忡�� i++�� �̿��� �Ѿ) �̸� �����մϴ�.
*/

//CharInfo �迭�� 1byte char���� ���ڿ��� ��ȯ��ŵ�ϴ�.
void FrameBuffer::ConvertStringCharInfo2Char(char* str, const CharInfo src[], size_t size) {
    unsigned int i, index = 0;

    for (i = 0; i < size; i++) {
        if (src[i].isMBCS) {
            //��Ƽ����Ʈ ������ ��� �� ���� ����Ʈ�� ���� �����մϴ�.
            str[index++] = (src[i].character >> 8) & 0xFF;
            str[index++] = src[i].character & 0xFF;
            //Ư�� ��Ƽ����Ʈ ���ڴ� Ŀ���� �� ĭ�� �̵���Ű�Ƿ� �ݺ��ϰų� ������ ä�� Ŀ���� �� ĭ �̵���Ű���� �մϴ�. 
            if ('��' <= src[i].character && src[i].character <= '��' + 72) {
                str[index++] = ' ';
            }

            i++;
        }
        else {
            //�� �� �̱۹���Ʈ ���ڴ� �״�� �����մϴ�.
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