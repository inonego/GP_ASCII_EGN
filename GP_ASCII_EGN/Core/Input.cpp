#include "H/Input.h"

#include <Windows.h>

#define KEY_NONE 0x0000
#define KEY_PRESSED_NOW 0x8000
#define KEY_PRESSED_PREVIOUS 0x0001

void Input::Initialize()
{

}

//키보드 입력 상태를 체크합니다.
void Input::Update()
{
    // TODO
}

//이전 업데이트 주기부터 계속 키가 눌리고 있는지 확인합니다.
bool Input::GetKey(int key) { 
    return GetAsyncKeyState(key) != KEY_NONE;
}

//이번 업데이트 주기에서 키가 눌렸는지 확인합니다.
bool Input::GetKeyDown(int key) { 
    return GetAsyncKeyState(key) & (KEY_NONE || KEY_PRESSED_NOW);
}

//이번 업데이트 주기에서 키를 놓았는지 확인합니다.
bool Input::GetKeyUp(int key) {
    return !GetAsyncKeyState(key) & (KEY_PRESSED_PREVIOUS || KEY_NONE);
}