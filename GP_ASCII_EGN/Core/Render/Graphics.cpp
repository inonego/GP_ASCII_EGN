#include "Graphics.h"

#include <string>

FrameDrawer::FrameDrawer(FrameBuffer* buffer) {
    this->buffer = buffer;
}

void FrameDrawer::PrintCharacter(Point point, WORD c) {
    buffer->WriteBufferCharacter(point.X, point.Y, c);
}

void FrameDrawer::PrintCharacterColored(Point point, WORD c, WORD color) {
    buffer->SetColor(color);
    buffer->WriteBufferCharacter(point.X, point.Y, c);
    buffer->EndColor();
}

void FrameDrawer::PrintText(Point point, const char str[]) {
    buffer->WriteBufferText(point.X, point.Y, str);
}

void FrameDrawer::PrintText(Point point, TextAlignment textAlignment, size_t size, const char* format, ...) {
    // 문자열을 저장할 버퍼
    char* buffer = new char[size];
    
    if (buffer != nullptr) {
        va_list args;

        va_start(args, format);

        // 포맷에 따라 문자열 작성
        vsnprintf(buffer, size, format, args);

        va_end(args);

        // 텍스트를 출력
        PrintTextAligned(buffer, point, textAlignment);
    }

    delete[] buffer;
}

//문자열을 특정 방향으로 정렬하여 화면에 출력합니다.
void FrameDrawer::PrintTextAligned(const char str[], Point point, TextAlignment textAlignment) {
	int length;

	if (str == NULL) {
		return;
	}

	length = (int)strlen(str) - 1;

	switch (textAlignment) {
	case TEXTALIGNMENT_LEFT:
		buffer->WriteBufferText(point.X, point.Y, str);
		break;
	case TEXTALIGNMENT_CENTER:
		buffer->WriteBufferText(point.X - (length / 2), point.Y, str);
		break;
	case TEXTALIGNMENT_RIGHT:
		buffer->WriteBufferText(point.X - length, point.Y, str);
		break;
	}
}

//문자 c를 반복하여 가로선을 긋습니다.
void FrameDrawer::DrawLineHorizontal(Point point, int length, WORD c) {  
	int x = point.X; 

	while (x < point.X + length)
	{
		x += buffer->WriteBufferCharacter(x, point.Y, c);
	} 
}

//문자 c를 반복하여 세로선을 긋습니다.
void FrameDrawer::DrawLineVertical(Point point, int length, WORD c) {
	int y = point.Y;

	while (y < point.Y + length)
	{
		buffer->WriteBufferCharacter(point.X, y++, c);
	}
} 

//선으로 이루어진 사각형을 그립니다.
void FrameDrawer::DrawBox(Rect rect, bool fill, bool isHighlighted) {
	int i;
	int highlight = ('━' - '─') * isHighlighted;

	if (fill) { 
		for (i = rect.Top + 1; i <= rect.Bottom - 1; i++) {
			DrawLineHorizontal(Point(rect.Left, i), rect.Right - rect.Left + 1, ' ');
		}
	}

	DrawLineHorizontal (Point(rect.Left, rect.Top),     rect.Right - rect.Left + 1, '─' + highlight);
	DrawLineHorizontal (Point(rect.Left, rect.Bottom),  rect.Right - rect.Left + 1, '─' + highlight);
	DrawLineVertical   (Point(rect.Left, rect.Top),     rect.Bottom - rect.Top + 1, '│' + highlight);
	DrawLineVertical   (Point(rect.Right, rect.Top),    rect.Bottom - rect.Top + 1, '│' + highlight);

	buffer->WriteBufferCharacter(rect.Left, rect.Top, '┌' + highlight);
	buffer->WriteBufferCharacter(rect.Right, rect.Top, '┐' + highlight);
	buffer->WriteBufferCharacter(rect.Left, rect.Bottom, '└' + highlight);
	buffer->WriteBufferCharacter(rect.Right, rect.Bottom, '┘' + highlight); 
}  

//문자 c를 반복하여 사각형을 그립니다.
void FrameDrawer::DrawRectangle(Rect rect, WORD c, bool fill) {
	int i; 

	if (fill) {
		for (i = rect.Top + 1; i <= rect.Bottom - 1; i++) {
			DrawLineHorizontal(Point(rect.Left, i), rect.Right - rect.Left + 1, c);
		}
	}

	DrawLineHorizontal (Point(rect.Left, rect.Top),      rect.Right - rect.Left + 1, c);
	DrawLineHorizontal (Point(rect.Left, rect.Bottom),   rect.Right - rect.Left + 1, c);
	DrawLineVertical   (Point(rect.Left, rect.Top),      rect.Bottom - rect.Top + 1, c);
	DrawLineVertical   (Point(rect.Right, rect.Top),     rect.Bottom - rect.Top + 1, c); 
}

//Rect영역에서 중간 점의 위치를 구합니다.
Point FrameDrawer::GetRectCenter(Rect rect) {
	return Point((rect.Left + rect.Right) / 2, (rect.Top + rect.Bottom) / 2);
}