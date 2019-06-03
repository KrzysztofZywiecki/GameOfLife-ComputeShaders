#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "Rectangle.h"

class Background
{
public:
    Background();
    ~Background();
    void Draw();
    void Create();
private:
    const char* vertexShaderPath = "Shaders/vs.txt";
    const char* fragmentShaderPath = "Shaders/bgrFS.txt";
    Geometry::Rectangle rect;
    unsigned int shader;
};

#endif // BACKGROUND_H
