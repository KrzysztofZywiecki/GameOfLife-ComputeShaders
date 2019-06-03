#include "Graphics.h"

#define ASSERT(x) if (!(x)) DebugBreak();

#define GlCall(x) GLClearError();\
        x;\
        ASSERT(GLLogCall(#x))



void GLClearError(){
    while(glGetError() != GL_NO_ERROR);
}
bool GLLogCall(const char* function){
    while(GLenum error = glGetError()){
        std::cout<<"OpenGL error: "<<error<<std::endl<<function<<std::endl;
        return false;
    }
    return true;
}

int Graphics::resolutionX = 0;
int Graphics::resolutionY = 0;
float Graphics::frameTime = 0.0f;
GLFWwindow* Graphics::window = nullptr;
std::chrono::steady_clock::time_point Graphics::previous;

Graphics::Graphics()
{
    //ctor
}

Graphics::~Graphics()
{
    //dtor
}

void Graphics::Init(int window_width, int window_height, const char* window_title){
    if(!glfwInit())
        exit(-1);
    Graphics::window = glfwCreateWindow(window_width, window_height, window_title, glfwGetPrimaryMonitor(), nullptr);
    if(!Graphics::window){
        glfwTerminate();
        exit(-2);
    }
    resolutionX = window_width;
    resolutionY = window_height;
    glfwMakeContextCurrent(Graphics::window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    Graphics::previous = std::chrono::steady_clock::now();
}

void Graphics::Clear(){
    glfwDestroyWindow(Graphics::window);
    glfwTerminate();
}

void Graphics::UpdateDisplay(){
    glfwSwapBuffers(Graphics::window);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    Graphics::UpdateClock();
}

void Graphics::PollEvents(){
    glfwPollEvents();
}

GLFWwindow* Graphics::GetWindow(){
    return window;
}

unsigned int Graphics::LoadShader(unsigned int type, const char* path){
    unsigned int shader;
    std::ifstream file;
    char* source = new char[2048];
    char* pBuf = source;

    shader = glCreateShader(type);
    file.open(path);
    if(!file.good()){
        std::cout<<"Nie istnieje taki plik";
        return 0;
    }

    file.get(source, 2048, '\0');
    glShaderSource(shader, 1, &pBuf, nullptr);
    glCompileShader(shader);
    int compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if(compiled != GL_TRUE){
        glGetShaderInfoLog(shader, 2048, &compiled, pBuf);
        std::cout<<source;
        return 0;
    }
    return shader;
}

void Graphics::GetWindowResolution(int* width, int* height){
    *width = Graphics::resolutionX;
    *height = Graphics::resolutionY;
}

float Graphics::GetPreviousFrameTime(){
    return Graphics::frameTime;
}

void Graphics::UpdateClock(){
    std::chrono::steady_clock::time_point temp;
    temp = std::chrono::steady_clock::now();
    Graphics::frameTime = std::chrono::duration_cast<std::chrono::milliseconds>(temp-Graphics::previous).count();
    Graphics::previous = temp;
}

void Graphics::CursorPosInWindowSpace(float *x, float *y){
    double xPos, yPos;
    glfwGetCursorPos(Graphics::GetWindow(), &xPos, &yPos);
    *x = ((float)(xPos)/(float)(resolutionX));
    *x = (*x * 2.0f) - 1.0f;
    *y = ((float)(yPos)/(float)(resolutionY));
    *y = -((*y * 2.0f) - 1.0f);
}

void Graphics::CursorPosInTextureSpace(float* x, float* y){
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    *x = (float)(xPos/resolutionX);
    *y = (float)(yPos/resolutionY);
}

