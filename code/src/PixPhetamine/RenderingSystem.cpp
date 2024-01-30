#include <PixPhetamine/RenderingSystem.h>
#include <DendyCommon/Logger.h>
#include <PixPhetamine/Definitions.h>

#include <lodepng.h>

#include <iostream>

PixPhetamine::CRenderingSystem::CRenderingSystem(bool isInDebugState):
m_IsInDebugState(isInDebugState),
m_IsRunning(true)
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    // GLFW
    if (!glfwInit())
    {
        LOG_CRITICAL_ERROR("Failed to initialize GLFW");
    }

    // OpenGL
    glfwWindowHint(GLFW_SAMPLES, 8);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    //m_pMainWindow = glfwCreateWindow(PixPhetamine::Definitions::c_WindowWidth, PixPhetamine::Definitions::c_WindowHeight, "Kvitka - V0.00.01", glfwGetPrimaryMonitor(), NULL); // Fullscreen
    m_pMainWindow = glfwCreateWindow(PixPhetamine::Definitions::c_WindowWidth, PixPhetamine::Definitions::c_WindowHeight, "Kvitka - V0.00.01", NULL, NULL); // Windowed
    if (m_pMainWindow == nullptr)
    {
        LOG_CRITICAL_ERROR("Failed to open GLFW window");
    }
    glfwMakeContextCurrent(m_pMainWindow);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG_CRITICAL_ERROR("Failed to initialize GL with Glad");
    }


    glViewport(0, 0, PixPhetamine::Definitions::c_WindowWidth, PixPhetamine::Definitions::c_WindowHeight);
    

    // Icon, not WS
    GLFWimage IconImages[1];
    std::vector<unsigned char> Data;
    unsigned Width, Height;
    unsigned Error = lodepng::decode(Data, Width, Height, "ressources/images/icon24.png");
    if (Error != 0)
    {
        LOG_CRITICAL_ERROR(lodepng_error_text(Error));
    }
    IconImages[0].height = Height;
    IconImages[0].width = Width;
    IconImages[0].pixels = Data.data();
    glfwSetWindowIcon(m_pMainWindow, 1, IconImages);
    //glfwSetFramebufferSizeCallback(pMainWindow, _FramebufferSizeCallback);

    //const GLFWvidmode* MonitorMode; // height, width, refreshRate
    //glfwWindowHint(GLFW_REFRESH_RATE, MonitorMode->refreshRate);

    // Retrieve the GPU - OpenGL Current spComponents for the platform --> Log file
    std::cerr << "=============[ PixPhetamine log-file ]=========================" << std::endl;
    std::cerr << ">Graphics Successfully Initialized !" << std::endl;
    std::cerr << "OpenGL Info" << std::endl;
    std::cerr << "    Version: " << glGetString(GL_VERSION) << std::endl;
    std::cerr << "     Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cerr << "   Renderer: " << glGetString(GL_RENDERER) << std::endl;
    int OpenGLVersionMajor, OpenGLVersionMinor;
    glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersionMajor);
    glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersionMinor);
    std::cerr << "     OpenGL: " << OpenGLVersionMajor << "." << OpenGLVersionMinor << std::endl;
    std::cerr << "    Shading: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    //std::cerr << "Monitor ref: " << MonitorMode->refreshRate << std::endl;
    std::cerr << "----------------------------------------------------------------" << std::endl;
    std::cerr << ">GPU Specifications for modern GLSL:" << std::endl;
    int uboBindings, uboSize, uboVertex, uboFragment, uboGeometry;
    glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &uboBindings);
    glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &uboSize);
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &uboVertex);
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &uboFragment);
    glGetIntegerv(GL_MAX_GEOMETRY_UNIFORM_BLOCKS, &uboGeometry);
    std::cerr << "  Max uniform block bindings: " << uboBindings << std::endl;
    std::cerr << "      Max uniform block size: " << uboSize << std::endl;
    std::cerr << "    Max uniform block vertex: " << uboVertex << std::endl;
    std::cerr << "  Max uniform block fragment: " << uboFragment << std::endl;
    std::cerr << "  Max uniform block geometry: " << uboGeometry << std::endl;
    std::cerr << "----------------------------------------------------------------" << std::endl;
    std::cerr << ">Texture specifications:" << std::endl;
    int texUnits;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &texUnits);
    //GL_MAX_COLOR_ATTACHMENTS
    //
    std::cerr << "     Max texture image units: " << uboBindings << std::endl;
    std::cerr << "----------------------------------------------------------------" << std::endl;


    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    /* ============================================== */
    /* Insert names of shaders to load in ShaderNames */
    /* ============================================== */
    m_ShaderNamesVec.push_back("basic");
    m_ShaderNamesVec.push_back("terrain");
    // m_ShaderNamesVec.push_back("postprocess");
    // m_ShaderNamesVec.push_back("downsampling");
    // m_ShaderNamesVec.push_back("blurH");
    // m_ShaderNamesVec.push_back("blurV");
    // m_ShaderNamesVec.push_back("rgbsplit");
    /* ============================================== */

    /* =========================================== */
    /* Insert names of meshes to load in MeshNames */
    /* =========================================== */
    //m_MeshNamesVec.push_back("lionhead");
    m_MeshNamesVec.push_back("Cube");
    m_MeshNamesVec.push_back("Quad");
    m_MeshNamesVec.push_back("Triangle");
    m_MeshNamesVec.push_back("Pawn");
    m_MeshNamesVec.push_back("human");
    m_MeshNamesVec.push_back("hata");
    m_MeshNamesVec.push_back("sich");
    //m_MeshNamesVec.push_back("sphere");
    /* =========================================== */

    std::cout << "Loading Shaders" << std::endl;
    _LoadShaders();
    std::cout << "Loading Shaders [COMPLETE]" << std::endl;

    AssertOpenGLErrors();

    std::cout << "Loading Meshes" << std::endl;
    _LoadMeshes();
    std::cout << "Loading Meshes [COMPLETE]" << std::endl;

    AssertOpenGLErrors();


    m_ShaderMapByName["terrain"]->DeclareUniformVariableName("u_ModelViewProjectionMatrix");

    m_ShaderMapByName["basic"]->DeclareUniformVariableName("u_ModelViewProjectionMatrix");
    m_ShaderMapByName["basic"]->DeclareUniformVariableName("u_Color");


    m_pMainCamera = new PixPhetamine::CCamera(PixPhetamine::Definitions::c_WindowWidth, PixPhetamine::Definitions::c_WindowHeight);
    


    LOG_CALLSTACK_POP();
}

PixPhetamine::CRenderingSystem::~CRenderingSystem()
{
    for (auto const &it_shaderName : m_ShaderNamesVec) {
        //delete m_ShaderMapByName[it_shaderName];
    }

    for (auto const &it_meshName : m_MeshNamesVec) {
        //delete m_MeshMap[it_meshName];
    }

    glfwDestroyWindow(m_pMainWindow);
    glfwTerminate();
}

void PixPhetamine::CRenderingSystem::_LoadShaders()
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    for (auto const &ShaderName : m_ShaderNamesVec)
    {
        std::string VertexShaderFullPath = "shaders\\" + ShaderName + ".vs";
        std::string FragmentShaderFullPath = "shaders\\" + ShaderName + ".fs";
        m_ShaderMapByName[ShaderName] = std::make_unique<CShader>(VertexShaderFullPath.c_str(), FragmentShaderFullPath.c_str());
    }

    LOG_CALLSTACK_POP();
}

void PixPhetamine::CRenderingSystem::_LoadMeshes()
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    for (auto const &MeshName : m_MeshNamesVec)
    {
        if (MeshName == "Triangle")
        {
            m_MeshMapByName[MeshName] = std::make_unique<CMesh>(MeshName, CMesh::EBasicMeshes::Triangle, false, false);
            m_MeshMapByName[MeshName]->LoadToGPU();
        }
        else if (MeshName == "Cube")
        {
            m_MeshMapByName[MeshName] = std::make_unique<CMesh>(MeshName, CMesh::EBasicMeshes::Cube, false, false);
            m_MeshMapByName[MeshName]->LoadToGPU();
        }
        else if (MeshName == "Quad")
        {
            m_MeshMapByName[MeshName] = std::make_unique<CMesh>(MeshName, CMesh::EBasicMeshes::Quad, false, false);
            m_MeshMapByName[MeshName]->LoadToGPU();
        }
        else if (MeshName == "Pawn")
        {
            m_MeshMapByName[MeshName] = std::make_unique<CMesh>(MeshName, CMesh::EBasicMeshes::Pawn, false, false);
            m_MeshMapByName[MeshName]->LoadToGPU();
        }
        else
        {
            m_MeshMapByName[MeshName] = std::make_unique<CMesh>(MeshName, "ressources/meshes/"+MeshName+".obj", false, false);
            m_MeshMapByName[MeshName]->LoadToGPU();
        }
    }

    LOG_CALLSTACK_POP();
}

void PixPhetamine::CRenderingSystem::AddPawnInstance(glm::mat4 const& transformMatrix,glm::vec3 const& color)
{
    m_PawnInstanceDataVec.push_back({transformMatrix,color});
}

void PixPhetamine::CRenderingSystem::AddStaticMesh(std::string const& name, glm::mat4 const& transformMatrix, glm::vec3 const& color)
{
    m_StaticMeshInstanceDataVec.push_back({name,transformMatrix,color});
}

#include <DendyCommon/Math.h>
void PixPhetamine::CRenderingSystem::InitialiseTerrain(size_t terrainSize, float scale, float heightScale, const uint16_t* pHeightsVec)
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    if (m_MeshMapByName["Terrain"] != nullptr && m_MeshMapByName["Terrain"]->GetIsLoadedInGPUState())
        m_MeshMapByName["Terrain"]->UnloadFromGPU();
    m_MeshMapByName["Terrain"] = std::make_unique<CMesh>("Terrain", true, true);
    CMesh* pMesh = m_MeshMapByName["Terrain"].get();

    for (size_t iHeight=0; iHeight<(terrainSize+1)*(terrainSize+1); iHeight++)
    {
        float PosX = iHeight % (terrainSize+1);
        float PosZ = iHeight / (terrainSize+1);

        //pMesh->AddPosition({(PosX-terrainSize/2.0f) * scale, static_cast<float>(pHeightsVec[iHeight])/65535.0f*heightScale, (PosZ-terrainSize/2.0f) * scale});
        pMesh->AddPosition({(PosX-terrainSize/2.0f) * scale, static_cast<float>(0)/65535.0f*heightScale, (PosZ-terrainSize/2.0f) * scale});

        pMesh->AddNormal({0.0f, 1.0f, 0.0f});

        pMesh->AddTextureCoordinate({0,0});
    }
    
    for (uint16_t y=0; y<terrainSize+1; y++)
    {
        for (uint16_t x=0; x<terrainSize+1; x++)
        {
            // Top left, bottom left, top right
            pMesh->AddTriangleIndices(y*(terrainSize+1) + x, (y+1)*(terrainSize+1) + x, y*(terrainSize+1) + (x+1));
            
            // Bottom right, top right, bottom left
            //pMesh->AddTriangleIndices((y+1)*terrainSize + (x+1),  y*terrainSize + (x+1), (y+1)*terrainSize + x);
        }
    }

    m_MeshMapByName["Terrain"]->LoadToGPU();

    LOG_CALLSTACK_POP();
}

void PixPhetamine::CRenderingSystem::RenderScene()
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
    m_pMainCamera->SetPosition(m_CameraLookAtPosition + m_CameraArmTranslation);
    m_pMainCamera->SetTarget(m_CameraLookAtPosition);

    glm::mat4 ViewProjectionMatrix = m_pMainCamera->GetViewProjectionMatrix();


    CShader* pCurrentShader = m_ShaderMapByName["terrain"].get();
    glUseProgram(pCurrentShader->GetId());
    
    { // Render Terrain
        glBindVertexArray(m_MeshMapByName["Terrain"].get()->GetVAO());

        glm::mat4 ModelMatrix = glm::mat4(1.0f);
        glm::mat4 ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;

        pCurrentShader->SendUniformVariable("u_ModelViewProjectionMatrix", ModelViewProjectionMatrix);

        glDrawElements(GL_TRIANGLES, m_MeshMapByName["Terrain"].get()->GetTriangleCount(), GL_UNSIGNED_INT, (void *)0);
    }

    pCurrentShader = m_ShaderMapByName["basic"].get();
    glUseProgram(pCurrentShader->GetId());
    {
        // Render Pawns
        CMesh* pMeshToRender = m_MeshMapByName["human"].get();
        glBindVertexArray(pMeshToRender->GetVAO());

        for (auto [PawnTransformMatrix,PawnColor] : m_PawnInstanceDataVec)
        {
            glm::mat4 ModelViewProjectionMatrix = ViewProjectionMatrix * PawnTransformMatrix;

            pCurrentShader->SendUniformVariable("u_ModelViewProjectionMatrix", ModelViewProjectionMatrix);
            pCurrentShader->SendUniformVariable("u_Color", PawnColor);

            glDrawElements(GL_TRIANGLES, pMeshToRender->GetTriangleCount(), GL_UNSIGNED_INT, (void *)0);
        }

        // Render Static Meshes
        for (auto [MeshName,TransformMatrix,Color] : m_StaticMeshInstanceDataVec)
        {
            CMesh* pMeshToRender = m_MeshMapByName[MeshName].get();
            glBindVertexArray(pMeshToRender->GetVAO());

            glm::mat4 ModelViewProjectionMatrix = ViewProjectionMatrix * TransformMatrix;

            pCurrentShader->SendUniformVariable("u_ModelViewProjectionMatrix", ModelViewProjectionMatrix);
            pCurrentShader->SendUniformVariable("u_Color", Color);

            glDrawElements(GL_TRIANGLES, pMeshToRender->GetTriangleCount(), GL_UNSIGNED_INT, (void *)0);
        }
    }


    // std::unordered_map<char,glm::vec2> TexCoordByChar = {
    //     { 'A', {0.0, 0.0} },
    //     { 'B', {0.0, 1.0} }
    // }
    // pCurrentShader = m_ShaderMapByName["text"].get();
    // glUseProgram(pCurrentShader->GetId());
    // {
    //     CMesh* pMeshToRender = m_MeshMapByName["Quad"].get();
    //     glBindVertexArray(pMeshToRender->GetVAO());
    //     for (auto [String,ScreenCoordinates] : m_TextsDataVec)
    //     {
    //         for (char Char : String)
    //         {
    //             pCurrentShader->SendUniformVariable("u_Color", Color);
    //         }

    //         glm::mat4 ModelViewProjectionMatrix = ViewProjectionMatrix * TransformMatrix;

    //         pCurrentShader->SendUniformVariable("u_ModelViewProjectionMatrix", ModelViewProjectionMatrix);
    //         pCurrentShader->SendUniformVariable("u_Color", Color);

    //         glDrawElements(GL_TRIANGLES, pMeshToRender->GetTriangleCount(), GL_UNSIGNED_INT, (void *)0);
    //     }
    // }


    AssertOpenGLErrors();
    m_PawnInstanceDataVec.clear();
    m_StaticMeshInstanceDataVec.clear();

    // OpenGL
    glfwSwapBuffers(m_pMainWindow);
    LOG_CALLSTACK_POP();
}

void PixPhetamine::CRenderingSystem::ReloadShaders()
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);
    for (const auto& [ShaderName, Shader] : m_ShaderMapByName)
    {
        Shader->Reload();
    }
    LOG_CALLSTACK_POP();
}

void PixPhetamine::CRenderingSystem::AssertOpenGLErrors()
{
    // if (m_IsInDebugState == false)
    //     return;
	GLenum status = glGetError();
	if (status != GL_NO_ERROR)
    {
		switch (status)
        {
		case GL_INVALID_ENUM:
            LOG_CRITICAL_ERROR("OpenGL error: GL_INVALID_ENUM");
			break;
		case GL_INVALID_OPERATION:
            LOG_CRITICAL_ERROR("OpenGL error: GL_INVALID_OPERATION");
			break;
		case GL_INVALID_VALUE:
            LOG_CRITICAL_ERROR("OpenGL error: GL_INVALID_VALUE");
			break;
		case GL_STACK_OVERFLOW:
            LOG_CRITICAL_ERROR("OpenGL error: GL_STACK_OVERFLOW");
			break;
		case GL_STACK_UNDERFLOW:
            LOG_CRITICAL_ERROR("OpenGL error: GL_STACK_UNDERFLOW");
			break;
		case GL_OUT_OF_MEMORY:
            LOG_CRITICAL_ERROR("OpenGL error: GL_OUT_OF_MEMORY");
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
            LOG_CRITICAL_ERROR("OpenGL error: GL_INVALID_FRAMEBUFFER_OPERATION");
			break;
		default:
            LOG_CRITICAL_ERROR("OpenGL error: Unknown error!");
			break;
		}
	}
}