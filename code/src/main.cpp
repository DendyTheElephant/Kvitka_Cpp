
//import <iostream>;

#include <DendyEngine/EngineCore.h>
#include <DendyCommon/Logger.h>
#include <DendyCommon/Timer.h>

#include <DendyEngine/ECS/ECSEngine.h>


#include <thread>
#include <iostream>


int main()
{
    if (__cplusplus > 202101L) std::cout << "C++23("<<__cplusplus<<"L)";
    else if (__cplusplus > 202002L) std::cout << "C++20("<<__cplusplus<<"L)";
    else if (__cplusplus > 201703L) std::cout << "C++17("<<__cplusplus<<"L)";
    else if (__cplusplus > 201402L) std::cout << "C++14("<<__cplusplus<<"L)";
    else if (__cplusplus > 201103L) std::cout << "C++11("<<__cplusplus<<"L)";
    else if (__cplusplus > 199711L) std::cout << "C++98("<<__cplusplus<<"L)";
    else std::cout << "pre-standard C++." << __cplusplus;
    std::cout << std::endl;

    DendyCommon::CLogger* pLogger = &DendyCommon::CLogger::GetInstance();
    pLogger->SetOutputFile("LogError.txt");


    bool IsInDebug = true;

    DendyCommon::CTimer GlobalUpdaterPerfTimer;
    DendyCommon::CTimer DeltaTimeTimer;
    //DendyCommon::CTimer RealTimer;
    int Counter = 0;

    //system("pause>nul");
    float DeltaTimeMs = 0.0f;

    GlobalUpdaterPerfTimer.Reset();
    DeltaTimeTimer.Reset();

    DendyEngine::CEngineCore* pEngine = new DendyEngine::CEngineCore(IsInDebug);
    while(pEngine->GetRunningState())
    {
        
        DeltaTimeTimer.Pause();
        //std::this_thread::sleep_for(std::chrono::milliseconds(Timer.GetElapsedMiliseconds()));
        DeltaTimeMs = DeltaTimeTimer.GetElapsedNanoseconds() / 1000000.0f;
        DeltaTimeTimer.Reset();

        //RealTimer.Reset();
        pEngine->Update(DeltaTimeMs);
        Counter++;
        
        

        if (Counter == 10)
        {
            GlobalUpdaterPerfTimer.Pause();
            
            std::string Caption;
            Caption += std::string("Global Update: Elapsed ");
            float ElapsedMiliseconds = GlobalUpdaterPerfTimer.GetElapsedNanoseconds() / 1000000.0f;
            Caption += std::string( std::to_string( ElapsedMiliseconds / (float)Counter ));
            Caption += std::string("ms, fps: ");
            Caption += std::string( std::to_string(1.0f/ (ElapsedMiliseconds/1000.0f /(float)Counter) ));
            pEngine->SetWindowCaption( Caption );

            Counter = 0;
            GlobalUpdaterPerfTimer.Reset();
            
            //std::cout << "Global Update: Elapsed " << (Timer.GetElapsedNanoseconds()/1000000.0f) << "ms, fps: " << (1.0f/(float)Timer.GetElapsedNanoseconds()*1000000000.0f) << std::endl;
            //RealTimer.Pause();
            //std::cout << "Global Real Update: Elapsed " << (RealTimer.GetElapsedNanoseconds()/1000000.0f) << "ms, fps: " << (1.0f/(float)RealTimer.GetElapsedNanoseconds()*1000000000.0f) << std::endl;
            //system("pause>nul");

        }
        
    }
    
    delete pEngine;

    
    DendyCommon::CLogger::DestroyInstance();

    //system("pause>nul");6

    return 0;
}