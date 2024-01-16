#include <iostream>

#include "PixPhetamine/RenderingCore.h"
#include "DendyEngine/EngineCore.h"
#include "DendyCommon/Logger.h"


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


    bool IsInDebug = true;

    DendyEngine::CEngineCore* pEngine = new DendyEngine::CEngineCore(IsInDebug);
    while(pEngine->GetRunningState())
    {
        pEngine->Update();
    }
    
    delete pEngine;

    
    DendyCommon::CLogger::DestroyInstance();

    return 0;
}