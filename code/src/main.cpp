#include <iostream>

#include "PixPhetamine/RenderingCore.h"
#include "DendyEngine/EngineCore.h"
#include "DendyCommon/Logger.h"

int main()
{

    std::cout << __cplusplus << std::endl;

    DendyCommon::CLogger* pLogger = &DendyCommon::CLogger::GetInstance();
    pLogger->SetOutputFile("G:\\DyCode\\Kvitka_Cpp\\LogError.txt");


    bool IsInDebug = true;

    DendyEngine::CEngineCore* pEngine = new DendyEngine::CEngineCore(IsInDebug);
    PixPhetamine::CRenderingCore* pRenderingEngine = new PixPhetamine::CRenderingCore(IsInDebug);

    pEngine->SetRenderingCoreInstance(pRenderingEngine);

    do
    {
        pEngine->Update();
    } while (pEngine->GetRunningState());
    
    delete pEngine;
	delete pRenderingEngine;

    
    DendyCommon::CLogger::DestroyInstance();


    return 0;
}