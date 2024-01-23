#include <DendyEngine/Terrain.h>
#include <DendyCommon/Logger.h>
#include <DendyCommon/Math.h>

//#include <CImg.h>
#include <lodepng.h>

#include <algorithm>
#include <random>
#include <iostream>

DendyEngine::CTerrain::CTerrain():
IMesh("myTerrain")
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    // Load file and decode image.
    std::vector<unsigned char> Data;
    unsigned Width, Height;
    unsigned Error = lodepng::decode(Data, Width, Height, "ressources/images/ruggedTerrainHeightmap.png");
    if (Error != 0)
    {
        LOG_CRITICAL_ERROR(lodepng_error_text(Error));
    }

    //m_HeightsArray.fill(0.0f);

    // read image in any Format
    // cimg_library::CImg<unsigned char> Image("ressources/images/ruggedTerrainHeightmap.png"); //T=unsigned char, meaning R/G/B must be whole number
    // auto ImageData = Image.data();

    // // read and write a pixel
    // int w=Image.width();
    // int h=Image.height();
    // int c=Image.spectrum();
    // std::cout << "Dimensions: " << w << "x" << h << " " << c << " channels" << std::endl;


    //cimg_library::CImgDisplay main_disp(Image,"Click a point");
    // while (!main_disp.is_closed()) {
    //     main_disp.wait();
    // }
    // Dump all pixels
    for (int y=0; y<Height; y++)
    {
        for (int x=0; x<Width; x++)
        {
            size_t Coordinate = y*c_TerrainSize * 4+x * 4;
            // float MinValue255 = 18.0f;
            // float MaxValue255 = 45.0f;
            float MinValue255 = 0.0f;
            float MaxValue255 = 255.0f;
            float Height = (float)(Data.at(Coordinate)-MinValue255) / MaxValue255;
            //float Height = (float)Image(x,y,0,0) / 255.0f;
            _SetHeight(x,y,Height*c_TerrainMaxHeight);
        }
    }







    // std::random_device RandomDevice;

    // //
    // // Engines 
    // //
    // std::mt19937 RandomMersenneEngine(RandomDevice());
    // //std::knuth_b RandomMersenneEngine(RandomDevice());
    // //std::default_random_engine RandomMersenneEngine(RandomDevice()) ;

    // //
    // // Distribtuions
    // //
    // std::uniform_real_distribution<> dist(0, c_TerrainMaxHeight);
    // //std::normal_distribution<> dist(2, 2);
    // //std::student_t_distribution<> dist(5);
    // //std::poisson_distribution<> dist(2);
    // //std::extreme_value_distribution<> dist(0,2);
    
    // auto RandomGenerator = [&dist, &RandomMersenneEngine](){return dist(RandomMersenneEngine);};

    // std::generate(begin(m_HeightsArray), end(m_HeightsArray), RandomGenerator);

    //m_HeightsArray.at(c_TerrainSize*c_TerrainSize/2.0f+c_TerrainSize/2.0f) = 500.0f;

    _InitialiseMesh();
}

DendyEngine::CTerrain::~CTerrain()
{
    
}

void DendyEngine::CTerrain::LoadToGPU()
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);
    
    std::array<GLuint,3> VBOHandlesVec = {0,0,0};
    glGenBuffers(3, VBOHandlesVec.data());

    GLuint PositionsBufferHandle = VBOHandlesVec[0];
    GLuint NormalsBufferHandle = VBOHandlesVec[1];
    GLuint TextureCoordinatesBufferHandle = VBOHandlesVec[2];

    // Populate the position buffer
    glBindBuffer(GL_ARRAY_BUFFER, PositionsBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, m_PositionsVec.size()*sizeof(GLfloat), m_PositionsVec.data(), GL_STATIC_DRAW);
    // Populate the normal buffer
    glBindBuffer(GL_ARRAY_BUFFER, NormalsBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, m_NormalsVec.size()*sizeof(GLfloat), m_NormalsVec.data(), GL_STATIC_DRAW);
    // Populate the texture coordinate buffer
    glBindBuffer(GL_ARRAY_BUFFER, TextureCoordinatesBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, m_TextureCoordinatesVec.size()*sizeof(GLfloat), m_TextureCoordinatesVec.data(), GL_STATIC_DRAW);

    // Create and set the VAO (Vertex Array Object)
    glGenVertexArrays(1, &m_VAOHandle);
    glBindVertexArray(m_VAOHandle);

    // Enable Vertex attributes arrays
    glEnableVertexAttribArray(0); // Vertex position
    glEnableVertexAttribArray(1); // Vertex normal
    glEnableVertexAttribArray(2); // Vertex texture coordinate

    // Tell the formats of each buffer
    glBindVertexBuffer(0, PositionsBufferHandle, 0, sizeof(GLfloat) * 3);
    glBindVertexBuffer(1, NormalsBufferHandle, 0, sizeof(GLfloat) * 3);
    glBindVertexBuffer(2, TextureCoordinatesBufferHandle, 0, sizeof(GLfloat) * 2);

    
    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(0, 0); // Map positions to shader pos0

    glVertexAttribFormat(1, 3, GL_FLOAT, GL_TRUE, 0);
    glVertexAttribBinding(1, 1); // Map normals to shader pos1

    glVertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(2, 2); // Map texture coords to shader pos2


    // Indices
    GLuint IndicesHandle;
    glGenBuffers(1, &IndicesHandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndicesHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_FaceIndicesVec.size()*sizeof(GLuint), m_FaceIndicesVec.data(), GL_STATIC_DRAW);

    
    m_IsLoadedInGPU = true;

    LOG_CALLSTACK_POP();
}

void DendyEngine::CTerrain::UnloadFromGPU()
{
    glDeleteBuffers(1, &m_VAOHandle);
}

void DendyEngine::CTerrain::_InitialiseMesh()
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    for (size_t iHeight=0; iHeight<c_TerrainSize*c_TerrainSize; iHeight++)
    {
        float PosX = iHeight % c_TerrainSize;
        float PosZ = iHeight / c_TerrainSize;
        m_PositionsVec.push_back((PosX-c_TerrainSize/2.0f) * m_Scale);
        m_PositionsVec.push_back(_GetHeight(iHeight));
        m_PositionsVec.push_back((PosZ-c_TerrainSize/2.0f) * m_Scale);


        m_NormalsVec.push_back(0.0f);
        m_NormalsVec.push_back(0.0f);
        m_NormalsVec.push_back(0.0f);

        m_TextureCoordinatesVec.push_back(0.0f);
        m_TextureCoordinatesVec.push_back(0.0f);
    }
    
    for (size_t iFaces=0; iFaces<c_TerrainSize*c_TerrainSize; iFaces++)
    {
        m_FaceIndicesVec.push_back(iFaces); // Top left
        m_FaceIndicesVec.push_back(iFaces+c_TerrainSize); // Bottom left
        m_FaceIndicesVec.push_back(iFaces+1); // Top right
        

        //m_FaceIndicesVec.push_back(iFaces+c_TerrainSize+1); // Bottom right
        //m_FaceIndicesVec.push_back(iFaces+1); // Top right
        //m_FaceIndicesVec.push_back(iFaces+c_TerrainSize); // Bottom left
    }

    LOG_CALLSTACK_POP();
}

float DendyEngine::CTerrain::GetHeightAtPosition(glm::vec2 const& position) const
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);
//#if defined(_DEBUG)
    if (std::abs(position.x) > static_cast<float>(c_TerrainSize)/2.0*m_Scale)
        LOG_CRITICAL_ERROR("position.x value ["+std::to_string(position.x)+"] out of terrain!");
    if (std::abs(position.y) > static_cast<float>(c_TerrainSize)/2.0*m_Scale)
        LOG_CRITICAL_ERROR("position.y value ["+std::to_string(position.y)+"] out of terrain!");
//#endif // _DEBUG

    glm::vec2 PositionInTerrainSpace;
    PositionInTerrainSpace.x = (position.x / m_Scale + static_cast<float>(c_TerrainSize)/2.0);
    PositionInTerrainSpace.y = (position.y / m_Scale + static_cast<float>(c_TerrainSize)/2.0);

    int FloorX = static_cast<int>(std::floor(PositionInTerrainSpace.x));
    int FloorY = static_cast<int>(std::floor(PositionInTerrainSpace.y));
    float HeightTopLeft = _GetHeight(FloorX,FloorY);
    float HeightTopRight = _GetHeight(FloorX+1,FloorY);
    float HeightBotLeft = _GetHeight(FloorX,FloorY+1);
    float HeightBotRight = _GetHeight(FloorX+1,FloorY+1);

    float FractionX = PositionInTerrainSpace.x - std::floor(PositionInTerrainSpace.x);
    float FractionY = PositionInTerrainSpace.y - std::floor(PositionInTerrainSpace.y);

    // Bilinear
    // float HeightTop = std::lerp(HeightTopLeft, HeightTopRight, FractionX);
    // float HeightBot = std::lerp(HeightBotLeft, HeightBotRight, FractionX);

    // float Height = std::lerp(HeightTop, HeightBot, FractionY);
    
    // Triangle Barycentric
    float Height;
    if (FractionX < 1.0f - FractionY)
    { // Triangle 00
        
        Height = DendyCommon::Math::BarycentricCoordinates(glm::vec3 (0, HeightTopLeft, 0), glm::vec3(1,HeightTopRight, 0), glm::vec3(0,HeightBotLeft, 1), glm::vec2(FractionX, FractionY));
    }
    else
    { // Triangle 11
        Height = DendyCommon::Math::BarycentricCoordinates(glm::vec3 (1, HeightBotRight, 1), glm::vec3(1,HeightTopRight, 0), glm::vec3(0,HeightBotLeft, 1), glm::vec2(FractionX, FractionY));
    }

    LOG_CALLSTACK_POP();
    return Height;
}

glm::vec3 DendyEngine::CTerrain::GetNormalAtPosition(glm::vec2 const& position) const
{
    return glm::vec3(0.0f);
}

glm::vec3 DendyEngine::CTerrain::GetNormalAtPosition(glm::vec3 const& position) const
{
    return glm::vec3(0.0f);
}

float DendyEngine::CTerrain::GetHeightAtPosition(glm::vec3 const& position) const
{
    return GetHeightAtPosition(glm::vec2(position.x, position.z));
}
