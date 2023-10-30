#include "EngineRenderer/WindowSystem.h"
#include "common/openglerrors.h"
#include "common/opengl.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

WindowSystem::WindowSystem(iShaderInfo* pShaderInfo)
{
    this->m_pWindow = nullptr;
    this->m_pShaderInfo = pShaderInfo;
}

WindowSystem::~WindowSystem()
{
}

bool WindowSystem::Initialize(uint windowWidth, uint windowHeight, const std::string& windowName, GLFWkeyfun KeyCallback)
{
    glfwSetErrorCallback(GlfwErrorCallback);
    if (!glfwInit()) {
        printf("Error initializing GLFW!\n");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    this->m_width = windowWidth;
    this->m_height = windowHeight;

    this->m_pWindow = glfwCreateWindow(this->m_width, this->m_height, windowName.c_str(), NULL, NULL);
    if (!this->m_pWindow)
    {
        printf("Error creating window '%s'!\n", windowName.c_str());
        glfwTerminate();
        return false;
    }
    glfwSetKeyCallback(this->m_pWindow, KeyCallback);

    glfwMakeContextCurrent(this->m_pWindow);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);

    // Enabling error outputs
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    return true;
}

void WindowSystem::Destroy()
{
    if (this->m_pWindow) {
        glfwDestroyWindow(this->m_pWindow);
    }
    glfwTerminate();
}

void WindowSystem::NewFrame(int shaderID)
{
    // Update viewport
    glfwGetFramebufferSize(this->m_pWindow, &this->m_width, &this->m_height);
    this->m_ratio = this->m_width / (float)this->m_height;
    glViewport(0, 0, this->m_width, this->m_height);

    // Clear frame
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // While drawing a pixel, see if the pixel that's already there is closer or not?
    glEnable(GL_DEPTH_TEST);
    // set to not draw "back facing" triangles
    glCullFace(GL_BACK);
}

void WindowSystem::UpdateUL(int shaderID)
{
    if(this->m_ratio == 0.0)
    {
        return;
    }

    // Update matrix projection in shader
    glm::mat4 matProjection = glm::perspective(0.6f,
                                               this->m_ratio,
                                               1.0f,
                                              1000.0f);
    GLint matProjectionUL = this->m_pShaderInfo->GetUL(shaderID, "matProjection");
    glUniformMatrix4fv(matProjectionUL, 1, GL_FALSE, glm::value_ptr(matProjection));
}

void WindowSystem::EndFrame()
{
    glfwSwapBuffers(this->m_pWindow);
    glfwPollEvents();
}

bool WindowSystem::WindowShouldClose() {
    return glfwWindowShouldClose(this->m_pWindow);
}

float WindowSystem::GetCurrentWindowRatio() {
    return this->m_ratio;
}