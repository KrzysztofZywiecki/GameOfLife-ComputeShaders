#ifndef MAGNIFYINGGLASS_H
#define MAGNIFYINGGLASS_H

#include "Graphics.h"
#include "Rectangle.h"

class MagnifyingGlass
{
public:
    MagnifyingGlass();
    ~MagnifyingGlass();
    void Init(float radious, float zoom);
    void Update();
    void Draw(unsigned int texture);
    void SwapFullscreen();
private:
    const char* vertexShaderPath = "Shaders/magVS.txt";
    const char* fragmentShaderPath = "Shaders/magFS.txt";
    const char* geometryShaderPath = "Shaders/magGS.txt";

    void ProcessKeyboardInput();

    unsigned int VAO;
    unsigned int position_L;
    unsigned int radious_L;
    unsigned int zoom_L;
    unsigned int shader;
    float position[2];
    float radious;
    float zoom;
    bool fullscreen = false;
    bool show = false;
    bool buttonClicked = false;
};

#endif // MAGNIFYINGGLASS_H
