#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <chrono>

void GLClearError();
bool GLLogCall(const char* function);

class Graphics
{
public:
    Graphics();
    ~Graphics();
    static void Init( int window_width, int window_height, const char* window_title );
    static void Clear();
    static void PollEvents();
    static void UpdateDisplay();
    static GLFWwindow* GetWindow();
    static float GetPreviousFrameTime();
    static void GetWindowResolution(int* width, int* height);
    static unsigned int LoadShader(unsigned int type, const char* path);
    static void CursorPosInWindowSpace(float* x, float* y);
    static void CursorPosInTextureSpace(float* x, float* y);
private:
    static void UpdateClock();
    static int resolutionX;
    static int resolutionY;
    static float frameTime;
    static std::chrono::steady_clock::time_point previous;
    static GLFWwindow* window;
};

#endif // GRAPHICS_H
