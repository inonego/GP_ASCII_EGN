#pragma once

static class Time
{
    friend class Core;

    private:
        static float updateTimeElapsed;

        static float fps;

        //정밀한 시간 측정을 위한 변수
        static long long frequency;
        static long long prevTime;

        //업데이트 주기를 설정합니다.
        static const float updateTime;

        //마지막 업데이트 이후로 지난 시간입니다.
        //외부에서는 DeltaTime()를 통해 값을 얻을 수 있습니다.
        static float deltaTime; 
        //게임(프로그램) 시작 이후로 지난 시간입니다.
        //외부에서는 InGameTime()를 통해 값을 얻을 수 있습니다.
        static float inGameTime;

        #pragma region Core 초기화 및 업데이트

        static void Initialize();
        
        static bool CheckUpdate();

        #pragma endregion

        static void AddTick();

        static void CheckFPS();

    public:
        //시간 스케일(배속)을 설정합니다.
        static float scale;

        static float DeltaTime()  { return deltaTime;  } 
        static float InGameTime() { return inGameTime; }
        static float FPS() { return fps; }
};