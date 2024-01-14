#include "PixPhetamine/RenderingCore.h"
#include "DendyCommon/Logger.h"

#include <iostream>

PixPhetamine::CRenderingCore::CRenderingCore(bool isInDebugState):
m_IsInDebugState(isInDebugState)
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

    // const char* FramebufferVertexShaderSource = R"(#version 460 core
    // layout (location = 0) in vec3 pos;
    // layout (location = 1) in vec2 uvs;
    // out vec2 UVs;
    // void main()
    // {
    //     gl_Position = vec4(2.0 * pos.x, 2.0* pos.y, 2.0*pos.z, 1.000);
    //     UVs = uvs;
    // })";

    m_pMainWindow = glfwCreateWindow(VIEWPORT_WIDTH, VIEWPORT_HEIGHT, "Kvitka - V0.00.01", NULL, NULL);
    if (m_pMainWindow == nullptr)
    {
        LOG_CRITICAL_ERROR("Failed to open GLFW window");
    }
    glfwMakeContextCurrent(m_pMainWindow);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG_CRITICAL_ERROR("Failed to initialize GL with Glad");
    }


    glViewport(0, 0, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
    //glfwSetFramebufferSizeCallback(pMainWindow, _FramebufferSizeCallback);

    // Retrieve the GPU - OpenGL Current specs for the platform --> Log file
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


    // Init InputHandler
    m_pInputHandler = new CInputHandler(m_pMainWindow);
    m_pMainCamera = new PixPhetamine::CCamera(VIEWPORT_WIDTH, VIEWPORT_HEIGHT);

    /* ============================================== */
    /* Insert names of shaders to load in ShaderNames */
    /* ============================================== */
    m_ShaderNamesVec.push_back("basic");
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

    m_ShaderMapByName["basic"]->DeclareUniformVariableName("u_ModelViewProjectionMatrix");
    m_ShaderMapByName["basic"]->DeclareUniformVariableName("u_Color");
    

    // STACK_MESSAGE("Creation of FrameBuffers");
    // m_GBufferMS = new CFrameBuffer(PIX::WINDOW_WIDTH, PIX::WINDOW_HEIGHT, CFrameBuffer::EType::WITH_DEPTH_MULTISAMPLED);
    // m_GBufferAA = new CFrameBuffer(PIX::WINDOW_WIDTH, PIX::WINDOW_HEIGHT, CFrameBuffer::EType::WITH_DEPTH);
    // m_DownSampled = new CFrameBuffer(PIX::WINDOW_WIDTH / static_cast<pxUInt>(PIX::DOWNSCALING), PIX::WINDOW_HEIGHT / static_cast<pxUInt>(PIX::DOWNSCALING), CFrameBuffer::EType::NORMAL);
    // m_RGBSplitted = new CFrameBuffer(PIX::WINDOW_WIDTH, PIX::WINDOW_HEIGHT, CFrameBuffer::EType::NORMAL);
    // m_BufferBlurPartial = new CFrameBuffer(PIX::WINDOW_WIDTH / static_cast<pxUInt>(PIX::DOWNSCALING), PIX::WINDOW_HEIGHT / static_cast<pxUInt>(PIX::DOWNSCALING), CFrameBuffer::EType::NORMAL);
    // m_BufferBlur = new CFrameBuffer(PIX::WINDOW_WIDTH / static_cast<pxUInt>(PIX::DOWNSCALING), PIX::WINDOW_HEIGHT / static_cast<pxUInt>(PIX::DOWNSCALING), CFrameBuffer::EType::NORMAL);
    // STACK_MESSAGE("Creation of FrameBuffers [COMPLETE]");

    // STACK_MESSAGE("Checking OpenGL errors");
    // Utility::UErrorHandler::checkOpenGLErrors();

    // STACK_MESSAGE("Initialisation of FrameBuffers");
    // // Texture attachment
    // m_GBufferMS->addTexture("colorTexture", PixPhetamine::LowLevelWrapper::CTexture::ETextureType::NORMAL, 0);
    // m_GBufferMS->addTexture("normalTexture", PixPhetamine::LowLevelWrapper::CTexture::ETextureType::NORMAL, 1);
    // m_GBufferMS->addTexture("typeTexture", PixPhetamine::LowLevelWrapper::CTexture::ETextureType::NORMAL, 2);

    // m_GBufferAA->addTexture("colorTexture", PixPhetamine::LowLevelWrapper::CTexture::ETextureType::NORMAL, 0);
    // m_GBufferAA->addTexture("normalTexture", PixPhetamine::LowLevelWrapper::CTexture::ETextureType::NORMAL, 1);
    // m_GBufferAA->addTexture("typeTexture", PixPhetamine::LowLevelWrapper::CTexture::ETextureType::NORMAL, 2);
    // m_GBufferAA->addTexture("blurredColorTexture", PixPhetamine::LowLevelWrapper::CTexture::ETextureType::NORMAL, 3);

    // m_DownSampled->addTexture("processedTexture", PixPhetamine::LowLevelWrapper::CTexture::ETextureType::NORMAL, 0);
    // m_RGBSplitted->addTexture("processedTexture", PixPhetamine::LowLevelWrapper::CTexture::ETextureType::NORMAL, 0);
    // m_BufferBlurPartial->addTexture("processedTexture", PixPhetamine::LowLevelWrapper::CTexture::ETextureType::NORMAL, 0);
    // m_BufferBlur->addTexture("processedTexture", PixPhetamine::LowLevelWrapper::CTexture::ETextureType::NORMAL, 0);
    // STACK_MESSAGE("Initialisation of FrameBuffers [COMPLETE]");

    // STACK_MESSAGE("Checking OpenGL errors");
    // Utility::UErrorHandler::checkOpenGLErrors();

    // STACK_MESSAGE("Setup of Post Process passes");
    // // Creation
    // m_DownSamplingPass = new CPostProcessPass(m_ShaderList["downsampling"], m_DownSampled);
    // m_BlurPassPartI = new CPostProcessPass(m_ShaderList["blurH"], m_BufferBlurPartial);
    // m_BlurPassPartII = new CPostProcessPass(m_ShaderList["blurV"], m_BufferBlur);
    // m_RGBSplitPass = new CPostProcessPass(m_ShaderList["rgbsplit"], m_RGBSplitted);
    // m_DeferredShadingPass = new CPostProcessPass(m_ShaderList["postprocess"]);
    // // Downsampling pass set-up
    // m_DownSamplingPass->bindTexture(m_GBufferAA->getTexture("colorTexture"), "image", 0);
    // m_DownSamplingPass->bindVariableName("scale");
    // // Blur pass I pass set-up
    // m_BlurPassPartI->bindTexture(m_DownSampled->getTexture("processedTexture"), "image", 0);
    // // Blur pass II pass set-up
    // m_BlurPassPartII->bindTexture(m_BufferBlurPartial->getTexture("processedTexture"), "image", 0);
    // // RGB Split pass set-up
    // m_RGBSplitPass->bindTexture(m_GBufferAA->getTexture("colorTexture"), "image", 0);
    // m_RGBSplitPass->bindVariableName("split");
    // // Deferred shading pass set-up
    // m_DeferredShadingPass->bindTexture(m_GBufferAA->getTexture("colorTexture"), "color_map", 0);
    // m_DeferredShadingPass->bindTexture(m_GBufferAA->getTexture("normalTexture"), "normal_map", 1);
    // m_DeferredShadingPass->bindTexture(m_GBufferAA->getTexture("typeTexture"), "type_map", 2);
    // m_DeferredShadingPass->bindTexture(m_GBufferAA->getTexture("blurredColorTexture"), "blurred_color_map", 3);
    // m_DeferredShadingPass->bindTexture(m_GBufferAA->getTexture("depth_texture"), "depth_map", 4);
    // m_DeferredShadingPass->bindVariableName("sun_direction");
    // STACK_MESSAGE("Setup of Post Process passes [COMPLETE]");

    // STACK_MESSAGE("Checking OpenGL errors");
    // Utility::UErrorHandler::checkOpenGLErrors();

    // std::vector<std::string> skyboxTextures;
    // skyboxTextures.push_back("textures/skyboxRight.png");
    // skyboxTextures.push_back("textures/skyboxLeft.png");
    // skyboxTextures.push_back("textures/skyboxTop.png");
    // skyboxTextures.push_back("textures/skyboxBottom.png");
    // skyboxTextures.push_back("textures/skyboxFront.png");
    // skyboxTextures.push_back("textures/skyboxBack.png");
    // m_skyBox = new PixPhetamine::SceneRendering::CSkybox("textures/skybox");	

    // STACK_MESSAGE("Checking OpenGL errors");
    // Utility::UErrorHandler::checkOpenGLErrors();


    LOG_CALLSTACK_POP();
}

PixPhetamine::CRenderingCore::~CRenderingCore()
{
    //PixPhetamine::Display::shutdownSDL_GL(m_SDLWindow, m_GLContext);

    for (auto const &it_shaderName : m_ShaderNamesVec) {
        //delete m_ShaderMapByName[it_shaderName];
    }

    for (auto const &it_meshName : m_MeshNamesVec) {
        //delete m_MeshMap[it_meshName];
    }

    glfwDestroyWindow(m_pMainWindow);
    glfwTerminate();
}

void PixPhetamine::CRenderingCore::_LoadShaders()
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    for (auto const &ShaderName : m_ShaderNamesVec)
    {
        std::string VertexShaderFullPath = "G:\\DyCode\\Kvitka_Cpp\\shaders\\" + ShaderName + ".vs";
        std::string FragmentShaderFullPath = "G:\\DyCode\\Kvitka_Cpp\\shaders\\" + ShaderName + ".fs";
        m_ShaderMapByName[ShaderName] = new PixPhetamine::CShader(VertexShaderFullPath.c_str(), FragmentShaderFullPath.c_str());
    }

    LOG_CALLSTACK_POP();
}

void PixPhetamine::CRenderingCore::_ReloadShaders()
{
    // STACK_TRACE;
    // for (auto const &it_shaderName : m_ShaderNames) {
    //     m_ShaderList[it_shaderName]->reload();
    // }
    // UNSTACK_TRACE;
}

void PixPhetamine::CRenderingCore::_LoadMeshes()
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    for (auto const &MeshName : m_MeshNamesVec)
    {
        if (MeshName == "Triangle")
        {
            m_MeshMapByName[MeshName] = new CMesh(MeshName, CMesh::BasicMeshes::Triangle);
            m_MeshMapByName[MeshName]->LoadToGPU();
        }
        else if (MeshName == "Cube")
        {
            m_MeshMapByName[MeshName] = new CMesh(MeshName, CMesh::BasicMeshes::Cube);
            m_MeshMapByName[MeshName]->LoadToGPU();
        }
        else if (MeshName == "Quad")
        {
            m_MeshMapByName[MeshName] = new CMesh(MeshName, CMesh::BasicMeshes::Quad);
            m_MeshMapByName[MeshName]->LoadToGPU();
        }
        else
        {
            // std::string vertexShader = "G:\\DyCode\\Kvitka_Cpp\\shaders\\" + MeshName + ".vs";
            // std::string fragmentShader = "G:\\DyCode\\Kvitka_Cpp\\shaders\\" + MeshName + ".fs";
            // m_ShaderMapByName[it_shaderName] = new PixPhetamine::CShader(vertexShader.c_str(), fragmentShader.c_str());
        }
    }
    
    LOG_CALLSTACK_POP();
}






void PixPhetamine::CRenderingCore::RunGameLoop()
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);
    m_IsRunning = true;

    // m_secondTimer.start();

    float V = 0;

    while(!glfwWindowShouldClose(m_pMainWindow))
    {
        m_pInputHandler->UpdateInputs();

        /* =========================================================================================== */
        /* ==== Draw the Scene ======================================================================= */
        /* =========================================================================================== */
        
        // GLenum gBufferTargets[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_DEPTH_ATTACHMENT };
        // PixPhetamine::LowLevelWrapper::initialiseDrawIntoBuffer(m_ShaderList["basic"]->id(), m_GBufferMS->getId(), gBufferTargets, 3);

        /* Draw LionHeads */
        //pxFloat type_fox[4] = { 1.0f, 0.0f, 0.0f, 1.0f };

        //m_pMainCamera->SetPosition(glm::vec3(0.0f, V*100.0f, 1.0f));
        m_pMainCamera->SetPosition(glm::vec3(0.0f, 10.0f, 0.1f));
        m_pMainCamera->SetTarget(glm::vec3(0.0f, 0.0f, 0.0f));

        m_ViewProjectionMatrix = m_pMainCamera->GetViewProjectionMatrix();

        V = V+0.01f;
        if (V > 1.0f)
            V = 0.0f;

        

        //glEnable(GL_DEPTH_TEST);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        CShader* pCurrentShader = m_ShaderMapByName["basic"];
        CMesh* pCurrentMesh = m_MeshMapByName["Cube"];

        glUseProgram(pCurrentShader->GetId());
        glBindVertexArray(pCurrentMesh->GetVAO());
        

        for (size_t i = 0; i < 1000; ++i)
        {
            glm::mat4 ModelMatrix = glm::mat4(1);
            //pxVec3f rotateY(0.0f, 1.0f, 0.0f);
            ModelMatrix = glm::translate(ModelMatrix, glm::vec3((float)(i/100)*3.0f, 0.0f, (float)(i%100)*3.0f));
            //M = glm::rotate(M, 90.0f, rotateY);
            //ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));

            m_ModelViewProjectionMatrix = m_ViewProjectionMatrix * ModelMatrix;

            
            pCurrentShader->SendUniformVariable("u_ModelViewProjectionMatrix", m_ModelViewProjectionMatrix);
            pCurrentShader->SendUniformVariable("u_Color", glm::vec3(V,1.0f-V,i/1000.0));
            

            //glUniformMatrix4fv(glGetUniformLocation(pCurrentShader->GetId(), "ModelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(m_ModelViewProjectionMatrix));

            // glUniform3fv(glGetUniformLocation(pCurrentShader->GetId(), "sun_direction"), 1, old_sunDirection);
            // glUniform3fv(glGetUniformLocation(pCurrentShader->GetId(), "sun_color"), 1, sunColor);
            // glUniform4fv(glGetUniformLocation(pCurrentShader->GetId(), "object_type"), 1, type_fox);

            glDrawElements(GL_TRIANGLES, pCurrentMesh->GetTriangleCount(), GL_UNSIGNED_INT, (void *)0);
        }

        // disable
        // glBindVertexArray(0);
        // glUseProgram(0);

        //STACK_MESSAGE("Scene Draw");
        AssertOpenGLErrors();

        // OpenGL
        glfwSwapBuffers(m_pMainWindow);
    }

//     do {
//         STACK_TRACE;

//         const pxUInt startFrameTime = SDL_GetTicks();

//         m_InputHandler->update();

//         m_pMainCamera->moveView((pxFloat)m_InputHandler->getMouseMotionX(), (pxFloat)m_InputHandler->getMouseMotionY());

//         pxFloat speed = 0.4f;

//         if (m_InputHandler->getKey(CInputHandler::EInput::LEFT)) {
//             m_pMainCamera->moveCameraLeft(speed);
//         }
//         if (m_InputHandler->getKey(CInputHandler::EInput::RIGHT)) {
//             m_pMainCamera->moveCameraRight(speed);
//         }
//         if (m_InputHandler->getKey(CInputHandler::EInput::UP)) {
//             m_pMainCamera->moveCameraForward(speed);
//         }
//         if (m_InputHandler->getKey(CInputHandler::EInput::DOWN)) {
//             m_pMainCamera->moveCameraBackward(speed);
//         }
//         if (m_InputHandler->getKey(CInputHandler::EInput::SHOULDER_RIGHT)) {
//             m_pMainCamera->moveCameraUp(speed);
//         }
//         if (m_InputHandler->getKey(CInputHandler::EInput::SHOULDER_LEFT)) {
//             m_pMainCamera->moveCameraDown(speed);
//         }

//         /*if (pxUInt value = m_InputHandler->getBulletTime()) {
//             reloadShaders();
//         }*/


//         pxVec3f sunDirectionV = pxVec3f(0.5f, 0.5f, 0.0f);
//         sunDirectionV = glm::normalize(sunDirectionV);
//         pxFloat old_sunDirection[3] = { sunDirectionV.x, sunDirectionV.y, sunDirectionV.z };
//         pxVec3f sunDirection = pxVec3f(sunDirectionV.x, sunDirectionV.y, sunDirectionV.z);
//         pxFloat sunColor[3] = { 1.0f, 1.0f, 1.0f };


//         STACK_MESSAGE("Scene Draw");
//         STACK_MESSAGE("Checking OpenGL errors");
//         Utility::UErrorHandler::checkOpenGLErrors();

//         /* =========================================================================================== */
//         /* ==== Draw the Scene ======================================================================= */
//         /* =========================================================================================== */
//         m_ViewProjectionMatrix = m_pMainCamera->getViewProjectionMatrix();
//         GLenum gBufferTargets[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_DEPTH_ATTACHMENT };
//         PixPhetamine::LowLevelWrapper::initialiseDrawIntoBuffer(m_ShaderList["basic"]->id(), m_GBufferMS->getId(), gBufferTargets, 3);

//         /* Draw LionHeads */
//         pxFloat type_fox[4] = { 1.0f, 0.0f, 0.0f, 1.0f };


//         // PRE SKYBOX OPENGL DIRECTIVES:
//         m_skyBox->draw(m_ViewProjectionMatrix, m_pMainCamera->getPosition());
//         // POST SKYBOX OPENGL DIRECTIVES:
//         glEnable(GL_DEPTH_TEST);
//         glUseProgram(m_ShaderList["basic"]->id());

//         glBindVertexArray(m_MeshList["lionhead"]->getVBO());

//         for (size_t i_lionhead = 0; i_lionhead < 3000; ++i_lionhead) {

//             m_ModelMatrix = pxMat4f();
//             pxVec3f rotateY(0.0f, 1.0f, 0.0f);
//             m_ModelMatrix = glm::translate(m_ModelMatrix, pxVec3f(-(i_lionhead % 33 * 3.0f), 0.0f, -(i_lionhead / 33 * 3.0f)));
//             //M = glm::rotate(M, 90.0f, rotateY);
//             m_ModelMatrix = glm::scale(m_ModelMatrix, pxVec3f(0.5f, 0.5f, 0.5f));

//             m_ModelViewProjectionMatrix = m_ViewProjectionMatrix * m_ModelMatrix;

//             glUniformMatrix4fv(glGetUniformLocation(m_ShaderList["basic"]->id(), "MVP"), 1, GL_FALSE, glm::value_ptr(m_ModelViewProjectionMatrix));
//             glUniform3fv(glGetUniformLocation(m_ShaderList["basic"]->id(), "sun_direction"), 1, old_sunDirection);
//             glUniform3fv(glGetUniformLocation(m_ShaderList["basic"]->id(), "sun_color"), 1, sunColor);
//             glUniform4fv(glGetUniformLocation(m_ShaderList["basic"]->id(), "object_type"), 1, type_fox);

//             glDrawElements(GL_TRIANGLES, m_MeshList["lionhead"]->getNumberOfFaces(), GL_UNSIGNED_SHORT, (void *)0);
//         }
//         glBindVertexArray(0);

//         // disable shader
//         glUseProgram(0);

//         STACK_MESSAGE("Scene Draw [COMPLETE]");

//         STACK_MESSAGE("Scene Draw");
//         STACK_MESSAGE("Checking OpenGL errors");
//         Utility::UErrorHandler::checkOpenGLErrors();

//         STACK_MESSAGE("Anti Aliasing filtering");

//         /* =========================================================================================== */
//         /* ==== Anti Aliasing filtering ============================================================== */
//         /* =========================================================================================== */
//         //PixPhetamine::LowLevelWrapper::multiSamplingAntiAliasing(m_GBufferMultiSampled, m_GBufferWitoutAliasing, WINDOW_WIDTH, WINDOW_HEIGHT);
//         STACK_TRACE;

//         m_GBufferMS->blit(m_GBufferAA, { 
//             CFrameBuffer::SBlit("colorTexture", "colorTexture"),
//             CFrameBuffer::SBlit("normalTexture", "normalTexture"),
//             CFrameBuffer::SBlit("typeTexture", "typeTexture"),
//             CFrameBuffer::SBlit("depth_texture", "depth_texture")
//         });

//         UNSTACK_TRACE;

//         STACK_MESSAGE("Anti Aliasing filtering [COMPLETE]");
//         STACK_MESSAGE("Scene Draw");
//         STACK_MESSAGE("Checking OpenGL errors");
//         Utility::UErrorHandler::checkOpenGLErrors();
//         /* =========================================================================================== */
//         /* ==== Post Process ========================================================================= */
//         /* =========================================================================================== */

//         ///* Blur ====================================================================================== */
//         // Downsampling
        
//         m_DownSamplingPass->sendVariable("scale", float(PIX::DOWNSCALING));
//         m_DownSamplingPass->process({ "processedTexture" });

//         // Horizontal blur
//         m_BlurPassPartI->process({ "processedTexture" });

//         // Vertical blur
//         m_BlurPassPartII->process({ "processedTexture" });

//         //m_GBufferAA->replaceTexture("colorTexture", m_BufferBlur, "processedTexture");
//         //m_GBufferAA->replaceTexture("blurredColorTexture", m_BufferBlur, "processedTexture");
//         m_BufferBlur->blit(m_GBufferAA, { CFrameBuffer::SBlit("processedTexture", "blurredColorTexture") });




//         //if (pxUInt value = m_InputHandler->getShoot()) {
//             /* RGB Split ================================================================================================================ */

//             //pxFloat split = (pxFloat)value / 10.0f;
//             //
//             //m_RGBSplitPass->sendVariable("split", split);
//             //m_RGBSplitPass->process({ "processedTexture" });

//             ////m_GBufferAA->replaceTexture("colorTexture", m_RGBSplitted, "processedTexture");
//             //m_RGBSplitted->blit(m_GBufferAA, { CFrameBuffer::SBlit("processedTexture", "colorTexture") });
//         //}

//         /* =========================================================================================== */
//         /* ============ Deferred Shading, final pass ================================================= */
//         /* =========================================================================================== */
//         m_DeferredShadingPass->sendVariable("sun_direction", sunDirection);
//         m_DeferredShadingPass->process();









//         /* Swap our back buffer to the front */
//         SDL_GL_SwapWindow(m_SDLWindow);


//         const Uint32 endFrameTime = SDL_GetTicks();

//         if (m_secondTimer.getElapsedTime() > 500) {

//             m_ElapsedTime += endFrameTime - startFrameTime;
//             ++m_frame;

// #ifdef _MSC_VER
//             sprintf_s(m_windowCaption, "%s    FPS: %f", PIX::WINDOW_CAPTION, m_frame / (m_ElapsedTime / 1000.0));
// #else
//             snprintf(m_windowCaption, 64, "%s    FPS: %f", PIX::WINDOW_CAPTION, m_frame / (m_ElapsedTime / 1000.0));
// #endif

//             SDL_SetWindowTitle(m_SDLWindow, m_windowCaption);
//             /*
//             std::cout << "==========================================" << std::endl;
//             std::cout << "Scene rendering:        ";//< m_renderSceneTimer.getElapsedTime() << " ms" << std::endl;
//             std::cout << "MSAAfiltering:          ";//<< m_renderAntiAliasingTimer.getElapsedTime() << " ms" << std::endl;
//             std::cout << "Post-process rendering: ";//<< m_renderPostProcessTimer.getElapsedTime() << " ms" << std::endl;
//             std::cout << "==========================================" << std::endl;
//             */
//             m_secondTimer.start();
//         }

//         // Every 10 sec
//         if (m_ElapsedTime > 1000) {
//             m_ElapsedTime = 0;
//             m_frame = 0;
//         }

//         STACK_MESSAGE("Scene Draw");
//         STACK_MESSAGE("Checking OpenGL errors");
//         Utility::UErrorHandler::checkOpenGLErrors();

//         UNSTACK_TRACE;

//     } while (m_InputHandler->isQuit() == false);
    LOG_CALLSTACK_POP();
}

void PixPhetamine::CRenderingCore::AssertOpenGLErrors()
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