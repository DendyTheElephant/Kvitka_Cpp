#include <iostream>

#include "PixPhetamine/RenderingCore.h"
#include "DendyCommon/Logger.h"

// #include <stacktrace>

// OpenGL Mathematics (0.9.9.8)
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>


int main()
{

    std::cout << __cplusplus << std::endl;

    DendyCommon::CLogger* pLogger = &DendyCommon::CLogger::GetInstance();
    pLogger->SetOutputFile("G:\\DyCode\\Kvitka_Cpp\\LogError.txt");


    bool IsInDebug = true;
    PixPhetamine::CRenderingCore* pRenderingEngine = new PixPhetamine::CRenderingCore(IsInDebug);

	pRenderingEngine->RunGameLoop();

	delete pRenderingEngine;

    
    DendyCommon::CLogger::DestroyInstance();


    return 0;
}