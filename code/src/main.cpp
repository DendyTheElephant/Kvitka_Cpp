#include <iostream>
//import <iostream>;

#include <DendyEngine/EngineCore.h>
#include <DendyCommon/Logger.h>
#include <DendyCommon/Timer.h>

#include <DendyEngine/ECS/ECSEngine.h>





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
    pLogger->SetOutputFile("G:\\DyCode\\Kvitka_Cpp\\LogError.txt");


    DendyEngine::ECS::CECSEngine* pECSEngine = new DendyEngine::ECS::CECSEngine();

    DendyEngine::ECS::CGameObject* pGameObject = pECSEngine->AddGameObject("Kossack001");
    DendyEngine::ECS::STransform* pTransform = pECSEngine->AddComponent<DendyEngine::ECS::STransform>(pGameObject);
    pTransform->X = 42.0f;

    DendyEngine::ECS::STransform* pOtherTransform = pECSEngine->GetComponent<DendyEngine::ECS::STransform>(pGameObject);

    for (auto pGameObject : pECSEngine->GetGameObjectsVecWithComponents<DendyEngine::ECS::STransform>())
    {
        std::cout << *pGameObject << std::endl;
    }
    //pECSEngine->GetGameObjectsSetWithComponents<DendyEngine::ECS::EGameComponentType::Transform,DendyEngine::ECS::EGameComponentType::SimpleMesh>();


    bool IsInDebug = true;

    DendyCommon::CTimer Timer;
    int Counter = 0;

    //system("pause>nul");

    DendyEngine::CEngineCore* pEngine = new DendyEngine::CEngineCore(IsInDebug);
    while(pEngine->GetRunningState())
    {
        Timer.Reset();
        pEngine->Update();
        Counter++;
        if (Counter == 99)
        {
            Counter = 0;
            Timer.Pause();
            //std::cout << "Global Update: Elapsed " << (Timer.GetElapsedNanoseconds()/1000000.0f) << "ms, fps: " << (1.0f/(float)Timer.GetElapsedNanoseconds()*1000000000.0f) << std::endl;
        }
        
    }
    
    delete pEngine;

    
    DendyCommon::CLogger::DestroyInstance();

    system("pause>nul");

    return 0;
}