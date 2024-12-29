#include "Time.h"

#include <Windows.h>

//목표 초당 프레임 수 입니다.
#define TARGET_FPS 60.0f

float Time::updateTimeElapsed = 0.0f;

float Time::fps = 0.0f;

long long Time::frequency = 0;
long long Time::prevTime = 0;

const float Time::updateTime = 1.0f / TARGET_FPS;

float Time::deltaTime = 0.0f;
float Time::inGameTime = 0.0f;

void Time::Initialize()
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	QueryPerformanceCounter((LARGE_INTEGER*)&prevTime);
}

//업데이트 주기가 되었는지 확인합니다.
bool Time::CheckUpdate()
{
	//지난 업데이트 이후로 임계 값 이상의 시간이 지나면 true를 반환합니다.
	if (updateTimeElapsed >= updateTime) {
		deltaTime = updateTimeElapsed; 
        
        updateTimeElapsed = 0.0f;

		CheckFPS();

		return true;
	}
	else 
    {
		return false;
	}
}

//프로그램 한 번의 Cycle에 걸린 시간을 측정합니다.
void Time::AddTick()
{
	long long t; QueryPerformanceCounter((LARGE_INTEGER*)&t);
    
	float tick = (float)(t - prevTime) / frequency;

	prevTime = t; 

    // 게임 시간 누적
	inGameTime += tick;

    // 업데이트 시간 누적
    updateTimeElapsed += tick;
} 

// FPS(초당 프레임수)를 체크합니다.
void Time::CheckFPS()
{
	static const float checkTime  = 1.0f;
	static float checkTimeElasped = 0.0f;

	static int count = 0;

	checkTimeElasped += deltaTime;

	count++;

	if (checkTimeElasped >= checkTime)
    {
        fps = count / checkTime;

		checkTimeElasped = 0.0f;

		count = 0;
	}
}
