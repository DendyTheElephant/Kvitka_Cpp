#include <iostream>

// OpenGL context GLAD
// https://glad.dav1d.de/
// OpenGL Core V4.6 no extensions
#include <glad/glad.h>

// GLFW (Graphics Library Framework)
// https://www.glfw.org/
#include <GLFW/glfw3.h>

// OpenGL Mathematics (0.9.9.8)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
{
    // glm
    glm::vec4 Vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 MatTransform = glm::mat4(1.0f); // identity
    MatTransform = glm::translate(MatTransform, glm::vec3(100.0f, 100.0f, 0.0f));
    Vec = MatTransform * Vec;
    // std::cout << Vec.x << ' ' << Vec.y << ' ' << Vec.z << ' ' << std::endl;

    glm::vec3 Vec1(0.0f, 1.0f, 0.0f);
    glm::vec3 Vec2(-1.0f, 0.0f, 0.0f);
    Vec2 = glm::normalize(Vec2);
    glm::vec3 CrossResult = glm::cross(Vec1, Vec2);


    std::cout << "Dot: " << glm::dot(Vec1, Vec2) << std::endl;
    std::cout << "Cross: ( " << CrossResult.x << ", " << CrossResult.y << ", " << CrossResult.z << " )" << std::endl;

    // glm::mat4* pMat = new glm::mat4(1.0f);
    // int Int = 4;
    // std::cout << "sizeof ptr: " << sizeof(pMat) << std::endl;
    // std::cout << "sizeof int: " << sizeof(Int) << std::endl;


    // GLFW
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // OpenGL
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* pMainWindow;
    pMainWindow = glfwCreateWindow(800, 600, "Kvitka - V0.00.01", NULL, NULL);
    if (pMainWindow == NULL)
    {
        std::cout << "Failed to open GLFW window" << std::endl;
        return -1;
    }
    glfwMakeContextCurrent(pMainWindow);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(pMainWindow, framebuffer_size_callback);

    while(!glfwWindowShouldClose(pMainWindow))
    {
        // OpenGL
        glfwSwapBuffers(pMainWindow);
        glfwPollEvents();    
    }

    glfwDestroyWindow(pMainWindow);
    glfwTerminate();
    return 0;
}